import json
import socket

from dns_node import DNSNode
import settings


class DNSAgent(DNSNode):
    def __init__(self, config):
        self.root = config.pop('root')
        
        data = {
            'dns': {'': self.root},
            'address': dict()
        }
        
        config['load_balance'] = False
        super(DNSAgent, self).__init__(config, data)

    def resolve_iterative(self, request):
        name = request['name']

        if self._can_resolve_locally(name):
            return self._local_resolve(name)
        
        resolver_ip, resolver_port = self.root['ip'], self.root['port']
        while True:
            self.logger.debug("resolve iterative, asking %s:%d" %
                              (resolver_ip, resolver_port))

            connection = socket.create_connection((resolver_ip, resolver_port))
            connection.sendall(json.dumps(request))
            data = connection.recv(settings.MAX_MESSAGE_LEN)
            connection.close()
            
            data = json.loads(data)

            dns_type = data['type']
            if dns_type == 'NA':
                resolver_ip, resolver_port = data['ip'], data['port']
            else:
                if dns_type == 'A':
                    self._cache_record(name, data)
                return data
