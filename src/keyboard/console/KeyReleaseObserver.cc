#include "KeyReleaseObserver.h"

keyboard::console::KeyReleaseObserver::KeyReleaseObserver() {
  reset();
}

void keyboard::console::KeyReleaseObserver::keyDown() {
  m_activated = true;
}

void keyboard::console::KeyReleaseObserver::keyUp() {
  if (m_activated) {
    m_latched = true;
  }
}

bool keyboard::console::KeyReleaseObserver::wasReleased() {
  return m_latched;
}

void keyboard::console::KeyReleaseObserver::reset() {
  m_activated = false;
  m_latched = false;
}
