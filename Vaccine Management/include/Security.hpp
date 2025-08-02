#pragma once
#include <string>
#include <functional>
#include <random>
#include <ctime>

namespace vms {
namespace security {

class PasswordHasher {
private:
    static constexpr size_t SALT_LENGTH = 16;
    static constexpr size_t HASH_ITERATIONS = 10000;

    static std::string generateSalt() {
        static const char charset[] = "0123456789"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz";
        static std::mt19937 generator(std::random_device{}());
        static std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

        std::string salt;
        salt.reserve(SALT_LENGTH);
        for (size_t i = 0; i < SALT_LENGTH; ++i) {
            salt += charset[distribution(generator)];
        }
        return salt;
    }

public:
    static std::string hashPassword(const std::string& password) {
        std::string salt = generateSalt();
        return hashWithSalt(password, salt);
    }

    static bool verifyPassword(const std::string& password, const std::string& storedHash) {
        if (storedHash.length() <= SALT_LENGTH) return false;
        
        std::string salt = storedHash.substr(0, SALT_LENGTH);
        std::string computedHash = hashWithSalt(password, salt);
        return computedHash == storedHash;
    }

private:
    static std::string hashWithSalt(const std::string& password, const std::string& salt) {
        std::string salted = salt + password;
        std::hash<std::string> hasher;
        size_t hash = hasher(salted);

        // Multiple iterations to make it more secure
        for (size_t i = 1; i < HASH_ITERATIONS; ++i) {
            hash = hasher(std::to_string(hash));
        }

        return salt + std::to_string(hash);
    }
};

} // namespace security
} // namespace vms
