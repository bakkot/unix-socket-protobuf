BINS := client_sync server_sync server_async
BINDIR := bin
# consider also -lboost_thread-mt -lpthread
CPP_LIBS = -lboost_system-mt -lprotobuf
CPP_FLAGS = -Wall -O2 --std=c++11 -L/usr/local/lib 


OUTBINS := $(addprefix $(BINDIR)/,$(BINS))



.PHONY: all
all: $(BINS)

.PHONY: $(BINS)
$(BINS): %: $(BINDIR)/%

$(OUTBINS): $(BINDIR)/%: src/%.cc gen/msg.pb.cc src/asio_server.h | $(BINDIR)
	clang++ $(CPP_FLAGS) src/$*.cc gen/msg.pb.cc -o bin/$* $(CPP_LIBS)

$(BINDIR):
	mkdir -p $(BINDIR)

gen/msg.pb.cc: ../msg.proto
	mkdir -p gen
	protoc --cpp_out=gen -I .. ../msg.proto



.PHONY: clean
clean:
	rm -f bin/* gen/*

.PHONY: really_clean
really_clean:
	rm -rf gen $(BINDIR)
