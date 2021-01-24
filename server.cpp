
#include <iostream>
#include <boost/asio.hpp>
#include "tcp_server.hpp"
#include "server_base.hpp"
#include "logger.hpp"
#include <memory>
#include <unistd.h>
#include <filesystem>

std::string get_cwd(){
    char buff[128];
    getcwd(buff,128);
    std::string directory(buff);
    return directory;

}
int main(int argc, char* argv[])
{
    std::string cwd = get_cwd();
    int success = chroot(cwd.c_str());
    if(success)
    {
        std::cout << "CHROOT SUCCESSFUL\n";
    }
    
    else
    {
        std::cout << "CHROOT UNSUCCESSFUL\n";
    }
    logger logs;
    std::string abc = "server started\n";
    logs << abc;
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