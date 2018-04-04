
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class printer
{
  public:
    printer(boost::asio::io_service &io) : 
        timer_(io, boost::posix_time::seconds(1)),
        count_(0)
    {
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print()
    {
        if (count_ < 5)
        {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }

  private:
    boost::asio::deadline_timer timer_;
    int count_;
};

class triangle
{
    public:
        triangle(boost::asio::io_service &io, int n) :
            timer_(io, boost::posix_time::seconds(1)),
            count_(0),
            size_(n),
            stars_("*")
        {
            timer_.async_wait(boost::bind(&triangle::printStars, this));
        }

    ~triangle()
    {
        std::cout << "Finished printing triangle!\n";
    }

    void printStars()
    {
        if (count_ < size_)
        {
            std::cout << stars_ << std::endl;

            ++count_;
            stars_ += "*";

            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&triangle::printStars, this));
        }
    }

    private:
        boost::asio::deadline_timer timer_;
        int count_, size_;
        std::string stars_;
};

int main()
{
    boost::asio::io_service io;
    printer p(io);

    triangle t(io, 5);

    io.run();

    return 0;
}