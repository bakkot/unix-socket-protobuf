BINS = client server
BINDIR := bin

OUTBINS := $(addprefix $(BINDIR)/,$(BINS))



.PHONY: all
all: $(BINS)

.PHONY: $(BINS)
$(BINS): %: $(BINDIR)/%

$(OUTBINS): $(BINDIR)/%: src/%/main.go src/example_mess/msg.pb.go src/code.google.com/p/goprotobuf
	GOPATH=$(CURDIR) go install $*

src/example_mess/msg.pb.go: ../msg.proto
	mkdir -p src/example_mess
	protoc --go_out=src/example_mess -I .. ../msg.proto
	
src/code.google.com/p/goprotobuf:
	GOPATH=$(CURDIR) go get code.google.com/p/goprotobuf/proto




.PHONY: clean
clean:
	rm -f pkg/*/example_mess.a bin/*

.PHONY: really_clean
really_clean: clean
	rm -f src/example_mess/msg.pb.go # in here because protoc go compiler not always installed
	rm -rf pkg bin src/code.google.com src/example_mess
