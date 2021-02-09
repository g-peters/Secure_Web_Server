#pragma once
#include <string>

class HTTP_Headers {

private:
	std::string http_header_1;
	std::string http_header_2;
	std::string http_header_3;
	std::string http_header_4;
	std::string http_header_5;
	std::string html_homepage;
	std::string html_404;
	std::string html_400;
	std::string html_201;
	std::string html_405;
public:
	// standard http header
	std::string get_full_header(std::string, std::string, int);
	// function overload for file returning where disposition is needed in http header
	std::string get_full_header(std::string, std::string, int, std::string);
	std::string get_just_header(std::string status, std::string content);
	std::string get_homepage();
	std::string get_404();
	std::string get_400();
	std::string get_201();
	std::string get_405();

	HTTP_Headers();

};
