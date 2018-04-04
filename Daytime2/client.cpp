#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    /*
    The purpose of this application is to access a daytime service, so we need the user to
    specify the server 
    */
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>\n";
            return 1;
        }

        // All programs need to have at least one io_service object.
        boost::asio::io_service io_service;

        /*
        We need to turn the server name that was specified as a parameter to the
        application, into a TCP endpoint. To do this we use a ip::tcp::resolver object.
        */
        tcp::resolver resolver(io_service);

        /*
        A resolver takes a query object and turns it into a list of endpoints. We
        construct a query using the name of the server, specified in argv[1], and the name
        of the service, in this case "daytime".
        */
        tcp::resolver::query query(argv[1], "daytime");

        /*
        The list of endpoints is returned using an iterator of type
        ip::tcp::resolver::iterator (note that a default constructed iterator object can
        be used as an end iterator).
        */
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        /*
        Now we create and connect the socket. The list of endpoints obtained above may
        contain both IPv4 and IPv6 endpoints, so we need to try each of them until we find
        one that works. This keeps the client program indepdendent of a specific IP
        version. The boost::asio::connect() function does this for us automatically.
        */
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        /*
        The connection is open. All we need to do now is read the response from the
        daytime service.

        We use a boost::array to hold the received data. The boost::asio::buffer()
        function automatically determines the size of the array to help prevent buffer
        overruns. Instead of a boost::array, we could have used a char[] or std::vector.
        */
        while (true)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
        /*
        When the server closes the connection, the ip::tcp::socket::read_some() function
        will exit with the boost:asio::error:eof error, which is how we know to exit the
        loop.
        */
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer
            else if (error)
                throw boost::system::system_error(error); // Some other error
            
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}