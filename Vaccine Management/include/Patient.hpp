#pragma once
#include <string>
#include <array>
#include <chrono>

namespace vms {

class Patient {
public:
    struct ID {
        std::string aadhar;
        std::string phone;
    };

    enum class VaccinationStatus {
        NOT_VACCINATED,
        FIRST_DOSE,
        FULLY_VACCINATED
    };

private:
    ID id_;
    std::string name_;
    int age_;
    std::string gender_;
    VaccinationStatus status_;
    std::array<std::chrono::system_clock::time_point, 2> vaccination_dates_;
    int center_id_;

public:
    Patient(ID id, std::string name, int age, std::string gender)
        : id_(std::move(id))
        , name_(std::move(name))
        , age_(age)
        , gender_(std::move(gender))
        , status_(VaccinationStatus::NOT_VACCINATED)
        , center_id_(-1) {}

    // Getters
    const ID& getId() const { return id_; }
    const std::string& getName() const { return name_; }
    int getAge() const { return age_; }
    const std::string& getGender() const { return gender_; }
    VaccinationStatus getStatus() const { return status_; }
    int getCenterId() const { return center_id_; }

    // Vaccination methods
    bool receiveFirstDose(int center_id) {
        if (status_ != VaccinationStatus::NOT_VACCINATED) return false;
        status_ = VaccinationStatus::FIRST_DOSE;
        center_id_ = center_id;
        vaccination_dates_[0] = std::chrono::system_clock::now();
        return true;
    }

    bool receiveSecondDose(int center_id) {
        if (status_ != VaccinationStatus::FIRST_DOSE) return false;
        
        // Check if enough time has passed since first dose (e.g., 28 days)
        auto now = std::chrono::system_clock::now();
        auto days_since_first = std::chrono::duration_cast<std::chrono::hours>
            (now - vaccination_dates_[0]).count() / 24;
        
        if (days_since_first < 28) return false;

        status_ = VaccinationStatus::FULLY_VACCINATED;
        center_id_ = center_id;
        vaccination_dates_[1] = now;
        return true;
    }
};

} // namespace vms
