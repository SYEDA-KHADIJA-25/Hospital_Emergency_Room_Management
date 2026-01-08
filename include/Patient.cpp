#include "Patient.h"

Patient::Patient(const std::string& _id,
                 const std::string& _name,
                 int _age,
                 const std::string& _condition,
                 Priority _priority)
    : id(_id), name(_name), age(_age), condition(_condition), priority(_priority) {}

std::string Patient::getID() const { return id; }
std::string Patient::getName() const { return name; }
int Patient::getAge() const { return age; }
std::string Patient::getCondition() const { return condition; }
Priority Patient::getPriority() const { return priority; }

std::string Patient::getPriorityStr() const {
    switch (priority) {
        case Priority::High: return "High";
        case Priority::Medium: return "Medium";
        default: return "Low";
    }
}
