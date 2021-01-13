# 2020-21

Secure Web Server Assessment

# Compilation
`g++  main_base.cpp server_base.cpp tcp_server.cpp  -o srv`

If using `<thread>` library, then compile with:
`g++ -pthread ...`

compilation 
g++ -pthread *.cpp -o serv -lboost_thread  -lboost_filesystem

