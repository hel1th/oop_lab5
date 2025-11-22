#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iterator>
#include <memory_resource>
#include <stdexcept>

template <typename T> class ForwardList {
private:
  struct Node {
    T data;
    Node *next;

    template <typename... Args>
    Node(Args &&...args) : data(std::forward<Args>(args)...), next(nullptr) {}
  };

  Node *head;
  std::pmr::polymorphic_allocator<Node> alloc;
  size_t sz;

public:
  class Iterator {
  private:
    Node *cur;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    Iterator(Node *node) : cur(node) {}

    T &operator*() { return cur->data; }
    T *operator->() { return &cur->data; }

    Iterator &operator++() {
      cur = cur->next;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      cur = cur->next;
      return tmp;
    }

    bool operator==(const Iterator &other) const { return cur == other.cur; }
    bool operator!=(const Iterator &other) const { return cur != other.cur; }
  };

  ForwardList(std::pmr::memory_resource *mr = std::pmr::get_default_resource());
  ~ForwardList();

  template <typename... Args> void push_front(Args &&...args);

  void pop_front();
  void clear();

  size_t size() const { return sz; }
  bool empty() const { return head == nullptr; }

  Iterator begin() { return Iterator(head); }
  Iterator end() { return Iterator(nullptr); }

  T &front();
};

template <typename T>
ForwardList<T>::ForwardList(std::pmr::memory_resource *mr)
    : head(nullptr), alloc(mr), sz(0) {}

template <typename T> ForwardList<T>::~ForwardList() { clear(); }

template <typename T>
template <typename... Args>
void ForwardList<T>::push_front(Args &&...args) {
  Node *new_node = alloc.allocate(1);
  alloc.construct(new_node, std::forward<Args>(args)...);
  new_node->next = head;
  head = new_node;
  sz++;
}

template <typename T> void ForwardList<T>::pop_front() {
  if (!head)
    return;

  Node *tmp = head;
  head = head->next;
  alloc.destroy(tmp);
  alloc.deallocate(tmp, 1);
  sz--;
}

template <typename T> void ForwardList<T>::clear() {
  while (head) {
    pop_front();
  }
}

template <typename T> T &ForwardList<T>::front() {
  if (!head)
    throw std::out_of_range("список пустой");
  return head->data;
}

#endif