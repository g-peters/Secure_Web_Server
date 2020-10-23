//
// TSS INTERNAL USE ONLY
//

#include <iostream>

#include "tcp_server.hpp"

// -----------------------------------------------------------------------------

const int max_length = 1024;

/*! \brief Is called on client connecting
 *
 *  Handles each tcp connection 
 */
void conn_session(int connection)
{   // BERTY: { Actual type this takes may depends on if you use sockets, boost.asio,
    //    or something else. }
  try
  {
    char request_data[max_length];

    // BERTY: {
    //   Read some data from the tcp connection into the data array above.
    //   Check for errors at every chance, and handle them in a sane way
    //    depending on the error!
    //
    //   Parse the raw data into some organisaed way, perhaps a std::string or even
    //    bunch of strings.
    //   Move all these steps into separate functions as I don't want to see spaghetti
    //    code.
    //   
    //   Maybe the connection could be handle be a separate class, with all the functions
    //    inside it, instead of calling this conn_session function.
    // }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread conn_session(): " << e.what() << "\n";
  }
}

// -----------------------------------------------------------------------------

/*! \brief Enters from main() here
 *  
 *  First function of server. Sets up the 
 */
void server_start(int port_number)
{

  // BERTY: { Create an instance of your server... }
  tcp_server server(port_number);

  // BERTY: {
  //   The core of your server.  You could have your main loop here waiting for
  //    connections, then launch the conn_session(...) in a thread each time a
  //    client connects, or you could move all the handling into the tcp_server
  //    or some other class. }
  //
}

