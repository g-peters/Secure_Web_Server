#include <fstream>
#include <string>
#include <boost/date_time.hpp>


class logger
{
public:
	logger(const logger&) = delete;
	logger& operator= (const logger&) = delete;
    logger();
    
    friend logger& operator << (logger& log, std::string text); 
    ~logger();
private:
	std::ofstream log_file;

};

