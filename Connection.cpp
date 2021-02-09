#include "Connection.hpp"

const std::string Connection::HOME_PAGE = "index.html";
const std::string Connection::UPLOADS = "uploads";
std::vector<std::string> non_allowed_string = { "cmd","../","&&","uploads" };

// code adapted from (boost, n.d.)

Connection::Connection(sock_ptr& s, Logger& log) : sock(std::move(s)), logger(log)
{
	buff.resize(1024);
	// upon created, calls start
	start();
}
// destructor
Connection::~Connection()
{

}
void Connection::start()
{   // create thread to handle connection
	std::thread thr([=] {new_connection(); });
	thr.join();
}



void Connection::new_connection()
{
	//std::thread::id this_id = std::this_thread::get_id(); // testing purposes
	//std::cout << "thread " << this_id << "\n"; // testing purposes
	boost::system::error_code error;
	int size = sock->read_some(boost::asio::buffer(buff), error);
	buff.resize(size);
	// check data doesn't contain unnacceptable strings
	if(check_data_unsafe(parse_data(buff)))
	{
		sock->close();
	}
	else
	{
		// if data is safe, checks for http method
	check_method(parse_data(buff)); // passes socket and string returned from parse data to check _method
	}

}
// changes buffer data from char to string
std::string Connection::parse_data(std::vector<char>& buff)
{
	std::string data;
	// lambda to convert char vector into string
	std::for_each(buff.begin(), buff.end(), [&](char i) {data.push_back(i); });
	std::cout << data << std::endl;
	return data;
}

// compares received data to vector of bad strings, if true, connection closes
bool Connection::check_data_unsafe(std::string unchecked_data)
{ 
	// default chooses safer option of data being unsafe to close connection
	bool bad_data_found = true;
	std::for_each(non_allowed_string.begin(),non_allowed_string.end(), [&](std::string i){

		if (unchecked_data.find(i) != std::string::npos) // code adapted from
		{
			//std::cout << "Bad string found!!!!!!!!" << i << std::endl;
			bad_data_found = true;
			sock->close();
		}
		else
		{
			bad_data_found = false;
		}
	});
	return bad_data_found;

}


// overloaded functions for sending data
void Connection::send_data(std::string header, std::string body, int size)
{
	boost::asio::write(*sock, boost::asio::buffer(header, header.size()));
	boost::asio::write(*sock, boost::asio::buffer(body, body.size()));
}
// sends contents of buffer (sending files)
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
	// checking data is safe again/ defence in depth
	if (check_data_unsafe(request))
	{
		sock->close();
	}
	else
	{
	
		// all post requests stored in uploads directory, inaccessable from client
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
}

void Connection::process_get(std::string request)
{
	//bool safe = check_data_unsafe(request);
	// if data not safe is true, close connection to socket
	if (check_data_unsafe(request)) 
	{
		sock->close();
	}
	

	//HTTP_Headers headers;
	std::string header;
	std::string body = "";
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
		int pos = request.find_last_of("/") + 1;
		std::string attachment = "attachment; filename=\"";
		std::string attach2 = request.substr(pos, request.size());
		//attachment += request.substr(pos, request.size()); // filename
		std::string mime = MIME.get_mime(request);
		buff = load_file_into_buffer(request.substr(1, request.size()));
		// checks to see if request is displayable in a browser (.html)
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
		//attachment += request.substr(pos, request.size()); // filename


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
	// check_data_unsafe returns true if unsafe
	if (check_data_unsafe(data)) {
		sock->close();
	}
	else
	{
	
		USHORT EoL1 = data.find("\r\n"); // end of line 1 (EoL1)
		std::string first_line = data.substr(0, EoL1);
		// logs client request
		logger << first_line;

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
			// send 405, method not acceptable
			std::string header;
			std::string body = "";
			body = headers.get_405();
			header = headers.get_full_header("405 OK", "text/html", body.size());
			send_data(header, body, body.size());
		}
	}
}