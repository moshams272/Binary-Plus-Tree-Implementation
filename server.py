import http.server
import webbrowser
import os

PORT = 8080

os.chdir(os.path.dirname(os.path.abspath(__file__)))

handler = http.server.CGIHTTPRequestHandler
server = http.server.HTTPServer(("", PORT), handler)

print(f"🚀 Server running at http://localhost:{PORT}")
webbrowser.open(f"http://localhost:{PORT}/index.html")

try:
    server.serve_forever()
except KeyboardInterrupt:
    server.server_close()
    print("Server Stopped.")