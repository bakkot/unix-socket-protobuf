
CPP_BINS = client_sync server_sync server_async

# consider also -lboost_thread-mt -lpthread
CPP_LIBS = -lboost_system-mt -lprotobuf
CPP_FLAGS = -Wall -O2 --std=c++11 -L/usr/local/lib 

all: CPP Python

CPP: $(CPP_BINS)

Python: Python/gen/msg_pb2.py

$(CPP_BINS): %: C++/src/%.cc C++/gen/msg.pb.cc C++/src/asio_server.h
	clang++ $(CPP_FLAGS) C++/src/$@.cc C++/gen/msg.pb.cc -o C++/bin/$@ $(CPP_LIBS)
	cp C++/bin/$@ bin/cpp_$@

C++/gen/msg.pb.cc: msg.proto
	protoc --cpp_out=C++/gen msg.proto

Python/gen/msg_pb2.py: msg.proto
	touch Python/gen/__init__.py # I hate this.
	protoc --python_out=Python/gen msg.proto
	echo "#!/bin/sh\n\npython Python/client.py" > bin/python_client.sh
	chmod +x bin/python_client.sh
	echo "#!/bin/sh\n\npython Python/server.py" > bin/python_server.sh
	chmod +x bin/python_server.sh

.PHONY : clean
clean:
	rm -f C++/bin/* C++/gen/* Python/gen/* bin/*
