# COVID-19 Vaccination Management System

A modern C++ application for managing COVID-19 vaccination centers, patient records, and vaccine stock.

## Features

- Secure admin authentication with password hashing
- Patient registration and status tracking
- Vaccine stock management across multiple centers
- Thread-safe operations
- Exception handling and error recovery
- Modern C++17 implementation

## Project Structure

`
include/
 Database.hpp      - Generic database operations
 Patient.hpp      - Patient data management
 Security.hpp     - Authentication and security
 VaccinationCenter.hpp - Center management

src/
 main.cpp         - Application entry point
`

## Requirements

- C++17 compatible compiler
- CMake 3.15 or higher (for building)

## Usage

1. **Admin Login**
   - Username: admin
   - Password: (contact administrator)

2. **Patient Registration**
   - Requires Aadhar number
   - Phone number
   - Basic personal information

3. **Check Vaccination Status**
   - Search by Aadhar number
   - View vaccination history

4. **Admin Features**
   - Manage vaccine stock
   - View patient records
   - Add/update center information

## Security Features

- Password hashing with salt
- Input validation
- Exception handling
- Thread-safe operations

## Technical Details

- Modern C++ features (smart pointers, RAII)
- Template-based generic database
- Thread-safe stock management
- Exception-safe operations
