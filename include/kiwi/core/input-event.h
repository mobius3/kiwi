#ifndef KW_INPUT_EVENT_H
#define KW_INPUT_EVENT_H

#include <stdint.h>
#include "input-key.h"

typedef enum KW_InputEventType {
  KW_INPUT_UNKNOWN,
  KW_INPUT_CURSOR_MOTION,
  KW_INPUT_CURSOR_BUTTON_DOWN,
  KW_INPUT_CURSOR_BUTTON_UP,
  KW_INPUT_TEXT,
  KW_INPUT_KEY_DOWN,
  KW_INPUT_KEY_UP
} KW_InputEventType;

struct KW_InputEventCursorMotion {
  KW_InputEventType type;
  uint32_t cursorId;
  int32_t x;
  int32_t y;
  int32_t xMotion;
  int32_t yMotion;
};

struct KW_InputEventCursorButton {
  KW_InputEventType type;
  uint32_t cursorId;
  uint32_t button;
  int32_t x;
  int32_t y;
};

struct KW_InputEventText {
  KW_InputEventType type;
  uint8_t text[32];
};

struct KW_InputEventKey {
  KW_InputEventType type;
  KW_InputKey key;
};

typedef union KW_InputEvent {
  KW_InputEventType type;
  struct KW_InputEventCursorMotion cursorMotion;
  struct KW_InputEventCursorButton cursorButtonDown;
  struct KW_InputEventCursorButton cursorButtonUp;
  struct KW_InputEventText text;
  struct KW_InputEventKey keyDown;
  struct KW_InputEventKey keyUp;
} KW_InputEvent;

#endif //KW_INPUT_EVENT_H
