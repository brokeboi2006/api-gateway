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
                        process_request();
                    }
                } 
            );
            
        }
        void do_write_dummy_response() {
            auto self = shared_from_this();
    
            // Создаем простейший ответ 200 OK
            auto res = std::make_shared<http::response<http::string_body>>(http::status::ok, req_.version());
            res->set(http::field::server, "My-C++-Gateway");
            res->body() = "Request received!";
            res->prepare_payload(); // Важно: Beast сам рассчитает Content-Length

            http::async_write(
                socket_, 
                *res,
                [this, self, res](beast::error_code ec, std::size_t) {
                    socket_.shutdown(tcp::socket::shutdown_send, ec); // Закрываем запись
            });
        }
        void process_request() {
            std::cout << "---- NEW REQUEST RECEIVED ----" << std::endl;
            std::cout << "request-method : " << req_.method_string() << std::endl;
            std::cout << "target string  : " << req_.target() << std::endl;

            std::cout << "-- HEADERS -- " << std::endl;
            for(const auto& field : req_) {
                std::cout << "  " << field.name_string() << " : " << field.value() << std::endl;
            }

            if(!req_.body().empty()) {
                std::cout << "Body: " << req_.body() << std::endl;
            }
            do_write_dummy_response();
        }
        private:
            tcp::socket socket_;
            beast::flat_buffer buffer_; 
            http::request<http::string_body> req_;
};