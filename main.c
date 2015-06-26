/*
* Copyright (c) 2015 Sergi Granell (xerpi)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include "config.h"
#include "menuAPI.h"
#include "ModuleUtils.h"
#include "draw.h"


int exitStatus = 0;

void exitHandler(SceCtrlData pad)
{
	if(pad.buttons & PSP2_CTRL_CROSS)exitStatus = 1;
}

void dumpNIDs(SceCtrlData pad)
{
	if(pad.buttons & PSP2_CTRL_CROSS){
		DumpAllNIDs(NID_LIST_OUT_PATH);
	}
}

void dumpModules(SceCtrlData pad)
{
	if(pad.buttons & PSP2_CTRL_CROSS){
		DumpAllModules();
	}
}

#define  ENTRY_COUNT 3

void startMenu()
{
	sceCtrlSetSamplingMode(PSP2_CTRL_MODE_DIGITAL);

	MENU_ENTRY entries[ENTRY_COUNT];
	entries[0].handler = &dumpNIDs;
	entries[0].value = "Dump NIDs to file";
	entries[1].handler = &dumpModules;
	entries[1].value = "Dump modules to file";
	entries[ENTRY_COUNT - 1].handler = &exitHandler;
	entries[ENTRY_COUNT - 1].value = "Exit";

	menuInitialize("Vita Tools", entries, ENTRY_COUNT);

	while(!exitStatus)
	{
		menuUpdate();
		menuDraw();
		sceDisplayWaitVblankStart();
	}

}

int _start()
{
	init_video();
	startMenu();
	end_video();

	return 0;
}
