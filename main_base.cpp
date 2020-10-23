//
// TSS INTERNAL USE ONLY
//
// BERTY: {
//   You probably don't have to alter this 'main' file much or at all.
//   The code to start programming on is server_base{.hpp,.cpp} and the tcp_server
//   class I have outline for you.
//   Please free to remove all my comments - It's in your hands now; it's your code! }

#include <iostream>

#include "server_base.hpp"

/*! \brief Main
 *  
 *  Requires one argument at the command line; the port number.
 */
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      return 1;
    }

    int port_number = atoi(argv[1]);

    server_start(port_number);   // Server starts here - see server_base code
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception at main(): " << e.what() << "\n";
  }

  return 0;
}
