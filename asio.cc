#include <iostream>
#include <string>
// #include <boost/array.hpp>
// #include <boost/bind.hpp>
// #include <boost/enable_shared_from_this.hpp>
// #include <boost/shared_ptr.hpp>
#include <functional>
#include <memory>
#include <array>
#include <vector>
#include <boost/asio.hpp>
#include <google/protobuf/io/coded_stream.h>

using namespace std;
using boost::asio::local::stream_protocol;

#define INITIAL_BUFFER_SIZE 256


class session : public enable_shared_from_this<session> {
 public:
	session(boost::asio::io_service& io_service)
    : socket_(io_service) {
    	cout << "Someone wants to talk!" << (this) << endl;
    }
    
    ~session() {
    	cout << "done talking..." << (this) << endl;
    }
    
    
    
    void handle_initial_read(const boost::system::error_code& error,
      size_t bytes_transferred) {
      	if(error) return;
      	google::protobuf::io::CodedInputStream cis(initial_buffer.data(), bytes_transferred);
      	auto r = cis.ReadVarint32(&message_length);
      	if(!r) {cerr << "what"<<endl; return;}
      	
      	cout << message_length << endl;
      }
    
    
    
    stream_protocol::socket& socket() {
    	return socket_;
    }

	void start() {
		socket_.async_read_some(boost::asio::buffer(initial_buffer.data(), INITIAL_BUFFER_SIZE),
			bind(&session::handle_initial_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	}
	
	// parse the size and return it. increments the given iterator, so it will start after the varint ends
	// only a member function for cleanness
	//static size_t parse_varint(array<uint8_t, INITIAL_BUFFER_SIZE>::iterator &x) {
		
	//}
	
 private:
	stream_protocol::socket socket_;
	array<uint8_t, INITIAL_BUFFER_SIZE> initial_buffer;
	vector<uint8_t> message;
	
	uint32_t message_length;
	
};



class server {
public:
	boost::asio::io_service& io_service_;
	stream_protocol::acceptor acceptor_;
	
	server(boost::asio::io_service& io_service, stream_protocol::endpoint endpoint)
	: io_service_(io_service), acceptor_(io_service, endpoint) {
		auto the_session = make_shared<session>(io_service_);
		acceptor_.async_accept(the_session->socket(), std::bind(&server::handle_accept, this, the_session, std::placeholders::_1));
	}
	
	void handle_accept(shared_ptr<session> session, const boost::system::error_code& error) {
		if(!error)
			session->start();
		
		session.reset(new class session(io_service_));
		acceptor_.async_accept(session->socket(), std::bind(&server::handle_accept, this, session, std::placeholders::_1));
	}
};
	

int main() {
	try {
		boost::asio::io_service io_service;
		unlink("./socket");
		stream_protocol::endpoint ep("./socket");
		server s(io_service, ep);
		
		io_service.run();
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}