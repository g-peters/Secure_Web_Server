//
// TSS INTERNAL USE ONLY
//
// BERTY: { Other C++ code files can include this header to create a tcp_server 
//    instance and access its public functions. }

//#include <whatever>


class tcp_server
{
public:
  tcp_server(int port_number);    // Constructor
  ~tcp_server();                  // Destructor

  int await_connection();
  // BERTY: { Put your other public functions here. }

private:

	// BERTY: {
	//   Maintain your tcp_server's private state here.
	//   Put your private functions here too that are only called from within
	//    this tcp_server. }
};

