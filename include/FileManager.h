#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

#include "Patient.h"
#include "Doctor.h"
#include "Resource.h"

class FileManager {
public:
    // Patients
    static void savePatients(const std::string& filename,
                             const std::vector<Patient>& patients);
    static std::vector<Patient> loadPatients(const std::string& filename);

    // Doctors
    static void saveDoctors(const std::string& filename,
                            const std::vector<Doctor>& doctors);
    static std::vector<Doctor> loadDoctors(const std::string& filename);

    // Resources
    static void saveResources(const std::string& filename,
                              const std::vector<Resource>& resources);
    static std::vector<Resource> loadResources(const std::string& filename);
};

#endif
