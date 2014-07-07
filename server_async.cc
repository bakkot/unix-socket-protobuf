#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <boost/asio.hpp>
#include "msg.pb.h"

#include "asio_server.h"

using namespace std;
using boost::asio::local::stream_protocol;

#define ADDRESS "./socket"


class handler: public asio_handler<handler> {
 public:
 	handler(boost::asio::io_service& io_service) : asio_handler(io_service) {}
 
    void handle_initial_read(const boost::system::error_code& error, size_t bytes_transferred) {
    	if(error) {cerr << error.message() << endl; return;}

      	message_length = ntohl(message_length);
      	
      	cout << message_length << endl;
      	
      	message.resize(message_length);
      	boost::asio::async_read(socket_, boost::asio::buffer(message),
			bind(&handler::handle_message, shared_from_this(), std::placeholders::_1));
    }
    
    void handle_message(const boost::system::error_code& error) {
    	if(error) {cerr << error.message() << endl; return;}
    	
    	asio::Boring msg;
    	bool r = msg.ParseFromArray(message.data(), message.size());
    	cout << "Message: " << msg.cont() << endl;
    }

	void handle() {
		auto handler = bind(&handler::handle_initial_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
		
		boost::asio::async_read(socket_, boost::asio::buffer(&message_length, 4), handler);
	}

 private:
	vector<uint8_t> message;
	
	uint32_t message_length = 0;
};



	

int main() {
	try {
		boost::asio::io_service io_service;
		unlink(ADDRESS);
		stream_protocol::endpoint ep(ADDRESS);
		asio_server<handler> s(io_service, ep);
		
		io_service.run();
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}
