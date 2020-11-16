#include <iostream>
#include "server_base.hpp"
#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    
        try
        {
            if (argc != 2)
            {
                std::cerr << "Error: No port number provided\n";
                return 1;
            }

            boost::asio::io_context context;  //io_service  - https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/io_service.html
            server_start(std::atoi(argv[1]),context);
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return 0;
    }