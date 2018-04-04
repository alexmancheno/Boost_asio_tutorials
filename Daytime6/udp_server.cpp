
#include "udp_server.h"

// Not to be used!
udp_server::udp_server()
    :   socket_(io_)
{
    // Do nothing
}

// Constructor
udp_server::udp_server(boost::asio::io_context& io_context)
    :   socket_(io_context, udp::endpoint(udp::v4(), 13))
{
    start_receive();
}

void udp_server::start_receive()
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this, boost::asio::placeholders::error)
    );
}

void udp_server::handle_receive(const boost::system::error_code& error)
{
    if (!error || error == boost::asio::error::message_size)
    {
        boost::shared_ptr<std::string> message(new std::string(make_daytime_string()));

        socket_.async_send_to(
            boost::asio::buffer(*message),
            remote_endpoint_,
            boost::bind(&udp_server::handle_send, this, message)
        );

        start_receive();
    }
}

void udp_server::handle_send(boost::shared_ptr<std::string> /*message*/)
{
    // Do nothing
}

std::string udp_server::make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}