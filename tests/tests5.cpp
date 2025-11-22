#include "../fixed_memory.h"
#include "../forward_list.h"
#include <gtest/gtest.h>
#include <string>

struct Point {
  int x, y;
  std::string name;

  Point(int x = 0, int y = 0, const std::string &name = "")
      : x(x), y(y), name(name) {}

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y && name == other.name;
  }
};

TEST(ForwardListTest, PushFrontInt) {
  FixedBlockMemoryResource mr(1024);
  ForwardList<int> list(&mr);

  list.push_front(10);
  list.push_front(20);
  list.push_front(30);

  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.front(), 30);
}

TEST(ForwardListTest, PopFrontInt) {
  FixedBlockMemoryResource mr(1024);
  ForwardList<int> list(&mr);

  list.push_front(10);
  list.push_front(20);
  list.pop_front();

  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.front(), 10);
}

TEST(ForwardListTest, IteratorInt) {
  FixedBlockMemoryResource mr(1024);
  ForwardList<int> list(&mr);

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  std::vector<int> vals;
  for (auto v : list) {
    vals.push_back(v);
  }

  EXPECT_EQ(vals.size(), 3);
  EXPECT_EQ(vals[0], 3);
  EXPECT_EQ(vals[1], 2);
  EXPECT_EQ(vals[2], 1);
}

TEST(ForwardListTest, PushFrontStruct) {
  FixedBlockMemoryResource mr(2048);
  ForwardList<Point> list(&mr);

  list.push_front(Point(1, 2, "A"));
  list.push_front(Point(3, 4, "B"));

  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.front().x, 3);
  EXPECT_EQ(list.front().name, "B");
}

TEST(ForwardListTest, IteratorStruct) {
  FixedBlockMemoryResource mr(2048);
  ForwardList<Point> list(&mr);

  list.push_front(Point(1, 2, "First"));
  list.push_front(Point(3, 4, "Second"));

  auto it = list.begin();
  EXPECT_EQ(it->x, 3);
  ++it;
  EXPECT_EQ(it->name, "First");
}

TEST(MemoryResourceTest, ReuseMemory) {
  FixedBlockMemoryResource mr(512);
  ForwardList<int> list(&mr);

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  size_t blocks_before = mr.get_block_count();

  list.pop_front();
  list.pop_front();

  EXPECT_GT(mr.get_free_block_count(), 0);

  list.push_front(10);
  list.push_front(20);

  EXPECT_EQ(mr.get_block_count(), blocks_before);
}

TEST(MemoryResourceTest, ClearList) {
  FixedBlockMemoryResource mr(1024);
  ForwardList<int> list(&mr);

  for (int i = 0; i < 5; i++) {
    list.push_front(i);
  }

  list.clear();

  EXPECT_EQ(list.size(), 0);
  EXPECT_TRUE(list.empty());
  EXPECT_GT(mr.get_free_block_count(), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}