#include "logger.hpp"


logger::~logger()
{
    log_file.close();
}

logger::logger()
{
    try
    {
        log_file.open("error_log.txt", std::ios::app);
    }
    catch (std::exception e) {
        std::cout << "error opening file: \n";
    }
}



 logger& operator<<(logger& log,  std::string text)
 {
    std::mutex file_lock;
    std::lock_guard<std::mutex> file_locking(file_lock);
    std::cout << "Mutex Locked!\n";
    if (!log.log_file.is_open()) {
        try
        {
            log.log_file.open("error_log.txt", std::ios::ate | std::ios::app);
        }
        catch (std::exception e) {
             std::cout << e.what();
        }
    }
    boost::posix_time::ptime date_time =
    boost::posix_time::second_clock::local_time();
    std::string datetime = boost::posix_time::to_simple_string(date_time);
    std::string to_log = datetime + " " + text;
    log.log_file << to_log <<std::endl;

    return log;
}

