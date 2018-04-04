#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

class udp_server
{
public:
    udp_server(); // Not to be used
    udp_server(boost::asio::io_context& io_context);

private:
    void start_receive();

    void handle_receive(const boost::system::error_code &error);

    void handle_send(boost::shared_ptr<std::string> /*message*/);

    std::string make_daytime_string();

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recv_buffer_;

    // Not to be used
    boost::asio::io_context io_;
};

#endif