#pragma once
#include <vector>
#include <algorithm>
#include "Patient.h"

struct Triage {
    std::vector<Patient> heap;

    void bubbleUp(int idx);
    void bubbleDown(int idx);
    void push(const Patient& p);
    Patient pop();
    bool isEmpty() const { return heap.empty(); }
    const std::vector<Patient>& getHeap() const { return heap; }
};
