#include "Resource.h"

Resource::Resource() : id(""), name(""), type(""), status("") {}

Resource::Resource(const std::string& _id, const std::string& _name, const std::string& _type, const std::string& _status)
    : id(_id), name(_name), type(_type), status(_status) {}

std::string Resource::getID() const { return id; }
std::string Resource::getName() const { return name; }
std::string Resource::getType() const { return type; }
std::string Resource::getStatus() const { return status; }