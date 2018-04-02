#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class worker
{
    public:
        worker(boost::asio::io_service &io)
        :   strand_(io),
            timer1_(io, boost::posix_time::seconds(1)),
            timer2_(io, boost::posix_time::seconds(1)),
            count_(0)
        {
            timer1_.async_wait(strand_.wrap(boost::bind(&worker::f1, this)));
            timer2_.async_wait(strand_.wrap(boost::bind(&worker::f2, this)));
        }

        ~worker()
        {
            std::cout << "Finished, count is: " << count_ << std::endl;
        }

        void f1()
        {
            if (count_ < 5)
            {
                std::cout << "f1 reporting! Count is " << count_++ << std::endl;
                timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
                timer1_.async_wait(strand_.wrap(boost::bind(&worker::f1, this)));
            }
        }

        void f2()
        {
            if (count_ < 7)
            {
                std::cout << "f2 reporting! Count is " << count_++ << std::endl;
                timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
                timer2_.async_wait(strand_.wrap(boost::bind(&worker::f2, this)));
            }
        }

        private:
            boost::asio::io_service::strand strand_;
            boost::asio::deadline_timer timer1_;
            boost::asio::deadline_timer timer2_;
            int count_;
};

int main()
{
    boost::asio::io_service io;
    worker w(io);
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io));

    io.run();
    t.join();
}