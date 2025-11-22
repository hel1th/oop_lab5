#include "fixed_memory.h"
#include "forward_list.h"
#include <iostream>

struct Point {
  int x, y;
  std::string name;
  Point(int x = 0, int y = 0, const std::string &name = "")
      : x(x), y(y), name(name) {}
};

int main() {
  std::cout << "=== Демонстрация работы ===\n\n";

  std::cout << "Работа с int:\n";
  FixedBlockMemoryResource mr1(1024);
  ForwardList<int> list1(&mr1);

  for (int i = 0; i < 5; i++) {
    list1.push_front(i * 10);
  }

  std::cout << "Элементы: ";
  for (auto v : list1) {
    std::cout << v << " ";
  }
  std::cout << "\nРазмер: " << list1.size() << "\n\n";

  std::cout << "Работа со struct:\n";
  FixedBlockMemoryResource mr2(2048);
  ForwardList<Point> list2(&mr2);

  list2.push_front(Point(1, 2, "Точка A"));
  list2.push_front(Point(3, 4, "Точка B"));

  std::cout << "Точки:\n";
  for (const auto &p : list2) {
    std::cout << "  (" << p.x << ", " << p.y << ") - " << p.name << "\n";
  }

  std::cout << "\nГотово!\n";
  return 0;
}