
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

/*
This tutorial program shows how to use asio to implement a client application with UDP.
*/
int main(int argc, char** argv)
{
    /*
    The start of the application is essentially the same as the TCP daytime client.
    */
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>\n";
            return 1;    
        }

        boost::asio::io_context io_context;

        /*
        We use an ip::udp::resolver object to find the correct remote endpoint to use
        based on the host and service names. The query is resitrected to return only IPv4
        endpoints by the ip::udp::v4() argument.
        */
        udp::resolver resolver(io_context);
        udp::resolver::query query(udp::v4(), argv[1], "daytime");

        /*
        The ip::udp::resolver::resolve() function is guaranteed to return at least one
        endpoint in the list if it does not fail. This means it is safe to dereference the
        return value directly.
        */
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        /*
        Since UDP is datagram-oriented, we will not be using a stream socket. Create an
        ip::udp::socket and initiate contact with the remote endpoint.
        */
        udp::socket socket(io_context);
        socket.open(udp::v4());

        boost::array<char, 1> send_buf = {{ 0 }};
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        /*
        Now, we need to be ready to accept whenever the server sends back to us. The
        endpoint on our side that receives the server's response will be initialised by
        ip::udp::socket::receive_from().
        */
        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}