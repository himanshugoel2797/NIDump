#include "menuAPI.h"

SceCtrlData pad, prevPad;
MENU_ENTRY *m_entries;
int m_entryCount;
int m_currentEntry;

char *m_title;
char currentFrame;

char statusData[2*STATUS_BUFFER_LENGTH];
int statusBufferPosition;

SceCtrlData menuGetCtrlData()
{
  return pad;
}

void menuInitialize(char *title, MENU_ENTRY *entries, int entryCount)
{
  m_entries = entries;
  m_entryCount = entryCount;

  m_title = title;

  m_currentEntry = 0;
  currentFrame = 0;
  sceCtrlPeekBufferPositive(0, &prevPad, 1);
  menuStatusClearBufferData();
}

void menuUpdate()
{
  currentFrame++;
  if(currentFrame == 10){
    sceCtrlPeekBufferPositive(0, &pad, 1);
    if(pad.buttons == prevPad.buttons)pad.buttons = 0;
    prevPad.buttons = pad.buttons;
    currentFrame = 0;
  }else{
    pad.buttons = 0;
  }
  if(BUTTON_PRESSED(prevPad, pad, PSP2_CTRL_UP)){
    m_currentEntry--;
  }
  if(pad.buttons & PSP2_CTRL_DOWN)
  {
    m_currentEntry++;
  }

  if(m_currentEntry >= m_entryCount)m_currentEntry = m_entryCount - 1;
  if(m_currentEntry < 0)m_currentEntry = 0;

  if( (BUTTON_PRESSED(prevPad, pad, PSP2_CTRL_CROSS) | BUTTON_PRESSED(prevPad, pad, PSP2_CTRL_SQUARE) | BUTTON_PRESSED(prevPad, pad, PSP2_CTRL_CIRCLE)) && m_entries[m_currentEntry].handler != NULL)
  {
    m_entries[m_currentEntry].handler(pad);
  }
}

void menuDraw()
{
  font_draw_string(10, 10, TITLE_COLOR, m_title);

  for(int i = 0; i < m_entryCount; i++)
  {
    if(m_currentEntry == i)
    {
      font_draw_string(SELECTED_SPACE_SIZE, 10 + (1 + i) * SPACE_SIZE, SELECTION_COLOR, m_entries[i].value );

    }else{
      font_draw_string(SPACE_SIZE, 10 + (1 + i) * SPACE_SIZE, TEXT_COLOR, m_entries[i].value );
    }
  }
  font_draw_string(SPACE_SIZE, STATUS_BUFFER_POS, TEXT_COLOR, statusData);
}

void menuSetSelectedEntry(int index)
{
  m_currentEntry = index;
}

void menuStatusSetBufferData(const char *data)
{
  strcpy(statusData, data);
  statusBufferPosition = 0;
}

void menuStatusAppendBufferData(const char *data)
{
  int len = strlen(data);
  if(statusBufferPosition + len > STATUS_BUFFER_LENGTH){
    memmove(statusData, statusData + statusBufferPosition + ((statusBufferPosition+ len) - STATUS_BUFFER_LENGTH), STATUS_BUFFER_LENGTH - statusBufferPosition);
    statusBufferPosition = STATUS_BUFFER_LENGTH - len;
  }
  memcpy(statusData + statusBufferPosition, data, strlen(data));
  statusBufferPosition += strlen(data);
}

void menuStatusClearBufferData()
{
  memset(statusData, 0, 2 * STATUS_BUFFER_LENGTH);
}
