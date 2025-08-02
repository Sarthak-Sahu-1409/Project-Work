#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <stdexcept>

namespace vms {  // Vaccine Management System namespace

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

template<typename T>
class Database {
private:
    std::string filename_;
    std::ios::openmode mode_;

public:
    explicit Database(std::string filename) 
        : filename_(std::move(filename)), mode_(std::ios::binary | std::ios::in | std::ios::out) {}

    void write(const T& record) {
        std::ofstream file(filename_, mode_ | std::ios::app);
        if (!file) {
            throw DatabaseException("Failed to open file for writing: " + filename_);
        }
        file.write(reinterpret_cast<const char*>(&record), sizeof(T));
    }

    std::optional<std::vector<T>> readAll() {
        std::ifstream file(filename_, mode_);
        if (!file) {
            return std::nullopt;
        }

        std::vector<T> records;
        T record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(T))) {
            records.push_back(record);
        }
        return records;
    }

    template<typename Predicate>
    std::optional<T> findFirst(Predicate predicate) {
        auto records = readAll();
        if (!records) {
            return std::nullopt;
        }

        auto it = std::find_if(records->begin(), records->end(), predicate);
        if (it != records->end()) {
            return *it;
        }
        return std::nullopt;
    }
};

} // namespace vms
