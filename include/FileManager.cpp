#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileManager::savePatients(const std::string& filename, const std::vector<Patient>& patients) {
    std::ofstream file(filename);
    file << "ID,Name,Age,Condition,Priority\n";

    for (const auto& p : patients) {
        file << p.getID() << ","
             << p.getName() << ","
             << p.getAge() << ","
             << p.getCondition() << ","
             << static_cast<int>(p.getPriority()) << "\n";
    }
}

std::vector<Patient> FileManager::loadPatients(const std::string& filename) {
    std::vector<Patient> patients;
    std::ifstream file(filename);
    std::string line;

    bool skipHeader = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (skipHeader) { skipHeader = false; continue; }

        std::stringstream ss(line);
        std::string id, name, ageStr, condition, priorityStr;

        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, ageStr, ',');
        std::getline(ss, condition, ',');
        std::getline(ss, priorityStr, ',');

        if (id.empty() || ageStr.empty()) continue;

        int age = std::stoi(ageStr);
        Priority priority = static_cast<Priority>(std::stoi(priorityStr));

        patients.emplace_back(id, name, age, condition, priority);
    }

    return patients;
}

// ======================= DOCTORS =======================

void FileManager::saveDoctors(const std::string& filename, const std::vector<Doctor>& doctors) {
    std::ofstream file(filename);
    file << "ID,Name,Specialty,Status\n";

    for (const auto& d : doctors) {
        file << d.getID() << ","
             << d.getName() << ","
             << d.getSpecialty() << ","
             << d.getStatus() << "\n";
    }
}

std::vector<Doctor> FileManager::loadDoctors(const std::string& filename) {
    std::vector<Doctor> doctors;
    std::ifstream file(filename);
    std::string line;
    bool skipHeader = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (skipHeader) { skipHeader = false; continue; }

        std::stringstream ss(line);
        std::string id, name, specialty, status;

        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, specialty, ',');
        std::getline(ss, status, ',');

        doctors.emplace_back(id, name, specialty, status);
    }

    return doctors;
}

// ======================= RESOURCES =======================

void FileManager::saveResources(const std::string& filename, const std::vector<Resource>& resources) {
    std::ofstream file(filename);
    file << "ID,Name,Type,Status\n";

    for (const auto& r : resources) {
        file << r.getID() << ","
             << r.getName() << ","
             << r.getType() << ","
             << r.getStatus() << "\n";
    }
}

std::vector<Resource> FileManager::loadResources(const std::string& filename) {
    std::vector<Resource> resources;
    std::ifstream file(filename);
    std::string line;
    bool skipHeader = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (skipHeader) { skipHeader = false; continue; }

        std::stringstream ss(line);
        std::string id, name, type, status;

        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, type, ',');
        std::getline(ss, status, ',');

        resources.emplace_back(id, name, type, status);
    }

    return resources;
}
