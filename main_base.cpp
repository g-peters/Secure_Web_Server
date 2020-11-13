//
// TSS INTERNAL USE ONLY

#include <iostream>

#include "server_base.hpp"


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
