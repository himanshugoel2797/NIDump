#ifndef  _MENU_API_NIDUMP_H_
#define  _MENU_API_NIDUMP_H_

#include <psp2/ctrl.h>
#include "draw.h"
#include "utils.h"

typedef void (*MENU_ENTRY_HANDLER)(SceCtrlData);

#define STATUS_BUFFER_LENGTH 1024

#define STATUS_BUFFER_HEIGHT 100
#define STATUS_BUFFER_POS (SCREEN_H - STATUS_BUFFER_HEIGHT)

#define TEXT_COLOR RGBA8(255,255,255,255)
#define SELECTION_COLOR RGBA8(255, 0, 0, 255)
#define TITLE_COLOR RGBA8(0,255,0, 255)

#define BUTTON_PRESSED(a, b, c) ((b.buttons & c))
#define SPACE_SIZE 20
#define SELECTED_SPACE_SIZE 30

typedef struct {
  MENU_ENTRY_HANDLER handler;
  char *value;
} MENU_ENTRY;

//TODO specify possible options and their handlers in menuInitialize

void menuInitialize(char *title, MENU_ENTRY *entries, int entryCount);
void menuUpdate();
void menuDraw();
void menuSetSelectedEntry(int index);
SceCtrlData menuGetCtrlData();

void menuStatusSetBufferData(const char *data);
void menuStatusAppendBufferData(const char *data);
void menuStatusClearBufferData();

#endif
