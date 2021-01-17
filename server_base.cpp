
#include "tcp_server.hpp"
#include <boost/asio.hpp>


void server_start(int port_number, boost::asio::io_context& io_context, logger& logs)
{
	std::string start = "server started";
	tcp_server server(io_context, port_number, logs);

}

