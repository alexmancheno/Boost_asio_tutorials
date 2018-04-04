#include <iostream>
#include "tcp_server"

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp_server server1(io_context);
        udp_server server2(io_context);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}