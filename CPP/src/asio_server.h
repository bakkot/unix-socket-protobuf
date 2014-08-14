/*
This seemed a sufficiently common pattern so as to be worth abstracting. Basically the simplest sensible server and handler, where the server dispatches a new handler per connection. Using this class makes setting up a server almost as easy as in Python: just defining a handle() method and inheriting appropriately.
*/

#ifndef ASIO_SERVER_H
#define ASIO_SERVER_H

#include <functional>
#include <memory>
#include <boost/asio.hpp>


// Convenience class for handlers.
// To use, define a class Foo which inherits from asio_handler<Foo>, calls its
// constructor, and overwrites 'handle'.
// eg:
/*
class handler: public asio_handler<handler> {
 public:
  handler(boost::asio::io_service& io_service) : asio_handler(io_service) {}
  void handle() {
    // do something like read from socket_
  }
}

*/ 
template<typename T>
class asio_handler : public std::enable_shared_from_this<T> {
 public:
  asio_handler(boost::asio::io_service& io_service)
    : socket_(io_service) {}
        
    boost::asio::local::stream_protocol::socket& socket() {
      return socket_;
    }

  void handle() {
  
  }
  
 protected:
  boost::asio::local::stream_protocol::socket socket_;  
};


// HANDLER must have a constructor with a 'boost::asio::io_service&' as its sole parameter,
// must implement a 'handle' fn taking no parameters.
// and must implement a 'socket' fn taking no parameters and returning a 'boost::asio::local::stream_protocol::socket'
// this being the socket it wants to talk on.
// In practice, just ensure HANDLER derives from asio_handler.
template<typename HANDLER>
class asio_server {
 public:
  boost::asio::io_service& io_service_;
  boost::asio::local::stream_protocol::acceptor acceptor_;
  
  asio_server(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint endpoint)
  : io_service_(io_service), acceptor_(io_service, endpoint) {
    auto the_session = std::make_shared<HANDLER>(io_service_);
    acceptor_.async_accept(the_session->socket(), std::bind(&asio_server::handle_accept, this, the_session, std::placeholders::_1));
  }
  
  void handle_accept(std::shared_ptr<HANDLER> session, const boost::system::error_code& error) {
    if(!error)
      session->handle();
    
    session.reset(new HANDLER(io_service_));
    acceptor_.async_accept(session->socket(), std::bind(&asio_server::handle_accept, this, session, std::placeholders::_1));
  }
};


#endif
