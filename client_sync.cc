#include <iostream>
#include <boost/asio.hpp>

#include "msg.pb.h"

using namespace std;

#define ADDRESS "./socket"

int main() {
	try {
		boost::asio::io_service io_service;
		boost::asio::local::stream_protocol::socket socket(io_service);
	
		socket.connect(boost::asio::local::stream_protocol::endpoint(ADDRESS));
	
		asio::Boring msg;
		msg.set_cont("Greetings from CPP-land!");
	
	
		boost::asio::streambuf sbuf;
		ostream serialized(&sbuf);
		msg.SerializeToOstream(&serialized);
		
		uint32_t message_length = sbuf.size();
		
		message_length = htonl(message_length); // endianness!
		
		boost::asio::write(socket, boost::asio::buffer(&message_length, 4));
		boost::asio::write(socket, sbuf);
	}
	catch(exception& e) {
		cout << e.what() << endl;
	}	
}
