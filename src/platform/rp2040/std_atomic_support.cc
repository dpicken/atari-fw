#ifdef BUILD_RP2040

#include "hardware/sync.h"

namespace {

spin_lock_t* makeAtomicLibSpinLock() {
  spin_lock_claim(PICO_SPINLOCK_ID_CLAIM_FREE_FIRST);
  return spin_lock_init(PICO_SPINLOCK_ID_CLAIM_FREE_FIRST);
}

static spin_lock_t* const AtomicLibSpinLock = makeAtomicLibSpinLock();

class SpinLock {
public:
  SpinLock(spin_lock_t* lock)
    : m_lock(lock)
    , m_irq(spin_lock_blocking(m_lock)) {
  }

  ~SpinLock() {
    spin_unlock(m_lock, m_irq);
  }

private:
  spin_lock_t* const m_lock;
  const uint32_t m_irq;
};

template<typename T>
inline bool atomicCompareExchange(volatile T* ptr, T* expected, T desired) {
  SpinLock lock(AtomicLibSpinLock);
  auto current = *ptr;
  if (current != *expected) {
    *expected = current;
    return false;
  }
  *ptr = desired;
  return true;
}

template<typename T>
inline unsigned int atomicExchange(volatile T* ptr, T desired) {
  SpinLock lock(AtomicLibSpinLock);
  auto old = *ptr;
  *ptr = desired;
  return old;
}

} // namespace

extern "C" {

bool __atomic_compare_exchange_4(volatile void* ptr, void* expected, unsigned int desired, bool weak, int success_memorder, int failure_memorder);
bool __atomic_compare_exchange_4(volatile void* ptr, void* expected, unsigned int desired, bool weak, int success_memorder, int failure_memorder) {
  (void) weak; (void) success_memorder; (void) failure_memorder;
  return atomicCompareExchange(reinterpret_cast<volatile unsigned int*>(ptr), reinterpret_cast<unsigned int*>(expected), desired);
}

unsigned int __atomic_exchange_4(volatile void* ptr, unsigned int desired , int memorder);
unsigned int __atomic_exchange_4(volatile void* ptr, unsigned int desired , int memorder) {
  (void) memorder;
  return atomicExchange(reinterpret_cast<volatile unsigned int*>(ptr), desired);
}

} // extern "C"

#endif // ifdef BUILD_RP2040
