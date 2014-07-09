
BINS = client_sync server_sync server_async

# consider also -lboost_thread-mt -lpthread
LIBS = -lboost_system-mt -lprotobuf
CFLAGS = -Wall -O2 --std=c++11 -L/usr/local/lib 

all: $(BINS) msg_pb2.py


$(BINS): %: %.cc msg.pb.cc asio_server.h
	clang++ $(CFLAGS) $@.cc msg.pb.cc -o $@ $(LIBS)

msg.pb.cc: msg.proto
	protoc --cpp_out=. msg.proto

msg_pb2.py: msg.proto
	protoc --python_out=. msg.proto

.PHONY : clean
clean:
	rm -f $(BINS) msg.pb.cc msg.pb.h msg_pb2.py msg_pb2.pyc
