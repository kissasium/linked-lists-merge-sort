
#pragma once

#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cerr, std::cout
#include <ostream> // for std::ostream

template <typename T>
class LinkedList {
public:

  // Node type that is particular to the LinkedList<T> type
  class Node {
  public:
    // The next node in the list, or nullptr if this is the last node.
    Node* next;
    // The previous node in the list, or nullptr if this is the first node.
    Node* prev;
    // An actual data item that this node contains.
    T data;
    
    // Default constructor: This lets data be constructed by
    // the default constructor of the T type.
    Node() : next(nullptr), prev(nullptr) {}

    // Argument constructor: Specifies the data that should be copied into
    // the T data member variable.
    Node(const T& dataArg) : next(nullptr), prev(nullptr), data(dataArg) {}

    Node(const Node& other) : next(other.next), prev(other.prev),
      data(other.data) {}

    // Copy assignment operator: Please see the note above.
    Node& operator=(const Node& other) {
      next = other.next;
      prev = other.prev;
      data = other.data;
      return *this;
    }

    ~Node() {}

  };

private:
  Node* head_;
  // The last node in the list, or nullptr if the list is empty.
  Node* tail_;


  int size_;

public:

  static constexpr char LIST_GENERAL_BUG_MESSAGE[] = "[Error] Probable causes: wrong head_ or tail_ pointer, or some next or prev pointer not updated, or wrong size_";

  Node* getHeadPtr() { return head_; }
  Node* getTailPtr() { return tail_; }

  int size() const { return size_; }

  bool empty() const { return !head_; }

  T& front(){
    if (!head_) {
      throw std::runtime_error("front() called on empty LinkedList");
    }
    else {
      return head_->data;
    }
  }

  const T& front() const {
    if (!head_) {
      throw std::runtime_error("front() called on empty LinkedList");
    }
    else {
      return head_->data;
    }
  }

  // Please see the note about "T& front()" above.
  T& back(){
    if (!tail_) {
      throw std::runtime_error("back() called on empty LinkedList");
    }
    else {
      return tail_->data;
    }
  }

  // Please see the note about "const T& front() const" above.
  const T& back() const {
    if (!tail_) {
      throw std::runtime_error("back() called on empty LinkedList");
    }
    else {
      return tail_->data;
    }
  }

  // Push a copy of the new data item onto the front of the list.
  void pushFront(const T& newData);
  // Push a copy of the new data item onto the back of the list.
  void pushBack(const T& newData);
  // Delete the front item of the list.
  void popFront();
  // Delete the back item of the list.
  void popBack();
  
  // Delete all items in the list, leaving it empty.
  void clear() {
    // As long as there are items left in the list, remove the tail item.
    while (head_) {
      popBack();
    }

    if (0 != size_) throw std::runtime_error(std::string("Error in clear: ") + LIST_GENERAL_BUG_MESSAGE);
  }

  // Two lists are equal if they have the same length
  // and the same data items in each position.
  // This check runs in O(n) time.
  bool equals(const LinkedList<T>& other) const;
  bool operator==(const LinkedList<T>& other) const {
    return equals(other);
  }
  bool operator!=(const LinkedList<T>& other) const {
    return !equals(other);
  }

  // Output a string representation of the list.
  // This requires that the data type T supports stream output itself.
  // This is used by the operator<< overload defined in this file.
  std::ostream& print(std::ostream& os) const;

  void insertOrdered(const T& newData);

  // Checks whether the list is currently sorted in increasing order.
  // This is true if for all adjacent pairs of items A and B in the list: A <= B.
  bool isSorted() const;

  LinkedList<T> insertionSort() const;

  LinkedList<LinkedList<T>> splitHalves() const;

  LinkedList<LinkedList<T>> explode() const;
  
  // Assuming this list instance is currently sorted, and the "other" list is
  // also already sorted, then merge returns a new sorted list containing all
  // of the items from both of the original lists, in linear time.
  // (This definition is in a separate file for the homework exercises.)
  LinkedList<T> merge(const LinkedList<T>& other) const;
  
