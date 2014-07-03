TARGET = asio

# consider also -lboost_thread-mt -lpthread
$(TARGET): $(TARGET).cc
	clang++ --std=c++11 $(TARGET).cc -o $(TARGET) -L/usr/local/lib -lboost_system-mt

clean:
	rm -f $(TARGET)