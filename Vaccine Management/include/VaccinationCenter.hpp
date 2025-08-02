#pragma once
#include <string>
#include <atomic>
#include <mutex>

namespace vms {

class VaccinationCenter {
private:
    int id_;
    std::string name_;
    std::string location_;
    std::atomic<int> vaccine_stock_;
    mutable std::mutex mutex_;

public:
    VaccinationCenter(int id, std::string name, std::string location, int initial_stock = 0)
        : id_(id)
        , name_(std::move(name))
        , location_(std::move(location))
        , vaccine_stock_(initial_stock) {}

    // Thread-safe operations
    bool useVaccine() {
        int current = vaccine_stock_.load();
        while (current > 0) {
            if (vaccine_stock_.compare_exchange_weak(current, current - 1)) {
                return true;
            }
        }
        return false;
    }

    void addStock(int amount) {
        if (amount <= 0) return;
        vaccine_stock_ += amount;
    }

    // Getters
    int getId() const { return id_; }
    
    std::string getName() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return name_;
    }
    
    std::string getLocation() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return location_;
    }
    
    int getStock() const {
        return vaccine_stock_.load();
    }
};

} // namespace vms
