#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "server.h"

const int low_invalid_port = 1023;
const int high_invalid_port = 65536;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

namespace Team15 {
namespace server {

  server::server(const std::string& address, const std::string& port, const NginxConfig& config):
    io_service_(),socket_(io_service_), acceptor_(io_service_),requestMgr_(config){
    if (!this->is_valid(address, port)) {
      fprintf(stderr, "Error: Invalid port input");
      exit(1);
    }

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    start_accepting();
  }

  bool server::is_valid(const std::string& address, const std::string& port) {
    int port_num = atoi(port.c_str());
    if (port == "") {
      printf("Empty port input.\n");
      return false;
    }
    if (!(port_num > low_invalid_port && port_num < high_invalid_port)) {
      printf("Invalid port input.\n");
      return false;
    }  
    return true;
  }

  void server::start_accepting() {
    std::cout << "Accepting connections." << std::endl;
    acceptor_.async_accept(socket_,
			   [this](boost::system::error_code ec)
			   {
			     if (!acceptor_.is_open()) { std::cerr << "Failed to open" << std::endl; return; }
			     if (!ec) {
			       connection* c_p = new connection(std::move(socket_),this);
			       connections_.insert(c_p);
			       std::cout << "Socket opened, Starting connection." << std::endl;
			       c_p->start();
			     }
			     else {
			       std::cerr << "Found error" << std::endl;
			     }
			     start_accepting();
			   });
  }
  void server::run() {
    io_service_.run();
  }
  void server::connection_done(connection* connection) {
    connections_.erase(connection);
    connection->stop();
    free(connection);
  }
  server::~server() {
    for (auto connection : connections_) {
      connection->stop();
      free(connection);
    }
  }
  boost::asio::io_service& server::getService()  {
    return io_service_;
  }
  //  const std::vector<requestconfig>& server::getRequestConfigVector() const {
  //   return requestConfigVector_;
  //}
}
}
