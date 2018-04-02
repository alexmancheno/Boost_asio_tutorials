
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

/*
This tutorial shows how to use asio to implement a server application with UDP
*/
int main()
{
    try
    {
        boost::asio::io_context io_context;
        /*
        Create an ip::udp::socket object to receive requests on UDP port 13
        */
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

        /*
        Wait for a client to initiate contact with us. The remote_endpoint object will be populated by ip::udp::socket::receive_from()
        */
        while (true)
        {
            boost::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            boost::system::error_code error;
            std::cout << "Waiting to receive connection.\n";
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);
            std::cout << "Received a connection from: " << remote_endpoint.address() << std::endl;
            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);
            
            /*
            Determine what we are going to send back to the client.
            */
            std::string message = make_daytime_string();

            /*
            Send the response to the remote_endpoint.
            */
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
        }
    }
    /*
    Finally, handle any exceptions.
    */
    catch (std::exception& e)
    {
        std::cerr << e.what() << std:: endl;
    }
}