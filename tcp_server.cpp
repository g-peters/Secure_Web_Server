//
// TSS INTERNAL USE ONLY
//

//#include <whatever>

#include "tcp_server.hpp"


/*! \brief Constructor
 *
 *  Set up resources for tcp_server
 */
tcp_server::tcp_server(int port)
{
	// BERTY: { Create the 'socket' or 'boost::asio::io_context' etc. here
	// or throw an error that it failed to be created. }
}

/*! \brief Destructor
 *
 *  Safely close/free tcp_server resources
 */
tcp_server::~tcp_server()
{
	// BERTY: { Clean up your resources here. }
}

/*! \brief
 *
 *  Detail of func here.
 */
int tcp_server::await_connection()
{
	// BERTY: { I only guess you'll need this!?  Not sure what type it'll return.
	//   Depends on if you use sockets, boost.asio, or something else. }
}


// BERTY: { Implement your other functions on the tcp_server class here... }

