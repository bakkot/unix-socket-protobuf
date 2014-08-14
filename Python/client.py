import socket
from struct import pack
from gen import msg_pb2

sock = socket.socket(socket.AF_UNIX)
address = './socket'


msg = msg_pb2.Boring()
msg.cont = "A very Pythonic hello to you!"
encoded = msg.SerializeToString()

try:
	sock.connect(address)
except socket.error as e:
		print(e)
		exit(1)

try:
	x = pack('>I', len(encoded))
	sock.sendall(x)
	sock.sendall(encoded)
except Exception as e:
	print(e)
	exit(1)
