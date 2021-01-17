#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "logger.hpp"

void server_start(int port_number, boost::asio::io_context& io_context, logger& logs);