# test_data/mock_server.py
from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import random

class MockServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        
        # 生成随机订单簿数据
        bid_price = round(100 + random.random(), 2)
        ask_price = round(bid_price + 0.1 + random.random()*0.2, 2)
        
        response = {
            "bids": [[bid_price, 50]],
            "asks": [[ask_price, 30]]
        }
        self.wfile.write(json.dumps(response).encode())

if __name__ == '__main__':
    server = HTTPServer(('localhost', 8080), MockServer)
    print("Mock server running on port 8080...")
    server.serve_forever()