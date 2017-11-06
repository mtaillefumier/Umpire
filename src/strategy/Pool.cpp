#include "umpire/strategy/Pool.hpp"
#include "umpire/util/Macros.hpp"

#include "umpire/AllocatorTraits.hpp"

namespace umpire {
namespace strategy {

Pool::Pool(
    AllocatorTraits traits,
    std::vector<std::shared_ptr<AllocationStrategy> > providers) :
  m_current_size(0),
  m_highwatermark(0)
{
  m_slots = traits.m_number_allocations;

  m_allocator = providers[0];


  UMPIRE_LOG("Creating " << m_slots << "-slot pool.");

  m_lengths = new int[m_slots];
  m_pointers = new void*[m_slots];

  for (int i = 0; i < m_slots; ++i) {
    m_pointers[i] = nullptr;
    m_lengths[i] = 0;
  }
}

void*
Pool::allocate(size_t bytes)
{
  void* ptr = nullptr;

  for (int i = 0; i < m_slots; ++i) {
     if (m_lengths[i] == bytes) {
        m_lengths[i] = -m_lengths[i] ;
        ptr = m_pointers[i] ;
        break ;
     } else if (m_lengths[i] == 0) {
        m_lengths[i] = -static_cast<int>(bytes) ;
        m_pointers[i] = m_allocator->allocate(bytes);
        ptr = m_pointers[i] ;
        break ;
     }
  }

  return ptr;
}

void
Pool::deallocate(void* ptr)
{
  for (int i = 0; i < m_slots; ++i) {
    if (m_pointers[i] == ptr) {
      m_lengths[i] = -m_lengths[i];
      ptr = nullptr;
      break;
    }
  }
}

long 
Pool::getCurrentSize()
{
  return m_current_size;
}

long 
Pool::getHighWatermark()
{
  return m_highwatermark;
}

size_t
Pool::getSize(void* ptr)
{
  return m_allocator->getSize(ptr);
}

Platform
Pool::getPlatform()
{
  return m_allocator->getPlatform();
}


} // end of namespace alloc
} // end of namespace pool
