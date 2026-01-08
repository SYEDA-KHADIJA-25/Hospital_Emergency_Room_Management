#pragma once
#include <string>

struct Resource {
    std::string id;
    std::string name;
    std::string type;
    std::string status;

    // Constructor declarations ONLY (no definitions!)
    Resource();
    Resource(const std::string& _id, const std::string& _name,
             const std::string& _type, const std::string& _status);

    // Method declarations ONLY (no definitions!)
    std::string getID() const;
    std::string getName() const;
    std::string getType() const;
    std::string getStatus() const;
};