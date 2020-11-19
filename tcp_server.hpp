#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "connection.hpp"
using boost::asio::ip::tcp;
typedef boost::asio::ip::tcp::socket bsock;
typedef boost::shared_ptr<bsock> socket_ptr;
class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_service, short port) { listen(io_service, port); }
    void conn(socket_ptr sock);
    void listen(boost::asio::io_context& io_service, short port);
    std::string parse_data(std::vector<char>& data);
    void act_on_data(std::string s, socket_ptr sock);

};

