// include/orderbook.hpp

#pragma once
#include <vector>
#include <mutex>
#include <cmath>

struct PriceLevel {
    double price;
    double amount;
    
    bool operator<(const PriceLevel& other) const {
        return price < other.price;
    }
};

class OrderBook {
public:
    void update(const std::vector<PriceLevel>& bids, const std::vector<PriceLevel>& asks) {
        std::lock_guard<std::mutex> lock(mutex_);
        bids_ = bids;
        asks_ = asks;
        std::sort(bids_.rbegin(), bids_.rend()); // 买盘降序
        std::sort(asks_.begin(), asks_.end());   // 卖盘升序
    }
    
    double best_bid() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return bids_.empty() ? NAN : bids_.front().price;
    }
    
    double best_ask() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return asks_.empty() ? NAN : asks_.front().price;
    }

private:
    mutable std::mutex mutex_;
    std::vector<PriceLevel> bids_, asks_;
};