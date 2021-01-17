# pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <boost/date_time.hpp>


class logger
{
public:
	logger(const logger&) = delete;
	logger& operator= (const logger&) = delete;
	friend logger& operator << (logger& log, std::string text);
    logger();
    ~logger();
private:
	std::ofstream log_file;

};

