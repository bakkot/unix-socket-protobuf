Protocol Buffers Over Sockets
=============================

I needed a little project to play with Google's [Protocol Buffers](https://code.google.com/p/protobuf/), a data-serialization tool. I also wanted to try using Unix domain sockets (like network sockets, but purely local). This was the result. 

Building
--------

You'll need to have Protocol Buffers installed locally, for the protoc compiler and its libraries. Both of those need to be on your path.

With that done (and modulo the caveats below), just `make`. Or `make CPP`, `make Python`, or `make Go` for individual targets (especially if, eg, you don't have Go installed). If you made all targets, binaries will be copied to the top-level bin/ folder; otherwise they'll be in their respective lower-level bin/ folders (with the exception of Python, which has `client.py` and `server.py` at the top level).

### Go notes

If you want to build the Go sample, you'll need to have golang installed. If you want to build the Go protobuff file (not necessary, as it's included), you'll also need Google's [goprotobuf](code.google.com/p/goprotobuf).

### Python notes

This is written in Python 2, because Protobuf doesn't natively support Python 3.

### C++ notes

You'll need Boost installed, because doing networking (even just Unix domain sockets) without Boost is less than fun. You may need to modify CPP/Makefile's `CPP_FLAGS` to reflect where libraries live on your machine.


Running
-------

Run one server at a time: eg `bin/CPP_server_async`. This will create (or overwrite) a pseudo-file (actually a Unix domain socket) named 'socket' in the local directory. From the same directory, run any client: eg `bin/Go_client`. You should see a message in the server window, with its length above it. That's it! You can run the client again, or another client, without restarting the server. When you're done, ctrl-c the server and `rm socket`.
