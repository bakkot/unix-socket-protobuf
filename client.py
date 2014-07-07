import socket
from struct import pack
import msg_pb2
from time import sleep #TODO

sock = socket.socket(socket.AF_UNIX)
address = './socket'


def str2bytes(s):
	return pack(str(len(s)) + 's', s)


msg = msg_pb2.Boring()
msg.cont = "awww yisss"
encoded = msg.SerializeToString()
bytes = str2bytes(encoded)

try:
	sock.connect(address)
except socket.error as msg:
    print(msg)
    exit(1)

try:
	x = pack('>I', len(bytes))
	sock.sendall(x)
	sock.sendall(bytes)
except Exception as e:
	print(e)
	exit(1)