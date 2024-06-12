#include "AdjList.h"
#include <iostream>

// Initialize AdjList
int maps::AdjList::size = 0;
int maps::AdjList::capacity = 0;
maps::AdjList::Point *maps::AdjList::array = nullptr;

void maps::AdjList::Init(int size) {
  capacity = size;
  array = new Point[size];
  for (int i = 0; i < size; i++) {
    array[i].id = -1;
    array[i].next = nullptr;
  }
}

void maps::AdjList::AddAdj(int pos, int id) {
  size++;
  // if adj list for a pos in empty, just add the id
  if (array[pos].id == -1) {
    array[pos].id = id;
  } else {
    // if adj list for a pos in already exists
    Point *temp = &array[pos];
    while (temp->next != nullptr) {
      temp = temp->next;
      // Traverse to End
    }
    temp->next = new Point;
    temp->next->id = id;
    temp->next->next = nullptr;
  }
}

void maps::AdjList::Print() {
  for (int i = 0; i < capacity; i++) {
    std::cout << "Node " << i << ": ";
    Point *temp = &array[i];
    while (temp != nullptr) {
      std::cout << temp->id << " ";
      temp = temp->next;
    }
    std::cout << std::endl;
  }
}