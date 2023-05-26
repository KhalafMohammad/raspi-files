# echo-client.py

import socket

HOST = "0.0.0.0"  # The server's hostname or IP address
PORT = 8989  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"F")
    data = s.recv(1024)
    # while True:
    #     message = input("Enter a message to send to the server: ")
    #     s.sendall(message.encode())
    #     data = s.recv(1024)
    #     print(f"Received from server: {data.decode()}")

print(f"Received {data!r}")