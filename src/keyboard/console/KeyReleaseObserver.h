#ifndef keyboard_console_KeyReleaseObserver_h
#define keyboard_console_KeyReleaseObserver_h

namespace keyboard { namespace console {

class KeyReleaseObserver {
public:
  KeyReleaseObserver();

  void keyDown();
  void keyUp();

  bool wasReleased();
  void reset();

private:
  bool m_activated;
  bool m_latched;
};

} } // namespace keyboard::console

#endif // ifndef keyboard_console_KeyReleaseObserver_h
