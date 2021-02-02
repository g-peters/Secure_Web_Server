#include "Connection.hpp"
const std::string Connection::HOME_PAGE = "index.html";
const std::string Connection::UPLOADS = "uploads";
std::vector<std::string> non_allowed_string = { "cmd","../","&&","uploads" };

Connection::Connection(sock_ptr& s, Logger& log) : sock(std::move(s)), logger(log) 
{
	buff.resize(1024);
	start();
}

void Connection::start()
{
	std::thread thr([=] {new_connection(); });
	thr.join();
}
/*
void Connection::receive()
{
	std::thread::id this_id = std::this_thread::get_id();
	std::cout << "thread " << this_id << "\n";

	int bytes = sock->read_some(boost::asio::buffer(buff));
	std::for_each(buff.begin(), buff.end(), [&](char i) {std::cout << i; });


	sock->close();
	delete this;
}
*/

void Connection::add_non_allowed_string(std::string bad_string)
{
	non_allowed_strings.push_back(bad_string);
}



void Connection::new_connection()
{
	//std::thread::id this_id = std::this_thread::get_id(); // testing purposes
	//std::cout << "thread " << this_id << "\n"; // testing purposes
	std::vector<char> buff(1024 * 10);
	boost::system::error_code error;
	int size = sock->read_some(boost::asio::buffer(buff), error);
	buff.resize(size);
	if(check_data_unsafe(parse_data(buff)))
	{
		//log << "Received unacceptable data";
		sock->close();
	}
	else
	{
	check_method(parse_data(buff)); // passes socket and string returned from parse data to check _method
	}

}

std::string Connection::get_date_time() // http://boost.sourceforge.net/regression-logs/cs-win32_metacomm/doc/html/date_time.posix_time.html
{
	boost::posix_time::ptime d_t =
	boost::posix_time::second_clock::local_time();
	std::string date_time = boost::posix_time::to_simple_string(d_t);


	return date_time;
}

std::string Connection::parse_data(std::vector<char>& buff)
{
	std::string data;
	// lambda to convert char vector into string
	std::for_each(buff.begin(), buff.end(), [&](char i) {data.push_back(i); });
	std::cout << data << std::endl;
	return data;
}

bool Connection::check_data_unsafe(std::string unchecked_data)
{
	for (int i = 0; i < non_allowed_string.size(); ++i) {
		int found = unchecked_data.find(non_allowed_string[i]);
		if (found != std::string::npos) {
			return true;
		}
		else return false;
	}
	return false;
}


// overloaded functions for sending data
void Connection::send_data(std::string header, std::string body, int size)
{
	boost::asio::write(*sock, boost::asio::buffer(header, header.size()));
	boost::asio::write(*sock, boost::asio::buffer(body, body.size()));
}

void Connection::send_data(std::string header, std::vector<char> body, int size)
{
	boost::asio::write(*sock, boost::asio::buffer(header, header.size()));
	boost::asio::write(*sock, boost::asio::buffer(body, body.size()));

}


// process post requests
void Connection::process_post( std::string request)
{
	//std::thread::id this_id = std::this_thread::get_id(); // testing purposes
	//std::cout << "thread " << this_id << "\n"; // testing purposes


	bool safe = check_data_unsafe(request);
	if (safe) {
		std::cout << "p:DATA IS NOT SAFE\n";

	}
	else
	{
		std::cout << "p:DATA IS SAFE\n";
	}
	HTTP_Headers headers;
	if (boost::filesystem::exists(UPLOADS)) {
		std::string uploadpath = "uploads/testfile.txt";
		USHORT end_of_header = request.find_last_of("\r\n");
		std::string post_data = request.substr(end_of_header, request.size());
		std::ofstream upload_file(uploadpath, std::ios::app);
		upload_file << post_data << std::endl;
		upload_file.close();
	}
	else
	{
		std::cout << "Error Upload dir does not exist\n";
	}
	std::string data = headers.get_201();
	std::string header = headers.get_full_header("201 OK", "text/html", data.size());
	send_data(header, data, data.size());
}

void Connection::process_get(std::string request)
{
	bool safe = check_data_unsafe(request);
	// if data not safe is true, close connection to socket
	if (safe) 
	{
		sock->close();
	}

	HTTP_Headers headers;
	std::string header;
	std::string body = "";
	logger << request;
	// homepage default
	if (request == "/")
	{
		if (boost::filesystem::exists(HOME_PAGE)) // incase file does not exist
		{
			buff = load_file_into_buffer(HOME_PAGE);
			header = headers.get_full_header("200 OK", MIME.get_mime(HOME_PAGE), buff.size());
			send_data(header, buff, buff.size());
		}
		else // send html from program
		{
			int body_size = headers.get_homepage().size();
			body = headers.get_homepage();
			header = headers.get_full_header("200 OK", "text/html", body.size());
			send_data(header, body, body.size());
		}
	}
	// check if request is a path and if it exists
	else if (boost::filesystem::exists(request.substr(1, request.size())))
	{
		std::cout << "File exists: " << request << std::endl;
		int pos = request.find_last_of("/") + 1;
		std::string attachment = "attachment; filename=\"";
		std::string attach2 = request.substr(pos, request.size());
		//attachment += request.substr(pos, request.size()); // filename
		std::string mime = MIME.get_mime(request);
		buff = load_file_into_buffer(request.substr(1, request.size()));
		if (boost::filesystem::extension(request) == ".html")
		{
			header = headers.get_full_header("200 OK", mime, buff.size()); // to be displayed
		}
		else
		{
			header = headers.get_full_header("200 OK", mime, buff.size(), attachment); // to be downloaded
		}
		send_data(header, buff, buff.size());
	}
	// 404 NOT FOUND
	else
	{
		body = headers.get_404();
		header = headers.get_full_header("404 OK", "text/html", body.size());
		send_data(header, body, body.size());
	}
}

// loads file (index.html/jpg's/txt files) into buffer for sending
std::vector<char> Connection::load_file_into_buffer(std::string filepath)
{
	std::vector<char> filebuffer;
	boost::filesystem::path path_(filepath);
	boost::system::error_code error;
	int filesize = file_size(path_, error);
	std::ifstream file_to_send(filepath, std::ios::binary);
	filebuffer.resize(filesize);
	filebuffer.assign(std::istreambuf_iterator<char>(file_to_send),
		std::istreambuf_iterator<char>());
	file_to_send.close();
	return filebuffer;
}

// check whether post/get request
void Connection::check_method(std::string data)
{
	bool safe = check_data_unsafe(data);
	if (safe) {
		std::cout << "m:DATA NOT SAFE\n";

	}
	else
	{
		std::cout << "m:DATA IS SAFE\n";
	}
	USHORT EoL1 = data.find("\r\n"); // end of line 1 (EoL1)
	std::string first_line = data.substr(0, EoL1);
	std::istringstream split_line(first_line);
	std::string method, target;
	split_line >> method; // gets GET or POST
	split_line >> target; // gets Request path
	if (method == "GET")
	{
		
			process_get(target);
		
	}
	else if (method == "POST")
	{
		// creates new thread for post request for further isolation
		std::thread thr([=] {process_post(data); });
		thr.join();
	
	}
	else
	{
		// Send response 405 ?
	}
}