TARGET = client_sync

# consider also -lboost_thread-mt -lpthread
$(TARGET): $(TARGET).cc msg.pb.cc
	clang++ -Wall -O2 --std=c++11 $(TARGET).cc msg.pb.cc -o $(TARGET) -L/usr/local/lib -lboost_system-mt -lprotobuf

msg.pb.cc:
	protoc --cpp_out=. msg.proto

clean:
	rm -f $(TARGET) msg.pb.cc msg.pb.h
