#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <array>
#include <vector>
#include <boost/asio.hpp>
#include "msg.pb.h"

#include "asio_server.h"

using namespace std;
using boost::asio::local::stream_protocol;

#define INITIAL_BUFFER_SIZE 256
#define ADDRESS "./socket"


class session : public enable_shared_from_this<session> {
 public:
	session(boost::asio::io_service& io_service)
    : socket_(io_service) {}
    
    void handle_initial_read(const boost::system::error_code& error) {
      	if(error) return;

      	for(auto x: header_buffer) {
      		message_length <<= 8;
      		message_length += x;
      	}
      	
      	cout << message_length << endl;
      	
      	message.resize(message_length);
      	boost::asio::async_read(socket_, boost::asio::buffer(message),
			bind(&session::handle_message, shared_from_this(), std::placeholders::_1));
    }
    
    void handle_message(const boost::system::error_code& error) {
    	if(error) {cerr << error.message() << endl; return;}
    	
    	asio::Boring msg;
    	bool r = msg.ParseFromArray(message.data(), message.size());
    	cout << "Message: " << msg.cont() << endl;
    }
    
    
    
    stream_protocol::socket& socket() {
    	return socket_;
    }

	void handle() {
		boost::asio::async_read(socket_, boost::asio::buffer(header_buffer.data(), 4),
			bind(&session::handle_initial_read, shared_from_this(), std::placeholders::_1));
	}
	
	
 private:
	stream_protocol::socket socket_;
	array<uint8_t, 4> header_buffer = {};
	vector<uint8_t> message;
	
	uint32_t message_length = 0;
	
};



	

int main() {
	try {
		boost::asio::io_service io_service;
		unlink(ADDRESS);
		stream_protocol::endpoint ep(ADDRESS);
		asio_server<session> s(io_service, ep);
		
		io_service.run();
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}
