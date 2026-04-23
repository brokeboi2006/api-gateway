#pragma once

#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {   
    public:
        explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}
        void start() {
            do_read();
        }
    private:
        void do_read() {
            auto self(shared_from_this());

            req_ = {};

            http::async_read(
                socket_,
                buffer_,
                req_,
                [this, self](beast::error_code ec, std::size_t length) {
                    boost::ignore_unused(length);
                    if (!ec) {
                        // process_request();
                    }
                } 
            );
            
        }
    private:
        tcp::socket socket_;
        beast::flat_buffer buffer_; 
        http::request<http::string_body> req_;
};