  // This is a wrapper function that calls one of either mergeSortRecursive
  // or mergeSortIterative.
  LinkedList<T> mergeSort() const;
  
  // The recursive version of the merge sort algorithm, which returns a new
  // list containing the sorted elements of the current list, in O(n log n) time.
  LinkedList<T> mergeSortRecursive() const;

  // The iterative version of the merge sort algorithm, which returns a new
  // list containing the sorted elements of the current list, in O(n log n) time.
  LinkedList<T> mergeSortIterative() const;

  // Default constructor: The list will be empty.
  LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
  
  // The copy assignment operator replicates the content of the other list
  // one element at a time so that pointers between nodes will be correct
  // for this copy of the list.
  LinkedList<T>& operator=(const LinkedList<T>& other) {
    // Clear the current list.
    clear();

    // We'll walk along the other list and push copies of its data.
    // (The declaration "const Node*" means a pointer to a const Node,
    //  not a const pointer to Node.)
    const Node* cur = other.head_;
    while (cur) {
      // Push back a copy of this node's data
      pushBack(cur->data);
      // Iterate
      cur = cur->next;
    }

    return *this;
  }
  
  // The copy constructor begins by constructing the default LinkedList,
  // then it does copy assignment from the other list. Please see the
  // definition of the copy assignment operator.
  LinkedList(const LinkedList<T>& other) : LinkedList() {
    *this = other;
  }

  // The destructor calls clear to deallocate all of the nodes.
  ~LinkedList() {
    clear();
  }

  // Checks whether the size has been correctly updated by member functions,
  // and otherwise throws an exception. This is for testing only.
  bool assertCorrectSize() const;
  
  // Checks whether the reverse-direction links in the list, given by
  // the prev pointers on the nodes, are correct. If an error is found,
  // this throws an exception. This is for testing only.
  bool assertPrevLinks() const;

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
  return list.print(os);
}

// In some versions of C++ we have to redeclare a constant static member
// at global scope like this to ensure that the linker doesn't give an error.
template <typename T>
constexpr char LinkedList<T>::LIST_GENERAL_BUG_MESSAGE[];

// Push a copy of the new data item onto the front of the list.
template <typename T>
void LinkedList<T>::pushFront(const T& newData) {

  // allocate a new node
  Node* newNode = new Node(newData);

  if (!head_) {
    // If empty, insert as the only item as both head and tail.
    // The Node already has next and prev set to nullptr by default.
    head_ = newNode;
    tail_ = newNode;
  }
  else {
    // Otherwise, add the new item as the head.
    // (We could rewrite this without the temporary variable "oldHead",
    //  but perhaps this way is clearer.)
    Node* oldHead = head_;
    oldHead->prev = newNode;
    newNode->next = oldHead;
    head_ = newNode;
  }

  // update size
  size_++;
}

// Push a copy of the new data item onto the back of the list.
template <typename T>
void LinkedList<T>::pushBack(const T& newData) {

  // allocate a new node
  Node* newNode = new Node(newData);

  if (!head_) {
    // If empty, insert as the only item as both head and tail.
    // The Node already has next and prev set to nullptr by default.
    head_ = newNode;
    tail_ = newNode;
  }
  else {
    // Otherwise, add the new item as the tail.
    // (We could rewrite this without the temporary variable "oldTail",
    //  but perhaps this way is clearer.)
    Node* oldTail = tail_;
    oldTail->next = newNode;
    newNode->prev = oldTail;
    tail_ = newNode;
  }

  // update size
  size_++;
}

// Delete the front item of the list.
template <typename T>
void LinkedList<T>::popFront() {

  // If list is empty, do nothing.
  if (!head_) return;

  // If the next item after the head is null, this is the last and only
  // item in the list.
  if (!head_->next) {
    // deallocate the only item
    delete head_;
    // reset list pointers
    head_ = nullptr;
    tail_ = nullptr;
    // decrease size and make sure it's zero
    size_--;
    if (0 != size_) throw std::runtime_error(std::string("Error in popFront: ") + LIST_GENERAL_BUG_MESSAGE);
    return;
  }

  // Otherwise, we need to remove the head item and set the following item
  // as the new head.

  // Temporarily save the old head pointer
  Node* oldHead = head_;
  // Update head_ to point to the following item.
  head_ = head_->next;
  // Now set the new head_'s previous pointer to null.
  head_->prev = nullptr;
  // Deallocate the old head_ item
  delete oldHead;
  // It's a good practice to set pointers to null after you delete them for safety,
  // even if you don't think you're going to dereference the same pointer again.
  oldHead = nullptr;

  // update size
  size_--;
}

