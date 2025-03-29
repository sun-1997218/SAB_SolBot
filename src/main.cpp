// src/main.cpp
#include "dex_websocket.hpp"
#include "order_book.hpp"
#include "monitor.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>


// 模拟DEX数据生成器（实际项目替换为真实WebSocket连接）
void run_mock_server(net::io_context& ioc, unsigned short port) {
    using namespace boost::asio;
    ip::tcp::acceptor acceptor(ioc, {ip::tcp::v4(), port});
    
    while(true) {
        ip::tcp::socket socket(ioc);
        acceptor.accept(socket);
        
        std::string mock_data = R"({
            "bids": [[100.1, 50], [100.0, 30]],
            "asks": [[100.2, 20], [100.3, 40]]
        })";
        
        boost::asio::write(socket, boost::asio::buffer(mock_data));
    }
}

int main() {
    net::io_context ioc;
    
    // 启动模拟服务器（实际项目删除这部分）
    std::thread mock_thread([&ioc]() {
        run_mock_server(ioc, 8080);
    });
    
    // 创建订单簿实例
    OrderBook raydium_book, serum_book;
    
    // 创建监控器（设置1%最小利润）
    ArbitrageMonitor monitor(raydium_book, serum_book, 0.01);
    monitor.start();
    
    // 模拟WebSocket数据更新（实际项目替换为真实连接）
    std::thread update_thread([&]() {
        while(true) {
            std::vector<PriceLevel> bids = {{100.1 + rand() % 10 * 0.1, 50}};
            std::vector<PriceLevel> asks = {{100.2 + rand() % 10 * 0.1, 20}};
            
            raydium_book.update(bids, asks);
            serum_book.update({{bids[0].price + 0.05, 30}}, {{asks[0].price - 0.05, 10}});
            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    
    std::cout << "Monitoring started. Press Enter to exit...\n";
    std::cin.get();
    
    // 清理
    monitor.stop();
    ioc.stop();
    mock_thread.join();
    update_thread.join();
    
    return 0;
}