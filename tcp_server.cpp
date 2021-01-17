#include "tcp_server.hpp"


tcp_server::tcp_server(boost::asio::io_context& io_service, short port, logger& logs_) : logs(logs_)
{
    listen(io_service, port); 
}


void tcp_server::conn(socket_ptr sock, int port)
{
    boost::posix_time::ptime date_time = boost::posix_time::second_clock::local_time();
    std::cout << date_time <<std::endl;
    std::cout << "Connected with Client:\n";
    logs << "client connected IP : " + sock->remote_endpoint().address().to_string();
    
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
        }/*
        else
        {
            sock->close();
        }
       */
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
    std::ifstream file_to_send(filepath, std::ios::binary);
    file_buffer.reserve(filesize);
    file_buffer.assign(std::istreambuf_iterator<char>(file_to_send),
    std::istreambuf_iterator<char>());
    file_to_send.close();
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
    logs << request;
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
        int file_sz = load_file_into_buffer("favicon.ico");
        header = headers.get_full_header("200 OK", "image/x-icon", file_sz);
        send_data(sock, header, file_buffer, file_sz);

    }
    else if (boost::filesystem::exists(request.substr(1,request.size()))) //check file exists and remove beginning /
    {
       
        int pos = request.find_last_of("/") + 1;
        std::string attachment = "attachment; filename=\"";
        attachment += request.substr(pos, request.size());

        std::string extension = boost::filesystem::extension(request);
        std::string mime = get_mime(extension);
       
        int file_sz = load_file_into_buffer(request.substr(1, request.size()));
        if(extension == ".html")
        {
            header = headers.get_full_header("200 OK", mime, file_sz);

        }
        else
        {
            header = headers.get_full_header("200 OK", mime, file_sz, attachment);
            std::cout << header << std::endl;
        }
        send_data(sock, header, file_buffer, file_sz);

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
    // TO DO 

}

bool tcp_server::check__unsafe_data(std::string unchecked_data)
{
    std::vector<std::string> bad_strings = {"../","cmd","/..","../../../","/etc/passwd"};
    for(int i = 0; i <bad_strings.size(); ++i)
    {
        int found =  unchecked_data.find("cmd");
        if (found == -1){
            return true;

        }
       
    }
    std::cout << "Bad data found\n"; // debug
    return false;

}



std::string tcp_server::get_mime(std::string extension)
{
    std::string mime = MIME.get_mime(extension);
    return mime;
}
