#ifndef FIXED_MEMORY_H
#define FIXED_MEMORY_H

#include <cstddef>
#include <list>
#include <memory_resource>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
  struct Block {
    void *ptr;
    size_t size;
    bool is_free;
  };

  void *memory_pool;
  size_t pool_size;
  size_t used_size;
  std::list<Block> blocks;

protected:
  void *do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void *ptr, size_t bytes, size_t alignment) override;
  bool
  do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

public:
  FixedBlockMemoryResource(size_t size);
  ~FixedBlockMemoryResource();

  size_t get_used_size() const { return used_size; }
  size_t get_block_count() const { return blocks.size(); }
  size_t get_free_block_count() const;
};

#endif