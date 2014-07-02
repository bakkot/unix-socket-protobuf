TARGET = asio

# consider also -lboost_thread-mt -lpthread
$(TARGET): $(TARGET).cc
	clang++ $(TARGET).cc -o $(TARGET) -L/usr/local/lib -lboost_system-mt

clean:
	rm -f $(TARGET)