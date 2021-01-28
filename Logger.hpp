# pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <boost/date_time.hpp>
#include <mutex>

typedef std::unique_ptr< std::mutex> mutex_ptr;

class Logger
{
public:
	Logger(const Logger&) = delete;
	Logger& operator= (const Logger&) = delete;
	friend Logger& operator << (Logger& log, std::string text);
	Logger(mutex_ptr&);
	~Logger();

private:
	std::ofstream log_file;
	mutex_ptr lock;
};

