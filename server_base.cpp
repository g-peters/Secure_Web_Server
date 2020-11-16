
#include <iostream>
#include "tcp_server.hpp"
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

const int max_length = 1024;

void server_start(int port_number, boost::asio::io_context& io_context)
{

	tcp_server session(io_context,12345);

}

