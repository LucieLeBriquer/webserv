import sys
import socket
import selectors
import types

HOST = '127.0.0.1'
PORT = 8080
message = "GET / HTTP/1.1\r\nhost: localhost\r\n\r\n"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        print('========connexion========', end='\n\n')
        print('Connecting to ' + HOST + ':' + str(PORT) + ' successful.', end='\n\n')
        print('=========sending=========', end='\n\n')
        print(message, end='\n\n')
        s.sendall(bytes(message, encoding= 'utf-8'))
        print('========receiving========', end='\n\n')
        try:
                data = s.recv(1024)
                while data:
                        print(data.decode())
                        data = s.recv(1024)
        finally:
                print('=======deconnexion=======')
                s.close()
