import socket
import struct

sock = socket.socket(socket.AF_UNIX)
address = './socket'

try:
	sock.connect(address)
except socket.error as msg:
    print(msg)
    sys.exit(1)

try:
	x = struct.pack('>I', 1337)
	sock.sendall(x)
except Exception as e:
	print(e)
	exit(1)