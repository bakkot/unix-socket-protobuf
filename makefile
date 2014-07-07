TARGET = server_async

# consider also -lboost_thread-mt -lpthread
$(TARGET): $(TARGET).cc msg.pb.cc
	clang++ --std=c++11 $(TARGET).cc msg.pb.cc -o $(TARGET) -L/usr/local/lib -lboost_system-mt -lprotobuf-lite

msg.pb.cc:
	protoc --cpp_out=. msg.proto

clean:
	rm -f $(TARGET) msg.pb.cc msg.pb.h
