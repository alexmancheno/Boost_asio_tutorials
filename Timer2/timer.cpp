
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

void sayHello(const boost::system::error_code&)
{
  std::cout << "I'm saying hello!\n";
}

int main()
{
  boost::asio::io_service io;

  boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
  boost::asio::deadline_timer f(io, boost::posix_time::seconds(3));
  
  std::cout << "Hello!\n";

  t.async_wait(&print);
  f.async_wait(&sayHello);

  io.run();

  return 0;
}