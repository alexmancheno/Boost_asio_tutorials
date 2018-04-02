//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code&, boost::asio::deadline_timer *t, int *count)
{
    if (*count < 3)
    {
        std::cout << "print: " << *count << std::endl;
        ++(*count);

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
    else
    {
        std::cout << "Finishing print()!\n";
    }
}

void sayHello(const boost::system::error_code&, boost::asio::deadline_timer* t, int *count)
{
    if (*count < 5)
    {
        std::cout << "sayHello:" << *count << std::endl;
        ++(*count);

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
        t->async_wait(boost::bind(sayHello, boost::asio::placeholders::error, t, count));
    }
    else
    {
        std::cout << "Finished sayHello()!\n";
    }
}

int main()
{
    boost::asio::io_service io;

    int count1 = 0, count2 = 0;
    
    boost::asio::deadline_timer f(io, boost::posix_time::seconds(1));
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

    f.async_wait(boost::bind(sayHello, boost::asio::placeholders::error, &f, &count2));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count1));

    io.run();

    std::cout << "Final count1 is " << count1 << std::endl;
    std::cout << "Final count2 is " << count2 << std::endl;

    return 0;
}