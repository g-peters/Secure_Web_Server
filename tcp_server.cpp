#include "tcp_server.hpp"


tcp_server::tcp_server(boost::asio::io_context& io_service, short port)
{
    listen(io_service, port); 
    http_headers headers; 
    mime_types MIME;
    
}

void tcp_server::conn(socket_ptr sock, int port)
{
    boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
    std::cout << date_time <<std::endl;

    std::cout << "Connected with Client:\n";
    try
    {
        std::vector<char> buff(1024);
        boost::system::error_code error;
        size_t length = sock->read_some(boost::asio::buffer(buff), error);
        std::string data = parse_data(buff);
        bool safe = check__unsafe_data(data);
        if (safe)
        {
            check_method(parse_data(buff), sock);
        }
        else
        {

            sock->close();
        }
       
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void tcp_server::listen(boost::asio::io_context& io_service, short port)
{
    try
    {
        tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        for (;;)
        {
            socket_ptr sock(new bsock(io_service));
            std::cout << "Server Listening for Connections:\n";
            acceptor.accept(*sock);
            boost::thread t(boost::bind(&tcp_server::conn, this, sock, port));


        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in tcp_server: " << e.what() << "\n";
    }
}

std::string tcp_server::parse_data(std::vector<char>& data)
{
    std::string client_data;
    // lambda to parse vector and put it into a string
    std::for_each(data.begin(), data.end(), [&](char i) {client_data.push_back(i); });
    // look_for_invalid chars(); <- need to make
    std::cout << client_data;
    return client_data;
}

int tcp_server::check_method(std::string s, socket_ptr sock)
{
    
    int carrage_loc;
    carrage_loc = s.find("\r\n");
    std::string request = s.substr(0, carrage_loc); //get first line of inout
    std::istringstream request_line(request);
    std::string method, request_target, protocol;
    request_line >> method;
    request_line >> request_target;
    request_line >> protocol;

    if (method == "GET") {
        std::cout << "process get called\n";
        process_get(sock, request_target);
        return 0;
    }
    else if (method == "POST") {
        process_post(sock,request_target);
        return 1;
    }
    else return -1;
    
}

int tcp_server::load_file_into_buffer(std::string filepath)
{  

    boost::filesystem::path p{filepath};
    boost::system::error_code ec;
    int filesize = file_size(p, ec);
    std::ifstream testFile(filepath, std::ios::binary);
    file_buffer.reserve(filesize);
    file_buffer.assign(std::istreambuf_iterator<char>(testFile),
    std::istreambuf_iterator<char>());
    testFile.close();
    return filesize;
}

void tcp_server::send_data(socket_ptr sock, std::string header, std::string body)
{
    boost::asio::write(*sock, boost::asio::buffer(header, header.size()));
    boost::asio::write(*sock, boost::asio::buffer(body, body.size()));
}

void tcp_server::send_data(socket_ptr sock, std::string header, std::vector<char> body, int size)
{
    boost::asio::write(*sock, boost::asio::buffer(header, header.size()));
    boost::asio::write(*sock, boost::asio::buffer(body, size));
}

void tcp_server::process_get(socket_ptr sock, std::string request)
{
    int body_size = headers.get_homepage().size();
    header = headers.get_full_header("200 OK", "text/html", body_size);
    if (request == "/")
    {
        header_to_send = header;
        data_to_send = headers.get_homepage();
        send_data(sock, header, data_to_send);
    }
    else if (request == "/favicon.ico")
    {
        int fl_sz = load_file_into_buffer("favicon.ico");
        header = headers.get_full_header("200 OK", "image/x-icon", fl_sz);
        send_data(sock, header, file_buffer, fl_sz);

    }
    else if (boost::filesystem::exists(request.substr(1,request.size()))) //check file exists and remove beginning /
    {
        std::cout << "file exists\n";
        std::string attachment = "attachment; filename=";
        attachment += request.substr(1, request.size());
        std::string extension = boost::filesystem::extension(request);
        std::string mime = get_mime(extension);
       
        int fl_sz = load_file_into_buffer(request.substr(1, request.size()));
        header = headers.get_full_header("200 OK", mime, fl_sz, attachment);
        send_data(sock, header, file_buffer, fl_sz);

    }
    else {
        data_to_send = headers.get_404();
        header = headers.get_full_header("404 OK", "text/html", data_to_send.size());
        send_data(sock, header, data_to_send);
    }
}

void tcp_server::process_post(socket_ptr sock, std::string request)
{
    std::cout << "Process Post called:\n";

}

bool tcp_server::check__unsafe_data(std::string unchecked_data)
{
    std::cout << "checking for bad strings\n";
    std::vector<std::string> bad_strings = {"../","cmd","/..","../../../"};
    for(int i = 0; i <bad_strings.size(); ++i)
    {
        int found =  unchecked_data.find("cmd");
        if (found == -1){
            std::cout << "found int " << found <<std::endl;
            return true;

        }
       
    }
    std::cout << "Bad data found\n";
    return false;

}



std::string tcp_server::get_mime(std::string extension)
{
    std::string mime = MIME.get_mime(extension);
    return mime;
}
