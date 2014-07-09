import SocketServer
from struct import unpack
from os import unlink
from gen import msg_pb2


address = './socket'

class Session(SocketServer.BaseRequestHandler):
	def handle(self):
		header = self.request.recv(4)
		message_length, = unpack('>I', header) #unpack always returns a tuple.
		print(message_length)
		
		message = self.request.recv(message_length)
		pb_message = msg_pb2.Boring()
		pb_message.ParseFromString(message)
		
		print("Message: " + pb_message.cont)
		

try:
	unlink(address)
except OSError as e:
	pass

server = SocketServer.UnixStreamServer(address, Session)
server.serve_forever()
