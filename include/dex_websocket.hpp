// dex_websocket.h

// #include <boost/beast/websocket.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/strand.hpp>

// include/dex_websocket.hpp
#pragma once
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <functional>
#include <string>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

class DexWebSocket {
public:
    using MessageHandler = std::function<void(const std::string&)>;
    
    DexWebSocket(net::io_context& ioc, MessageHandler handler);
    void connect(const std::string& host, const std::string& port, const std::string& path);
    void close();

private:
    void on_resolve(beast::error_code ec, net::ip::tcp::resolver::results_type results);
    void on_connect(beast::error_code ec);
    void on_handshake(beast::error_code ec);
    void read_loop();
    void fail(beast::error_code ec, const char* what);

    net::ip::tcp::resolver resolver_;
    websocket::stream<net::ip::tcp::socket> ws_;
    beast::flat_buffer buffer_;
    MessageHandler handler_;
    std::string host_, path_;
};

