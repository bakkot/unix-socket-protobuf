#include <iostream>
#include <array>
#include <vector>
#include <boost/asio.hpp>
#include "msg.pb.h"


#include <bitset> // todo

#include "asio_server.h"

using namespace std;

#define ADDRESS "./socket"



class handler: public asio_handler<handler> {
 public:
 	handler(boost::asio::io_service& io_service) : asio_handler(io_service) {}

	void handle() {
		vector<uint8_t> message;
		
		uint32_t message_length = 0;

		boost::asio::read(socket_, boost::asio::buffer(&message_length, 4));
		message_length = ntohl(message_length);
      	
      	cout << message_length << endl;

      	message.resize(message_length);
		boost::asio::read(socket_, boost::asio::buffer(message));

    	asio::Boring msg;
    	bool r = msg.ParseFromArray(message.data(), message.size());
    	cout << "Message: " << msg.cont() << endl;
	}
};



int main() {
	try {
		boost::asio::io_service io_service;
		unlink(ADDRESS);
		boost::asio::local::stream_protocol::endpoint ep(ADDRESS);
		asio_server<handler> s(io_service, ep);
		
		io_service.run();
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}
