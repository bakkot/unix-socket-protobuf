#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <array>
#include <vector>
#include <boost/asio.hpp>
#include "msg.pb.h"

using namespace std;
using boost::asio::local::stream_protocol;

#define INITIAL_BUFFER_SIZE 256
#define ADDRESS "./socket"


class session : public enable_shared_from_this<session> {
 public:
	session(boost::asio::io_service& io_service)
    : socket_(io_service) {
    	//cout << "Someone wants to talk!" << (this) << endl;
    }
    
    ~session() {
    	//cout << "done talking..." << (this) << endl;
    }
    
    
    
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

	void start() {
		boost::asio::async_read(socket_, boost::asio::buffer(header_buffer.data(), 4),
			bind(&session::handle_initial_read, shared_from_this(), std::placeholders::_1));
	}
	
	
 private:
	stream_protocol::socket socket_;
	array<uint8_t, 4> header_buffer = {};
	vector<uint8_t> message;
	
	uint32_t message_length = 0;
	
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
		unlink(ADDRESS);
		stream_protocol::endpoint ep(ADDRESS);
		server s(io_service, ep);
		
		io_service.run();
		
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
	
	return 0;
}
