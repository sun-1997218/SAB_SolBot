#include "monitor.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

void ArbitrageMonitor::start() {
    running_ = true;
    thread_ = std::thread([this]() {
        while (running_) {
            check_arbitrage();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void ArbitrageMonitor::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void ArbitrageMonitor::check_arbitrage() {
    double bid2 = book2_.best_bid();
    double ask1 = book1_.best_ask();
    
    if (!std::isnan(bid2) && !std::isnan(ask1)) {
        double spread = bid2 - ask1;
        if (spread >= min_profit_) {
            print_opportunity(spread, ask1, bid2); // 传递三个参数
        }
    }
}

void ArbitrageMonitor::print_opportunity(double spread, double buy_price, double sell_price) {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "\n[ARBITRAGE] " 
              << std::put_time(std::localtime(&now_time), "%T")
              << " | Spread: " << std::fixed << std::setprecision(4) << spread
              << " | Buy@DEX1: " << buy_price
              << " | Sell@DEX2: " << sell_price << "\n";
}