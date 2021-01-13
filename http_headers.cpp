#include "http_headers.hpp"




std::string http_headers::get_full_header(std::string status, std::string content, int size, std::string disposition)
{
	// resetting headers to defaults
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: keep-alive "; 
	http_header_5 = "Content-Disposition: ";

	std::string head = http_header_1 += status + "\r\n";
	head += http_header_2 += content += "\r\n";
	head += http_header_3 += std::to_string(size) += "\r\n";
	head += http_header_4 += "\r\n";
	head += http_header_5 += disposition += "\r\n\r\n";

	return head;
}
std::string http_headers::get_full_header(std::string status, std::string content, int size)
{
	// resetting headers to defaults
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: keep-alive ";
	http_header_5 = "Content-Disposition: ";

	std::string head = http_header_1 += status + "\r\n";
	head += http_header_2 += content += "\r\n";
	head += http_header_3 += std::to_string(size) += "\r\n";
	head += http_header_4 += "\r\n\r\n";
	
	return head;
}

std::string http_headers::get_homepage()
{
	return html_homepage;
}

std::string http_headers::get_404()
{
	return html_404;
}

std::string http_headers::get_400()
{
	return html_400;
}

http_headers::http_headers()
{
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: keep-alive ";

	html_404 = 
		"<html>"
		"<head>"
		"<title>"
		"404 NOT FOUND"
		"</title>"
		"</head>"
		"<h1>404 PAGE NOT FOUND</H1>"
		"<body>"
		"<p>Hellooooo, We can't find what your looking for....</p>"
		"</body>"
		"</html>";
	html_homepage = 
		"<html>"
		"<head>"
		"<title>"
		"Home"
		"</title>"
		"</head>"
		"<h1>Home</H1>"
		"<body>"
		"<p>Welcome to the home page</p>"
		"</body>"
		"</html>";
}
