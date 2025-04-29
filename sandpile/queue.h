#ifndef QUE_H
#define QUE_H

#include <cstdint>
#include <stdexcept>

// Represents a cell in the sandpile model
struct Cell {
    int16_t x;            // x-coordinate
    int16_t y;            // y-coordinate
    uint64_t sand_count;  // number of sand grains

    Cell(int16_t x, int16_t y, uint64_t count) 
        : x(x), y(y), sand_count(count) {}
};

// Node for doubly-linked list implementation
struct Node {
    Cell value;
    Node* next;
    Node* prev;
    
    explicit Node(const Cell& cell) 
        : value(cell), next(nullptr), prev(nullptr) {}
};

// Doubly-linked list implementation for sandpile simulation
class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    
    // Accessors
    bool IsEmpty() const;
    Cell Front() const;
    
    // Modifiers
    void PushBack(const Cell& cell);
    void PopFront();
    
    // Utility
    void OffsetCoordinates(bool offset_x, bool offset_y);
    void Print() const;

private:
    Node* head_;    // First node in list
    Node* tail_;    // Last node in list
    size_t size_;   // Current number of nodes
};

#endif // QUE_H
