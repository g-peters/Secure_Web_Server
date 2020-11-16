#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class tcp_server
{
public:
    int port;
    tcp_server(boost::asio::io_context& io_context, int port_) {
        port = port_;
        listen(io_context, port);

    }
    ~tcp_server(){}

    void session(boost::asio::ip::tcp::socket sock);
    /*
    {
        std::cout << "session function is called\n";
        try
        {
            for (;;)
            {
                char data[1024];

                boost::system::error_code error;
                size_t length = sock.read_some(boost::asio::buffer(data), error);
                if (error == boost::asio::error::eof)
                    break;
                else if (error)
                    throw boost::system::system_error(error);
                std::cout << data;

                std::thread::id main_thread_id = std::this_thread::get_id();
                std::cout << "\n\nTHIS THREAD ID = " << main_thread_id << "\n\n";
                boost::asio::write(sock, boost::asio::buffer(data, length));
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception in thread: " << e.what() << "\n";
        }
    }
*/
    
    void listen(boost::asio::io_context& io_context, int port)
    {
        std::cout << "listen function is called\n";
        using boost::asio::ip::tcp;
        tcp::acceptor accept_connection(io_context, tcp::endpoint(tcp::v4(), port));
        for (;;)
        {
            std::thread(&tcp_server::session,this ,accept_connection.accept()).detach();
        }



    }

};

