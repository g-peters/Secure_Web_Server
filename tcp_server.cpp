#include "tcp_server.hpp"
#include "messages.hpp"

void tcp_server::conn(socket_ptr sock)
{
    try
    {
      
                
            std::vector<char> buff(1024);
            

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(buff), error);
           // std::for_each(buff.begin(), buff.end(), [](char i) {std::cout << i; }); //https://en.cppreference.com/w/cpp/algorithm/for_each
            parse_data(buff);
            std::cout << parse_data(buff);
            act_on_data(parse_data(buff),sock);

            

            sock->close();
        
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void tcp_server::listen(boost::asio::io_context& io_service, short port)
{
    try
    {
        tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        for (;;)
        {
            socket_ptr sock(new bsock(io_service));
    
            acceptor.accept(*sock);
            boost::thread t(boost::bind(&tcp_server::conn, this, sock));


        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in tcp_erver: " << e.what() << "\n";
    }
}

std::string tcp_server::parse_data(std::vector<char>& data)
{
    std::string string_;
    // lambda to parse vector and put it into a string
    std::for_each(data.begin(), data.end(), [&](char i) {string_.push_back(i); });

    return string_;
}

void tcp_server::act_on_data(std::string s, socket_ptr sock)
{
int carrage_loc;
carrage_loc = s.find("\r\n");
std::string amend = s.substr(0,carrage_loc);
std::cout << "ammended string = " << amend << std::endl;
    if(amend == "GET / HTTP/1.1")
        {
        boost::asio::write(*sock, boost::asio::buffer(header, strlen(header)));
        boost::asio::write(*sock, boost::asio::buffer(body, strlen(body)));
        }
    else if(amend == "GET /favicon.ico HTTP/1.1"){
        sock->close();
    }

}
