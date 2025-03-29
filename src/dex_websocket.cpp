// src/dex_websocket.cpp
#include "dex_websocket.hpp"
#include <iostream>

DexWebSocket::DexWebSocket(net::io_context& ioc, MessageHandler handler)
    : resolver_(net::make_strand(ioc)), 
      ws_(net::make_strand(ioc)),
      handler_(std::move(handler)) {}

void DexWebSocket::connect(const std::string& host, const std::string& port, const std::string& path) {
    host_ = host;
    path_ = path;
    resolver_.async_resolve(host, port,
        [this](auto ec, auto results) { on_resolve(ec, results); });
}

void DexWebSocket::on_resolve(beast::error_code ec, net::ip::tcp::resolver::results_type results) {
    if(ec) return fail(ec, "resolve");
       // 获取底层 TCP socket 并异步连接
    beast::get_lowest_layer(ws_).async_connect(
        *results.begin(),
        [this](auto ec) { on_connect(ec); }
    );
}

void DexWebSocket::on_connect(beast::error_code ec) {
    if(ec) return fail(ec, "connect");
    ws_.async_handshake(host_, path_,
        [this](auto ec) { on_handshake(ec); });
}

void DexWebSocket::on_handshake(beast::error_code ec) {
    if(ec) return fail(ec, "handshake");
    read_loop();
}

void DexWebSocket::read_loop() {
    ws_.async_read(buffer_,
        [this](auto ec, size_t) {
            if(ec) return fail(ec, "read");
            handler_(beast::buffers_to_string(buffer_.data()));
            buffer_.consume(buffer_.size());
            read_loop();
        });
}

void DexWebSocket::fail(beast::error_code ec, const char* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}