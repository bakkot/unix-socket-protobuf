.PHONY: all
all: gen/msg_pb2.py

gen/msg_pb2.py: ../msg.proto
	mkdir -p gen
	touch gen/__init__.py # I hate this.
	protoc --python_out=gen -I .. ../msg.proto

.PHONY: clean
clean:
	rm -f gen/*

.PHONY: really_clean
really_clean:
	rm -rf gen
