#include "tcp_server.hpp"


void tcp_server::session(boost::asio::ip::tcp::socket sock) {
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
            sock.close();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}
