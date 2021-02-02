#pragma once
#include<iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/smart_ptr.hpp>
#include "HTTP_Headers.hpp"
#include "Logger.hpp"
#include "MIME_Types.hpp"
#include "Connection.hpp"

using boost::asio::ip::tcp;
typedef unsigned short int USHORT;
typedef boost::asio::ip::tcp::socket bsock;
typedef std::unique_ptr<bsock> sock_ptr;

class TCP_Server
{

public:
	TCP_Server(boost::asio::io_context&, USHORT, Logger&, Logger&);


private:
	std::vector<Connection> vec_con;
	void listen_connections(boost::asio::io_context&, USHORT);
	Logger& logger;
	Logger& err_log;

};

