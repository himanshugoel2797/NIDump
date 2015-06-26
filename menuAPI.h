#ifndef  _MENU_API_NIDUMP_H_
#define  _MENU_API_NIDUMP_H_

#include <psp2/ctrl.h>
#include "draw.h"

typedef void (*MENU_ENTRY_HANDLER)(SceCtrlData);

#define TEXT_COLOR RGBA8(255,255,255,255)
#define SELECTION_COLOR RGBA8(255, 0, 0, 255)
#define TITLE_COLOR RGBA8(0,255,0, 255)


#define SPACE_SIZE 20

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

#endif
