#ifndef PATIENT_H
#define PATIENT_H

#include <string>

enum class Priority {
    Low = 0,
    Medium = 1,
    High = 2
};

struct Patient {
    std::string id;
    std::string name;
    int age;
    std::string condition;
    Priority priority;

    Patient(const std::string& _id = "",
            const std::string& _name = "",
            int _age = 0,
            const std::string& _condition = "",
            Priority _priority = Priority::Low);

    // Getters
    std::string getID() const;
    std::string getName() const;
    int getAge() const;
    std::string getCondition() const;
    Priority getPriority() const;
    std::string getPriorityStr() const;
};

#endif
