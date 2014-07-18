
TARGETS := CPP Python Go

all: $(TARGETS) links

.PHONY: $(TARGETS)
$(TARGETS):
	cd $@ && $(MAKE)

	


.PHONY: links
links:
	mkdir -p bin
	$(foreach target,CPP Go, \
		$(foreach bin,$(wildcard $(target)/bin/*), cp $(bin) bin/$(target)_$(notdir $(bin));) \
	)
	if [ -a Python/gen/msg_pb2.py ] ; then \
		echo "#!/bin/sh\n\npython $(CURDIR)/Python/client.py" > bin/Python_client.sh; \
		chmod +x bin/Python_client.sh; \
		echo "#!/bin/sh\n\npython $(CURDIR)/Python/server.py" > bin/Python_server.sh; \
		chmod +x bin/Python_server.sh; \
	fi;

.PHONY: clean
clean:
	$(foreach target,$(TARGETS),$(MAKE) -C $(target) clean;)
	rm -f bin/*
	

.PHONY: really_clean
really_clean: clean
	$(foreach target,$(TARGETS),$(MAKE) -C $(target) really_clean;)
	rm -rf bin
