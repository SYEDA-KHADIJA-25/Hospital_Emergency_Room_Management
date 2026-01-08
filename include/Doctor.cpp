#include "Doctor.h"

// ONLY ONE constructor (matches .h)
Doctor::Doctor(const std::string& _id,
               const std::string& _name,
               const std::string& _specialty,
               const std::string& _status)
    : id(_id), name(_name), specialty(_specialty), status(_status) {}

// Getter definitions
std::string Doctor::getID() const {
    return id;
}

std::string Doctor::getName() const {
    return name;
}

std::string Doctor::getSpecialty() const {
    return specialty;
}

std::string Doctor::getStatus() const {
    return status;
}
