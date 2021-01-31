#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include "HTTP_Headers.hpp"
#include "Logger.hpp"
#include "MIME_Types.hpp"
using boost::asio::ip::tcp;
typedef boost::asio::ip::tcp::socket bsock;
typedef std::unique_ptr<bsock> sock_ptr;
typedef unsigned short int USHORT;



class Connection
{
public:
	Connection(sock_ptr& s, Logger&);
	
	
	void start();	
	void receive();
	
private:
	sock_ptr sock;
	std::vector<char> buff;
	std::vector<std::string> non_allowed_strings;
	void new_connection(); // New connection called from listen in a new thread
	std::string get_date_time();
	std::string parse_data(std::vector<char>&); // convert from char vec to std::string
	bool check_data_unsafe(std::string); // check if data contains non allowed characters
	void send_data( std::string, std::string, int);
	void send_data(std::string, std::vector<char>, int);
	void process_post(std::string);
	void process_get(std::string);
	void add_non_allowed_string(std::string);
	std::vector<char> load_file_into_buffer(std::string);
	void check_method(std::string);
	Logger& logger;
	MIME_Types MIME; // to be used by all threads
	// HTTP_Headers headers; will need new instance for each thread to stop data being edited
	std::vector<char> buffer;
	std::string header_to_send;
	std::string body_to_send;
	static const std::string HOME_PAGE;
	static const std::string UPLOADS;

};
