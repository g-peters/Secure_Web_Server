#include "TCP_Server.hpp"


//std::vector<std::string> TCP_Server::non_allowed_strings = { "cmd","../","&&","uploads" };



TCP_Server::TCP_Server(boost::asio::io_context& io, USHORT port, Logger& log): logger(log) 
{
	
	if (!boost::filesystem::exists("uploads")) 
	{
		boost::filesystem::create_directory("uploads");
	}
	//homepage = "index.html";
	std::cout << "Listen Called\n\n";
	listen_connections(io, port);
	
}

TCP_Server::TCP_Server(boost::asio::io_context& io, USHORT port, Logger& log, std::string new_bad_string) : logger(log)
{

	listen_connections(io, port);

}
void TCP_Server::listen_connections(boost::asio::io_context& context, USHORT port)
{
	// listen for connection, when connected, creates thread and calls new connection
	std::cout << "Listening\n\n";
	try
	{
		tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), port));
		for (;;)
		{
			sock_ptr sock(new bsock(context));
			acceptor.accept(*sock);
			std::shared_ptr<Connection> con(new Connection(sock,logger));
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Error in Listen for connections: " << e.what() << std::endl;
	}


}


