#include <iostream>

#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    private:
        tcp::socket socket_;
        char buf_[1024];    
    public:
        explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}
        void start() {
            do_read();
        }
    private:
        void do_read() {
            auto self(shared_from_this());

            socket_.async_read_some(
                asio::buffer(buf_),
                [this, self](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        do_write(length);
                    } else {
                        std::cout << "session closed : " << ec.message() << std::endl;
                    }
                } 
            );
            
        }
        void do_write(std::size_t lenght) {
            auto self(shared_from_this());

            asio::async_write(socket_, 
                asio::buffer(buf_),
                [this, self](const boost::system::error_code& ec, std::size_t) {
                    if (!ec) {
                        do_read();
                    }
                }
            );
        }
};