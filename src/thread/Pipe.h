#ifndef thread_Pipe_h
#define thread_Pipe_h

#include <atomic>

namespace thread {

template<typename Message>
class Pipe {
public:
  static constexpr Message NullMessage = Message();

  Pipe() {
  }

  Pipe(const Pipe&) = delete;
  Pipe& operator=(const Pipe&) = delete;

  bool tryPush(const Message& message) {
    Message expected = NullMessage;
    return m_message.compare_exchange_strong(expected, message, std::memory_order_relaxed);
  }

  Message tryPop() {
    return m_message.exchange(NullMessage, std::memory_order_relaxed);
  }

private:
  std::atomic<Message> m_message;
};

} // namespace thread

#endif // ifndef thread_Pipe_h
