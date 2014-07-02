#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::local::stream_protocol;

int main() {
	try {
		boost::asio::io_service io_service;
		unlink("./socket");
		stream_protocol::endpoint ep("./socket");
		stream_protocol::acceptor acceptor(io_service, ep);
		stream_protocol::socket socket(io_service);
		acceptor.accept(socket);
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}