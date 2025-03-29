// include/monitor.hpp
#pragma once
#include <atomic>
#include <thread>
#include <iomanip>
#include "order_book.hpp"
#include <iostream>

class ArbitrageMonitor {
public:
    ArbitrageMonitor(OrderBook& book1, OrderBook& book2, double min_profit = 0.01)
        : book1_(book1), book2_(book2), min_profit_(min_profit), running_(false) {}
    
    void start() ;
    
    void stop() ;

private:
    void check_arbitrage();
    
    void print_opportunity(double spread, double buy_price, double sell_price) ;

    OrderBook& book1_;
    OrderBook& book2_;
    double min_profit_;
    std::atomic<bool> running_;
    std::thread thread_;
};

