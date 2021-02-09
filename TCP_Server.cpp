#include "TCP_Server.hpp"


//std::vector<std::string> TCP_Server::non_allowed_strings = { "cmd","../","&&","uploads" };



TCP_Server::TCP_Server(boost::asio::io_context& io, USHORT port, Logger& log, Logger& err): logger(log), err_log(err)
{

	// on server start, checks if uploads directory exists, if not creates it
	if (!boost::filesystem::exists("uploads")) 
	{
		try
		{
			boost::filesystem::create_directory("uploads");
		}
		catch(std::exception e)
		{
			err_log << e.what();
		}
	}
	// listen for new connections
	listen_connections(io, port);
	
}

// listen for connection, when connected, creates new connection object to handle that connection

void TCP_Server::listen_connections(boost::asio::io_context& context, USHORT port)
{
	try
	{
		tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), port));
		for (;;)
		{
			sock_ptr sock(new bsock(context));
			acceptor.accept(*sock);
			//log << 
			// client connects, new Connection object created, utilizing shared pointer for resource cleanup
			std::shared_ptr<Connection> con(new Connection(sock,logger));
		}
	}
	catch (std::exception& e)
	{
		err_log << e.what();
		std::cout << "Error in Listen for connections: " << e.what() << std::endl;
	}


}