// Delete the back item of the list.
template <typename T>
void LinkedList<T>::popBack() {

  // If list is empty, do nothing.
  if (!head_) return;

  // If the tail item's prev is null, then this is the last and only
  // item in the list.
  if (!tail_->prev) {
    // deallocate the only item
    delete tail_;
    // reset list pointers
    head_ = nullptr;
    tail_ = nullptr;
    // decrease size and make sure it's zero
    size_--;
    if (0 != size_) throw std::runtime_error(std::string("Error in popBack: ") + LIST_GENERAL_BUG_MESSAGE);
    return;
  }

  // Otherwise, we need to remove the tail item and set the preceding item
  // as the new tail.

  // Temporarily save the old tail_ pointer
  Node* oldTail = tail_;
  // Update tail_ to point to the preceding item
  tail_ = tail_->prev;
  // Now set the new tail_'s next pointer to null.
  tail_->next = nullptr;
  // Deallocate the old tail_ item
  delete oldTail;
  // It's a good practice to set pointers to null after you delete them for safety,
  // even if you don't think you're going to dereference the same pointer again.
  oldTail = nullptr;

  // update size
  size_--;
}

// Checks whether the list is currently sorted in increasing order.
// This is true if for all adjacent pairs of items A and B in the list: A <= B.
template <typename T>
bool LinkedList<T>::isSorted() const {
  // Lists of size 0 or 1 are sorted.
  if (size_ < 2) return true;

  // If the list was not empty, then the head pointer should not be null.
  // But you could verify that manually for safety, as always.
  if (!head_) throw std::runtime_error(std::string("Error in isSorted: ") + LIST_GENERAL_BUG_MESSAGE);

  // There are at least two items in the list. We'll compare all adjacent
  // pairs to see if the sorted condition is maintained.
  const Node* prev = nullptr;
  const Node* cur = head_;
  while (cur->next) {
    // Step forward
    prev = cur;
    cur = cur->next;
    if (!(prev->data <= cur->data)) {
      // Previous data was not <= current data, so return false.
      return false;
    }
  }

  // If the function hasn't returned by now, then the sorted condition
  // was maintained throughout the loop.
  return true;
}

// Two lists are equal if they have the same length
// and the same data items in each position.
// This check runs in O(n) time.
template <typename T>
bool LinkedList<T>::equals(const LinkedList<T>& other) const {

  // If the lists are different sizes, they don't have the same contents.
  if (size_ != other.size_) {
    return false;
  }

  // We'll iterate along both lists and check that all items match by value.
  const Node* thisCur = head_;
  const Node* otherCur = other.head_;

  while (thisCur) {
    if (!otherCur) {
      throw std::runtime_error(std::string("Error in equals: ") + "otherCur missing a node or wrong item count");
    }
    if (thisCur->data != otherCur->data) {
      return false;
    }
    thisCur = thisCur->next;
    otherCur = otherCur->next;
  }

  return true;
}

template <typename T>
LinkedList<T> LinkedList<T>::insertionSort() const {
  // Make result list
  LinkedList<T> result;

  // Walk along the original list and insert the items to the result in order.
  const Node* cur = head_;
  while (cur) {
    result.insertOrdered(cur->data);
    cur = cur->next;
  }

  return result;
}


template <typename T>
std::ostream& LinkedList<T>::print(std::ostream& os) const {
  // List format will be [(1)(2)(3)], etc.
  os << "[";

  // Note that this works correctly for an empty list.
  Node* cur = head_;
  while (cur) {
    os << "(" << cur->data << ")";
    cur = cur->next;
  }

  os << "]";

  return os;
}

