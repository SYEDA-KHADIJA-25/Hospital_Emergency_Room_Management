#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>

struct Doctor {
    std::string id;
    std::string name;
    std::string specialty;
    std::string status;

    // ONLY declaration
    Doctor(const std::string& _id = "",
           const std::string& _name = "",
           const std::string& _specialty = "",
           const std::string& _status = "");

    // ONLY declarations
    std::string getID() const;
    std::string getName() const;
    std::string getSpecialty() const;
    std::string getStatus() const;
};

#endif
