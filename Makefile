# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude -Wall

# Source files
SRC = src/main.cpp \
      include/Module1_PatientManagement/Patient.cpp \
      include/Module1_PatientManagement/MinHeap.cpp \
      include/Module2_DoctorManagement/Doctor.cpp \
      include/Module2_DoctorManagement/Resource.cpp \
      include/Module3_MedicalRecords/BST.cpp \
      include/Module3_MedicalRecords/FileManager.cpp
      # GUI files can be added later:
      # include/Module4_GUI/GUI.cpp

# Output executable
TARGET = HospitalER

# Default rule: build executable
all: $(TARGET)

# Link all object files to create executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean compiled files
clean:
	rm -f $(TARGET)
