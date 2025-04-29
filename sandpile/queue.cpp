#include <iostream>
#include "queue.h"

// Initialize empty linked list
LinkedList::LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

// Clean up all nodes when destroyed
LinkedList::~LinkedList() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// Get front element (without removal)
Cell LinkedList::Front() const {
    if (head_ == nullptr) {
        throw std::runtime_error("Accessing empty list");
    }
    return head_->value;
}

// Add new cell to end of list
void LinkedList::PushBack(const Cell& cell) {
    Node* new_node = new Node(cell);
    if (size_ == 0) {
        head_ = tail_ = new_node;
    } else {
        new_node->prev = tail_;
        tail_->next = new_node;
        tail_ = new_node;
    }
    size_++;
}

// Shift all cell coordinates (for grid expansion)
void LinkedList::OffsetCells(bool offset_x, bool offset_y) {
    Node* current = head_;
    while (current != nullptr) {
        if (offset_x) current->value.x++;
        if (offset_y) current->value.y++;
        current = current->next;
    }
}

// Check if list contains no elements
bool LinkedList::IsEmpty() const {
    return head_ == nullptr;
}

// Remove first element from list
void LinkedList::PopFront() {
    if (size_ == 0) {
        throw std::runtime_error("Popping from empty list");
    }

    if (size_ == 1) {
        delete head_;
        head_ = tail_ = nullptr;
    } else {
        Node* new_head = head_->next;
        new_head->prev = nullptr;
        delete head_;
        head_ = new_head;
    }
    size_--;
}

// Print all cells (debugging purposes)
void LinkedList::Print() const {
    Node* current = head_;
    while (current != nullptr) {
        std::cout << "(" << current->value.x << ", " << current->value.y 
                  << "): " << current->value.sand_count << "\n";
        current = current->next;
    }
}
