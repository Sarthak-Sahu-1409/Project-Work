#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include "../include/Database.hpp"
#include "../include/Patient.hpp"
#include "../include/VaccinationCenter.hpp"
#include "../include/Security.hpp"

namespace vms {

class VaccineManagementSystem {
private:
    Database<Patient> patient_db_;
    std::vector<std::unique_ptr<VaccinationCenter>> centers_;
    security::PasswordHasher password_hasher_;

    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    static void waitForKey() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void initializeCenters() {
        centers_.emplace_back(std::make_unique<VaccinationCenter>(1, "City Hospital", "Downtown", 1000));
        centers_.emplace_back(std::make_unique<VaccinationCenter>(2, "Medical Center", "Suburb", 1000));
        centers_.emplace_back(std::make_unique<VaccinationCenter>(3, "Community Clinic", "Rural", 1000));
    }

    bool authenticateAdmin() {
        std::string username, password;
        std::cout << "Enter admin username: ";
        std::cin >> username;
        std::cout << "Enter admin password: ";
        std::cin >> password;

        // In a real system, these would be stored securely, not hardcoded
        return (username == "admin" && 
                security::PasswordHasher::verifyPassword(password, ADMIN_PASSWORD_HASH));
    }

public:
    VaccineManagementSystem() 
        : patient_db_("patient_data.db")
    {
        initializeCenters();
    }

    void run() {
        while (true) {
            clearScreen();
            std::cout << "\n=== COVID-19 Vaccination Management System ===\n"
                      << "1. Admin Login\n"
                      << "2. Patient Registration\n"
                      << "3. Check Vaccination Status\n"
                      << "4. Exit\n"
                      << "Choose an option: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: adminMenu(); break;
                case 2: registerPatient(); break;
                case 3: checkStatus(); break;
                case 4: return;
                default: 
                    std::cout << "Invalid option!\n";
                    waitForKey();
            }
        }
    }

private:
    void adminMenu() {
        if (!authenticateAdmin()) {
            std::cout << "Authentication failed!\n";
            waitForKey();
            return;
        }

        while (true) {
            clearScreen();
            std::cout << "\n=== Admin Menu ===\n"
                      << "1. View Vaccine Stock\n"
                      << "2. Add Vaccine Stock\n"
                      << "3. View Patient Records\n"
                      << "4. Back to Main Menu\n"
                      << "Choose an option: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: viewStock(); break;
                case 2: addStock(); break;
                case 3: viewPatients(); break;
                case 4: return;
                default:
                    std::cout << "Invalid option!\n";
                    waitForKey();
            }
        }
    }

    void registerPatient() {
        std::string name, aadhar, phone, gender;
        int age;

        std::cout << "\nPatient Registration\n";
        std::cout << "Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        
        std::cout << "Aadhar Number: ";
        std::cin >> aadhar;
        
        std::cout << "Phone Number: ";
        std::cin >> phone;
        
        std::cout << "Age: ";
        std::cin >> age;
        
        std::cout << "Gender (M/F/O): ";
        std::cin >> gender;

        try {
            Patient::ID id{aadhar, phone};
            Patient patient(id, name, age, gender);
            patient_db_.write(patient);
            std::cout << "\nRegistration successful!\n";
        }
        catch (const DatabaseException& e) {
            std::cout << "\nError: " << e.what() << "\n";
        }

        waitForKey();
    }

    void checkStatus() {
        std::string aadhar;
        std::cout << "\nEnter Aadhar number: ";
        std::cin >> aadhar;

        auto patient = patient_db_.findFirst([&](const Patient& p) {
            return p.getId().aadhar == aadhar;
        });

        if (!patient) {
            std::cout << "Patient not found!\n";
        } else {
            std::cout << "\nName: " << patient->getName()
                      << "\nVaccination Status: ";
            
            switch (patient->getStatus()) {
                case Patient::VaccinationStatus::NOT_VACCINATED:
                    std::cout << "Not vaccinated\n";
                    break;
                case Patient::VaccinationStatus::FIRST_DOSE:
                    std::cout << "First dose completed\n";
                    break;
                case Patient::VaccinationStatus::FULLY_VACCINATED:
                    std::cout << "Fully vaccinated\n";
                    break;
            }
        }

        waitForKey();
    }

    void viewStock() {
        std::cout << "\nVaccine Stock Status:\n";
        for (const auto& center : centers_) {
            std::cout << center->getName() << " (" << center->getLocation() 
                      << "): " << center->getStock() << " doses\n";
        }
        waitForKey();
    }

    void addStock() {
        int center_id, amount;
        
        std::cout << "\nAvailable Centers:\n";
        for (const auto& center : centers_) {
            std::cout << center->getId() << ". " << center->getName() << "\n";
        }
        
        std::cout << "Enter center ID: ";
        std::cin >> center_id;
        
        auto it = std::find_if(centers_.begin(), centers_.end(),
            [center_id](const auto& center) { return center->getId() == center_id; });
        
        if (it == centers_.end()) {
            std::cout << "Invalid center ID!\n";
            waitForKey();
            return;
        }

        std::cout << "Enter amount to add: ";
        std::cin >> amount;
        
        if (amount > 0) {
            (*it)->addStock(amount);
            std::cout << "\nStock updated successfully!\n";
        } else {
            std::cout << "\nInvalid amount!\n";
        }
        
        waitForKey();
    }

    void viewPatients() {
        auto patients = patient_db_.readAll();
        if (!patients || patients->empty()) {
            std::cout << "\nNo patient records found!\n";
        } else {
            std::cout << "\nPatient Records:\n";
            for (const auto& patient : *patients) {
                std::cout << "\nName: " << patient.getName()
                          << "\nAadhar: " << patient.getId().aadhar
                          << "\nPhone: " << patient.getId().phone
                          << "\nStatus: ";
                
                switch (patient.getStatus()) {
                    case Patient::VaccinationStatus::NOT_VACCINATED:
                        std::cout << "Not vaccinated";
                        break;
                    case Patient::VaccinationStatus::FIRST_DOSE:
                        std::cout << "First dose completed";
                        break;
                    case Patient::VaccinationStatus::FULLY_VACCINATED:
                        std::cout << "Fully vaccinated";
                        break;
                }
                std::cout << "\n-------------------\n";
            }
        }
        waitForKey();
    }

private:
    static constexpr const char* ADMIN_PASSWORD_HASH = 
        "XYZ123abc$%^789"; // This would be a proper hash in production
};

} // namespace vms

int main() {
    try {
        vms::VaccineManagementSystem system;
        system.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
