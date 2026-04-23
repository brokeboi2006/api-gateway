#pragma once

#include <boost/asio.hpp>
#include "session.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Server {
    public:
        Server(asio::io_context& io_context, short port) 
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
            do_accept();
        }
    private:
        void do_accept() {
            acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        std::make_shared<Session>(std::move(socket))->start();
                    }
                    do_accept();
                }
            );
        }
    private:
        tcp::acceptor acceptor_;
};