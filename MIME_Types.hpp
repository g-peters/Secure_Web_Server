#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
class MIME_Types
{
public:
	std::vector<std::pair<std::string, std::string>> MIME;
	MIME_Types();
	std::string get_mime(std::string);
};