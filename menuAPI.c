#include "menuAPI.h"

SceCtrlData pad;
MENU_ENTRY *m_entries;
int m_entryCount;
int m_currentEntry;

char *m_title;

char currentFrame;

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
}

void menuUpdate()
{
  currentFrame++;
  if(currentFrame == 20){
    sceCtrlPeekBufferPositive(0, &pad, 1);
    currentFrame = 0;
  }else{
    pad.buttons = 0;
  }
  if(pad.buttons & PSP2_CTRL_UP){
    m_currentEntry--;
  }
  if(pad.buttons & PSP2_CTRL_DOWN)
  {
    m_currentEntry++;
  }

  if(m_currentEntry >= m_entryCount)m_currentEntry = m_entryCount - 1;
  if(m_currentEntry < 0)m_currentEntry = 0;

  if( (pad.buttons & PSP2_CTRL_SQUARE | pad.buttons & PSP2_CTRL_CIRCLE | pad.buttons & PSP2_CTRL_CROSS) && m_entries[m_currentEntry].handler != NULL)
  {
    m_entries[m_currentEntry].handler(pad);
  }
}

void menuDraw()
{
  clear_screen();
  font_draw_string(10, 10, TITLE_COLOR, m_title);

  for(int i = 0; i < m_entryCount; i++)
  {
    if(m_currentEntry == i)
    {
      font_draw_string(20, 10 + (1 + i) * SPACE_SIZE, SELECTION_COLOR, m_entries[i].value );

    }else{
      font_draw_string(20, 10 + (1 + i) * SPACE_SIZE, TEXT_COLOR, m_entries[i].value );
    }
  }
  swap_buffers();
}

void menuSetSelectedEntry(int index)
{
  m_currentEntry = index;
}
