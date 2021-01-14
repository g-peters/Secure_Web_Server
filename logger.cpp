#include "logger.hpp"

logger& operator<<(logger& log, std::string text)
{
    boost::posix_time::ptime date_time =
        boost::posix_time::second_clock::local_time();

    std::string datetime = boost::posix_time::to_simple_string(date_time);
    log.log_file << date_time;
    log.log_file << " ";
    log.log_file << text;
    log.log_file << "\n";
    return log;
}

logger::~logger()
{
    log_file.close();
}

logger::logger()
{
    try
    {
        log_file.open("error_log.txt", std::ios::ate | std::ios::app);
    }
    catch (std::exception e) {
        std::cout << "error opening file: \n";
    }
}
