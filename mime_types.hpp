#include <vector>
#include <string>
#include <iostream>
class mime_types
{
public:
	std::vector<std::pair<std::string, std::string>> MIME;
	mime_types();
	std::string get_mime(std::string );
};