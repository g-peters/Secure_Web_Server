
#include <iostream>
#include <boost/asio.hpp>
#include "tcp_server.hpp"
#include "server_base.hpp"
#include "logger.hpp"
#include <memory>
int main(int argc, char* argv[])
{
    logger logs;
    std::string abc = "server started\n";
    try
    {    
        
        if (argc != 2)
        {
            std::cerr << "Error No Port Number Provided! \n";
            return 1;
        }
        
        std::string port_(argv[1]);

        boost::asio::io_context io_context;
        std::string start = "Server started on port : "  + port_;
        logs << start;
        server_start(std::atoi(argv[1]), io_context, logs);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}