//
// Created by 刘优 on 16/5/12.
//

#ifndef ELONSERVER_COMMON_RETCOUNT_H_
#define ELONSERVER_COMMON_RETCOUNT_H_

#include <atomic>

#ifndef NDEBUG
#include <cassert>
#endif

namespace webmonitor {

class RefCounted {
public:
  RefCounted();

  void ref() const;

  bool unref() const;

  bool count_is_one() const;

protected:

  virtual ~RefCounted();

private:
  mutable std::atomic_int_fast32_t _ref{1};

  RefCounted(const RefCounted&) = delete;
  void operator=(const RefCounted&) = delete;

};

inline RefCounted::RefCounted() { }
inline RefCounted::~RefCounted() {
#ifndef NDEBUG
  assert(_ref.load() == 0);
#endif
  }

inline void RefCounted::ref() const {
#ifndef NDEBUG
  assert(_ref.load() >= 1);
#endif
  _ref.fetch_add(1, std::memory_order_relaxed);
}
inline bool RefCounted::unref() const {
#ifndef NDEBUG
  assert(_ref.load() > 0);
#endif
  // If _ref==1, this object is owned only by the caller. Bypass a locked op
  // in that case.
  if (_ref.load(std::memory_order_acquire) == 1 || _ref.fetch_sub(1) == 1) {
    // Make DCHECK in ~RefCounted happy
    _ref.store(0);
    delete this;
    return true;
  } else {
    return false;
  }
}

inline bool RefCounted::count_is_one() const {
  return (_ref.load(std::memory_order_acquire) == 1);
}

} //namespace webmonitor


#endif //ELONSERVER_COMMON_RETCOUNT_H_
