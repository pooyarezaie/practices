"""
root -> 192.168.1.0
agent -> 192.168.1.1
com -> 192.168.1.2
org -> 192.168.1.3
google -> 192.168.1.4
gnu -> 192.168.1.5
"""
from mininet.net import Mininet
from mininet.topo import SingleSwitchTopo
import threading
import time


def run_and_print(host, cmd):
    run_thread = threading.Thread(target=host.cmd,
                                  args=[cmd])
    run_thread.start()
    while True:
        print host.stdout.readline()

nodes = ['root', 'com', 'org', 'google', 'gnu']
ips = ['192.168.1.0', '192.168.1.2', '192.168.1.3',
       '192.168.1.4', '192.168.1.5']

nodes = dict(zip(nodes, ips))

topo = SingleSwitchTopo(k=len(nodes)+2)
net = Mininet(topo=topo)

client_host = net.hosts[-1]
agent_host = net.hosts[-2]

client_host.setIP('192.168.1.6')
agent_host.setIP('192.168.1.1')


for node, host in zip(nodes, net.hosts[:-2]):
    host.setIP(nodes[node])


net.start()
net.pingAll()

threads = []

agent_thread = threading.Thread(target=run_and_print,
                                args=[agent_host, 'python run_agent.py'])
threads.append(agent_thread)
agent_thread.start()
for node, host in zip(nodes, net.hosts[:-2]):
    print "starting %s " % node
    chost_thread = threading.Thread(target=run_and_print,
                                    args=[host, 'python run_node.py %s' %
                                          node])
    threads.append(chost_thread)
    chost_thread.start()


time.sleep(5)
print client_host.cmdPrint('nc 192.168.1.1 12345 < samples/request1')
time.sleep(2)
print client_host.cmdPrint('nc 192.168.1.1 12345 < samples/request1')

for thread in threads:
    thread.join()
