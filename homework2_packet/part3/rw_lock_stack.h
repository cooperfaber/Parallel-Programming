#include <shared_mutex>
using namespace std;

class Llist_node {
 public:  
  Llist_node(int d) {   
    data = d;
    next = NULL;
  }
  
  int data;
  Llist_node *next;
};

class CSE113_Stack {
 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    //double check if we *need* to take the big lock
    internal_mutex.lock_shared();
    if (start == NULL) {
        internal_mutex.unlock_shared();
        return -1;
    }
    //we probably do
    internal_mutex.unlock_shared();
    internal_mutex.lock();
    if (start == NULL) {
      internal_mutex.unlock();
      return -1;
    }
    if (start->next == NULL) {
      int ret = start->data;
      delete start;
      start = NULL;
      internal_mutex.unlock();
      return ret;
    }
    Llist_node *current = start->next;
    Llist_node *previous = start;
    while (current->next != NULL) {
      previous = current;
      current = current->next;
    }
    int ret = current->data;
    previous->next = NULL;
    delete current;
    internal_mutex.unlock();
    return ret;
  }

  
  int peek() {
    internal_mutex.lock_shared();
    if (start == NULL) {
      internal_mutex.unlock_shared();
      return -1;
    }
    
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }
    int ret = current->data;
    internal_mutex.unlock_shared();
    return ret;    
  }
  
  void push(int p) {
    internal_mutex.lock();
    if (start == NULL) {

      start = new Llist_node(p);
      internal_mutex.unlock();
      return;
    }

    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    current->next = new Llist_node(p);
    internal_mutex.unlock();
  }

 private:
  Llist_node *start;
  shared_mutex internal_mutex;
};

