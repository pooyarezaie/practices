from dns_node import DNSNode
from utils import read_json_file
from sys import argv

config_file_path = "config/%s.json" % argv[1]
config = read_json_file(config_file_path)

data_file_path = "data/%s.json" % argv[1]
data = read_json_file(data_file_path)

node = DNSNode(config, data)
node.run()
