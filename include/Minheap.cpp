#include "Minheap.h"

void Triage::bubbleUp(int idx) {
    if(idx == 0) return;
    int parent = (idx - 1) / 2;
    if(static_cast<int>(heap[idx].priority) > static_cast<int>(heap[parent].priority)) {
        std::swap(heap[idx], heap[parent]);
        bubbleUp(parent);
    }
}

void Triage::bubbleDown(int idx) {
    int left = 2*idx + 1, right = 2*idx + 2, largest = idx;
    if(left < heap.size() && static_cast<int>(heap[left].priority) > static_cast<int>(heap[largest].priority))
        largest = left;
    if(right < heap.size() && static_cast<int>(heap[right].priority) > static_cast<int>(heap[largest].priority))
        largest = right;
    if(largest != idx) {
        std::swap(heap[idx], heap[largest]);
        bubbleDown(largest);
    }
}

void Triage::push(const Patient& p) {
    heap.push_back(p);
    bubbleUp(heap.size() - 1);
}

Patient Triage::pop() {
    if(heap.empty()) return Patient();
    Patient top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    bubbleDown(0);
    return top;
}
