#include "HTTP_Headers.hpp"
#include <string>

std::string HTTP_Headers::get_full_header(std::string status, std::string content, int size, std::string disposition)
{
	// resetting headers to defaults
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: close ";
	http_header_5 = "Content-Disposition: ";

	std::string head = http_header_1 += status + "\r\n";
	head += http_header_2 += content += "\r\n";
	head += http_header_3 += std::to_string(size) += "\r\n";
	head += http_header_4 += "\r\n";
	head += http_header_5 += disposition += "\"\r\n\r\n";

	return head;
}
std::string HTTP_Headers::get_full_header(std::string status, std::string content, int size)
{
	// resetting headers to defaults
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: close ";

	std::string head = http_header_1 += status + "\r\n";
	head += http_header_2 += content += "\r\n";
	head += http_header_3 += std::to_string(size) += "\r\n";
	head += http_header_4 += "\r\n\r\n";

	return head;
}
std::string HTTP_Headers::get_just_header(std::string status, std::string content)
{
	// resetting headers to defaults
	http_header_1 = "HTTP/1.1 ";
	http_header_4 = "Connection: close ";

	std::string head = http_header_1 += status + "\r\n";
	head += http_header_4 += "\r\n\r\n";

	return head;
}

std::string HTTP_Headers::get_homepage()
{
	return html_homepage;
}

std::string HTTP_Headers::get_404()
{
	return html_404;
}

std::string HTTP_Headers::get_400()
{
	return html_400;
}

std::string HTTP_Headers::get_201()
{
	return html_201;
}
std::string HTTP_Headers::get_405()
{
	return html_405;
}
HTTP_Headers::HTTP_Headers()
{
	http_header_1 = "HTTP/1.1 ";
	http_header_2 = "Content-Type: ";
	http_header_3 = "Content-Length: ";
	http_header_4 = "Connection: close";

	html_404 =
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>"
		"404 NOT FOUND"
		"</title>"
		"<link rel = \"stylesheet\" href = \"stylesheet.css\" >"
		"</head>"
		"<body>"
		"<div id = \"main\">"
		"<h1>404 PAGE NOT FOUND</H1>"
		"<p>Hellooooo, We can't find what your looking for....</p>"
		"</div>"
		"</body>"
		"</html>";
	html_homepage =
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>"
		"Home"
		"</title>"
		"<link rel = \"stylesheet\" href = \"stylesheet.css\" >"
		"</head>"
		"<h1>Home</H1>"
		"<body>"
		"<div id = \"main\">"
		"<p>Welcome to the home page</p>"
		"</div>"
		"</body>"
		"</html>";
	html_201 =
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>"
		"Home"
		"</title>"
		"<link rel = \"stylesheet\" href = \"stylesheet.css\" >"
		"</head>"
		"<div id = \"main\">"
		"<h1>Success</H1>"
		"<body>"
		"<p>Resource Successfully uploaded to server</p>"
		"</div>"
		"</body>"
		"</html>";
	html_405 =
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<title>"
		"Home"
		"</title>"
		"<link rel = \"stylesheet\" href = \"stylesheet.css\" >"
		"</head>"
		"<div id = \"main\">"
		"<h1>405 Method not allowed</H1>"
		"<body>"
		"</div>"
		"</body>"
		"</html>";


}

