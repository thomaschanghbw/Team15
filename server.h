#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "connection.h"
#include <set>
namespace Team15 {
namespace server {

class server
{
 public:
  //Constructor
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  explicit server(const std::string& address, const std::string& port);
  
  // Run the server's io loop
  void run();
 private:
  void start_accepting();
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::acceptor acceptor_;
  typedef std::shared_ptr<connection> connection_ptr;
  std::set<connection_ptr> connections_;

};
}
}
#endif