template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::splitHalves() const {

  // Prepare a list of lists for the result:
  LinkedList<LinkedList<T>> halves;
  // Prepare a working copy of "*this" object to be split:
  LinkedList<T> leftHalf = *this;
  // Prepare an empty right half to fill:
  LinkedList<T> rightHalf;

  if (size_ < 2) {
    halves.pushBack(leftHalf);
    halves.pushBack(rightHalf);
    return halves;
  }

  int rightHalfLength = size_ / 2;

  for (int i=0; i<rightHalfLength; i++) {

    T dataToCopy = leftHalf.back();
    rightHalf.pushFront(dataToCopy);
    // Remove the element from the left half:
    leftHalf.popBack();
  }

  halves.pushBack(leftHalf);
  halves.pushBack(rightHalf);

  return halves;
}


template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::explode() const {

  LinkedList<T> workingCopy = *this;

  LinkedList< LinkedList<T> > lists;

  while (!workingCopy.empty()) {
    LinkedList<T> singletonList;
    singletonList.pushBack(workingCopy.front());
    workingCopy.popFront();
    lists.pushBack(singletonList);
  }

  return lists;
}

// The recursive version of the merge sort algorithm, which returns a new
// list containing the sorted elements of the current list, in O(n log n) time.
template <typename T>
LinkedList<T> LinkedList<T>::mergeSortRecursive() const {

  if (size_ < 2) {
    // Return a copy of the current list.
    return *this;
  }

  // Split this list into a list of two lists (the left and right halves)
  LinkedList<LinkedList<T>> halves = splitHalves();

  LinkedList<T>& left = halves.front();
  LinkedList<T>& right = halves.back();

  // Relying on the inductive hypothesis that our algorithm successfully
  // sorts a smaller list than the original input, we recurse on each of
  // the two halves.
  left = left.mergeSortRecursive();
  right = right.mergeSortRecursive();

  
  return left.merge(right);


}


template <typename T>
LinkedList<T> LinkedList<T>::mergeSortIterative() const {


  if (size_ < 2) {
    // Return a copy of the current list.
    return *this;
  }

  LinkedList< LinkedList<T> > workQueue = explode();

  while(workQueue.size() > 1) {
    // Remove two lists from the front of the queue.
    LinkedList<T> left = workQueue.front();
    workQueue.popFront();
    LinkedList<T> right = workQueue.front();
    workQueue.popFront();
    // Merge the two lists.
    LinkedList<T> merged = left.merge(right);

    workQueue.pushBack(merged);
  }

  // When the workQueue is reduced to size 1, its only element is the result.
  return workQueue.front();
}

// This is a wrapper function that calls one of either mergeSortRecursive
// or mergeSortIterative.
template <typename T>
LinkedList<T> LinkedList<T>::mergeSort() const {

  // As a wrapper function, this should only call one version of mergeSort
  // or the other and return that result.

  return mergeSortRecursive();
  // return mergeSortIterative();

}

template <typename T>
bool LinkedList<T>::assertCorrectSize() const {
  int itemCount = 0;
  const Node* cur = head_;
  while (cur) {
    itemCount++;
    cur = cur->next;
  }
  if (itemCount != size_) throw std::runtime_error(std::string("Error in assertCorrectSize: ") + LIST_GENERAL_BUG_MESSAGE);
  else return true;
}

template <typename T>
bool LinkedList<T>::assertPrevLinks() const {
  // These should end up being the same list, but we'll build one
  // in the forward direction and the other in the reverse direction.
  LinkedList<const Node*> forwardPtrList;
  LinkedList<const Node*> reversePtrList;
  {
    const Node* cur = head_;
    while (cur) {
      forwardPtrList.pushBack(cur);
      cur = cur->next;
    }
  }
  {
    const Node* cur = tail_;
    while (cur) {
      reversePtrList.pushFront(cur);
      cur = cur->prev;
    }
  }

  if (forwardPtrList == reversePtrList) return true;
  else throw std::runtime_error(std::string("Error in assertPrevLinks: ") + LIST_GENERAL_BUG_MESSAGE);
}


#include "LinkedListExercises.h"

