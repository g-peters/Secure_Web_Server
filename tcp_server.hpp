#pragma once
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <string>
#include "connection.hpp"
#include "http_headers.hpp"
#include "mime_types.hpp"
#include "logger.hpp"
using boost::asio::ip::tcp;
typedef boost::asio::ip::tcp::socket bsock;
typedef boost::shared_ptr<bsock> socket_ptr;

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_service, short port, logger& log);
    void conn(socket_ptr sock, int port);
    void listen(boost::asio::io_context& io_service, short port);
    std::string parse_data(std::vector<char>& data);
    int check_method(std::string s, socket_ptr sock);
    int load_file_into_buffer(std::string);
    void send_data(socket_ptr, std::string, std::string);
    void send_data(socket_ptr, std::string, std::vector<char>, int);
    void process_get(socket_ptr, std::string);
    void process_post(socket_ptr,std::string );
    bool check__unsafe_data(std::string);
    std::string get_mime(std::string);
    mime_types MIME;


private:
    std::string header_to_send;
    std::string data_to_send;
    std::vector<std::string> not_allowed_requests;
    std::vector<char> file_buffer;
    std::string header;
    http_headers headers;
    logger& logs;
};

