import copy
import json
import socket
import threading
from SocketServer import ThreadingTCPServer, BaseRequestHandler
from datetime import datetime
from time import sleep

from utils import setup_logger, MutexCounter
import settings


class DNSNode(object):
    ALLOWED_ACTIONS = {'resolve_iterative', 'resolve_recursive', 'add_node'}

    class RequestHandler(BaseRequestHandler):
        def __init__(self, node, *args, **kwargs):
            self.node = node
            BaseRequestHandler.__init__(self, *args, **kwargs)

        def handle(self):
            self.node.logger.debug("new connection")
            
            data = self.request.recv(settings.MAX_MESSAGE_LEN)
            request = json.loads(data)
            
            response = self.node.dispatch(request)
            if response is None:
                response = {}

            self.request.sendall(json.dumps(response))

            self.node.logger.debug("connection closed")

        @classmethod
        def handler_factory(cls, node):
            def create_handler(*args, **kwargs):
                return cls(node, *args, **kwargs)

            return create_handler

    def __init__(self, config, data):
        self.logger = setup_logger(config['name'])
        self.logger.debug('first log')
        self.logger.debug('constructing node with config %s' % config)
        self.config = config
        
        self.data = data
        self.dns = data['dns']
        self.address = data['address']

        host, port = self.config['host'], self.config['port']
        self.tcp_server = ThreadingTCPServer(
            (host, port),
            DNSNode.RequestHandler.handler_factory(self),
        )
        self.cached_address = {}
        self.request_counter = MutexCounter()

    def dispatch(self, request):
        self.request_counter.increment()

        action = request.get('action')

        if action in DNSNode.ALLOWED_ACTIONS:
            try:
                return getattr(self, action)(request)
            except Exception as e:
                self.logger.error(self.config['name'] + str(e.message))
                return {'details': "bad request"}

    def _get_top_domain(self, name):
        match = dict()
        for dns in self.dns:
            length = len(dns)
            if name[len(name)-length:] == dns:
                match[length] = dns

        servers = self.dns[match[max(match)]] if match else None

        result = servers[0]
        servers.erase(0)
        servers.append(result)

        return result

    def _local_resolve(self, name):
        self.logger.debug("resolve locally %s" % name)

        if name in self.address:
            return {
                'type': 'A',
                'ip': self.address[name],
                'validity': self.config.get('validity', 0)
            }
        if name in self.cached_address:
            self.logger.debug("getting from cache")
            data = self.cached_address[name]
            received = data.get('received')

            seconds = (datetime.now()-received).total_seconds()

            validity = data.get('validity')
            if validity > seconds:
                return {
                    'type': 'A',
                    'ip': self.cached_address[name].get('ip'),
                    'validity': validity - seconds
                }

        return None

    def _can_resolve_locally(self, name):
        if name in self.address:
            return True

        if name in self.cached_address:
            data = self.cached_address[name]
            received, validity = data.get('received'), data.get('validity')
            seconds = (datetime.now()-received).total_seconds()
            return validity > seconds

    def _cache_record(self, name, data):
        cached_data = copy.copy(data)
        cached_data['received'] = datetime.now()
        self.cached_address[name] = cached_data

    def resolve_iterative(self, request):
        self.logger.debug("resolve iterative %s", request)

        name = request['name']

        if self._can_resolve_locally(name):
            return self._local_resolve(name)
        top_domain = self.dns[self._get_top_domain(name)]
        if top_domain is None:
            return {'type': 'F'}

        self.logger.debug("top domain found: %s" % top_domain)

        return {
            'type': 'NA',
            'ip': top_domain['ip'],
            'port': top_domain['port']
        }

    def resolve_recursive(self, request):
        self.logger.debug("resolve recursive %s" % request)

        name = request['name']

        if self._can_resolve_locally(name):
            return self._local_resolve(name)

        top_domain = self.dns[self._get_top_domain(name)]
        if top_domain is None:
            return {'type': 'F'}

        ip, port = top_domain['ip'], top_domain['port']
        self.logger.debug("top domain found: %s:%d" % (ip, port))
        connection = socket.create_connection((ip, port))
        connection.sendall(json.dumps(request))
        data = connection.recv(settings.MAX_MESSAGE_LEN)
        connection.close()

        data = json.loads(data)
        self._cache_record(name, data)
        return data

    def add_node(self, request):
        top_domain = request.pop('top_domain')

        if top_domain in self.dns:
            self.dns[top_domain].append(request)
        else:
            self.dns[top_domain] = [request]

    def _deploy_new_node(self):
        self.logger.debug('%s:%s deploying new node' % (self.config['name'],
                                                        self.config['port']))
        
        new_config = copy.deepcopy(self.config)
        new_config['port'] += 1
        self.config['load_balance'] = False

        new_node = DNSNode(new_config, self.data)
        new_thread = threading.Thread(target=new_node.run)
        new_thread.start()
        
        message = {
            'action': 'add_node',
            'ip': None,
            'port': None
        }

        root = self.config['root']
        root_ip, root_port = root['ip'], root['port']
        connection = socket.create_connection((root_ip, root_port))
        connection.sendall(json.dumps(message))
        connection.close()

    def run(self):
        self.logger.debug('starting new node %s:%s' % (self.config['name'],
                                                       self.config['port']))
        server_thread = threading.Thread(target=self.tcp_server.serve_forever)
        server_thread.start()

        while self.config['load_balance']:
            sleep(60)

            request_count = self.request_counter.get_and_reset()
            if request_count > settings.REQUEST_THRESHOLD:
                self._deploy_new_node()
