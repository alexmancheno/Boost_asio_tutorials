/*
This tutorial shows how to use asio to implement a server application with TCP
*/
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/*
We define the function make_daytime_string() to create the string to be sent back to the
client. This function will be reused in all of the daytime server tutorials.
*/

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try
    {
        boost ::asio::io_service io_service;
        /*
        A ip::tcp::acceptor object needs to be created to listen for new connections. It
        is initialised on a TCP port 13, for IP version 4.
        */
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
        /*
        This is an iterative server, which means that it will handle one connection at a
        time. Create a socket that will represent the connection to the client, and then
        wait for a connection.
        */
        while(true)
        {
            std::cout << "Creating socket .. \n";
            tcp::socket socket(io_service);
            std::cout << "Waiting to receive connection ..\n";
            acceptor.accept(socket);
            std::cout << "Connection received!\n";

            /*
            A client is accessing our service. Determine the current time and transfer
            this information to the client.
            */
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}