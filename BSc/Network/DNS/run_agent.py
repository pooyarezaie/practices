from agent import DNSAgent
from utils import read_json_file

config_file_path = "config/agent.json"
config = read_json_file(config_file_path)

agent = DNSAgent(config)
agent.run()
