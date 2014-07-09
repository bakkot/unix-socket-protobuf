
CPP_BINS = client_sync server_sync server_async

GO_BINS = client server

# consider also -lboost_thread-mt -lpthread
CPP_LIBS = -lboost_system-mt -lprotobuf
CPP_FLAGS = -Wall -O2 --std=c++11 -L/usr/local/lib 

all: CPP Python

CPP: $(CPP_BINS)

Python: Python/gen/msg_pb2.py

Go: $(GO_BINS)

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


$(GO_BINS): %: Go/src/%/main.go Go/src/example_mess/msg.pb.go Go/src/code.google.com/p/goprotobuf
	GOPATH=$(CURDIR)/Go go install $@
	cp Go/bin/$@ bin/go_$@

Go/src/example_mess/msg.pb.go: msg.proto
	protoc --go_out=Go/src/example_mess msg.proto
	
Go/src/code.google.com/p/goprotobuf:
	GOPATH=$(CURDIR)/Go go get code.google.com/p/goprotobuf/proto


.PHONY : clean
clean:
	rm -f C++/bin/* C++/gen/* Python/gen/* bin/* Go/pkg/*/example_mess.a Go/bin/*

.PHONY : really_clean
really_clean: clean
	rm -f Go/src/example_mess/msg.pb.go
	rm -rf Go/pkg/* Go/src/code.google.com
	