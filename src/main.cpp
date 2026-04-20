#include "gateway/network/server.hpp"
#include "gateway/network/session.hpp"

int main() {
    try {
        asio::io_context io_context;
        Server s(io_context, 6565);
        io_context.run();
    } catch(std::exception& e) {
        std::cerr << "error" << e.what() << std::endl;
    }
    return 0;
}