#include "fixed_memory.h"
#include <stdexcept>

FixedBlockMemoryResource::FixedBlockMemoryResource(size_t size)
    : pool_size(size), used_size(0) {
  memory_pool = ::operator new(size);
}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
  ::operator delete(memory_pool);
}

void *FixedBlockMemoryResource::do_allocate(size_t bytes, size_t alignment) {

  for (auto &block : blocks) {
    if (block.is_free && block.size >= bytes) {
      block.is_free = false;
      return block.ptr;
    }
  }

  size_t offset = used_size;
  size_t space = pool_size - used_size;
  void *ptr = static_cast<char *>(memory_pool) + offset;

  if (std::align(alignment, bytes, ptr, space) == nullptr) {
    throw std::bad_alloc();
  }

  size_t aligned_offset =
      static_cast<char *>(ptr) - static_cast<char *>(memory_pool);

  if (aligned_offset + bytes > pool_size) {
    throw std::bad_alloc();
  }

  void *result = ptr;
  used_size = aligned_offset + bytes;

  blocks.push_back({result, bytes, false});
  return result;
}

void FixedBlockMemoryResource::do_deallocate(void *ptr, size_t bytes,
                                             size_t alignment) {
  for (auto &block : blocks) {
    if (block.ptr == ptr) {
      block.is_free = true;
      return;
    }
  }
}

bool FixedBlockMemoryResource::do_is_equal(
    const std::pmr::memory_resource &other) const noexcept {
  return this == &other;
}

size_t FixedBlockMemoryResource::get_free_block_count() const {
  size_t cnt = 0;
  for (const auto &b : blocks) {
    if (b.is_free)
      cnt++;
  }
  return cnt;
}