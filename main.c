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
#include <psp2/kernel/threadmgr.h>
#include <time.h>
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
		menuStatusAppendBufferData("Dumping NIDs...\n");
		SceUID thid = sceKernelCreateThread("DumpAllNIDs", DumpAllNIDs, 0x10000100, 0x10000, 0, 0, NULL);
		sceKernelStartThread(thid, strlen(NID_LIST_OUT_PATH), NID_LIST_OUT_PATH);
	}
}

void dumpModules(SceCtrlData pad)
{
	if(pad.buttons & PSP2_CTRL_CROSS){
		menuStatusAppendBufferData("Dumping Modules...");
		SceUID thid = sceKernelCreateThread("DumpAllModules", DumpAllModules, 0x10000100, 0x10000, 0, 0, NULL);
		sceKernelStartThread(thid, 0, NULL);
	}
}

#define  ENTRY_COUNT 4

void startMenu()
{
	sceCtrlSetSamplingMode(PSP2_CTRL_MODE_DIGITAL);

	MENU_ENTRY entries[ENTRY_COUNT];
	entries[0].handler = &dumpNIDs;
	entries[0].value = "Dump NIDs to file";
	entries[1].handler = &dumpModules;
	entries[1].value = "Dump modules to file";
	entries[2].handler = &dumpModules;
	entries[2].value = "File Manager";
	entries[ENTRY_COUNT - 1].handler = &exitHandler;
	entries[ENTRY_COUNT - 1].value = "Exit";

	menuInitialize("Vita Tools by hgoel0974", entries, ENTRY_COUNT);

	time_t timeVal;
	struct tm *timeinfo;
	char timeStr[40];

	while(!exitStatus)
	{
	  clear_screen();
		menuUpdate();
		menuDraw();

		time(&timeVal);
		timeinfo = localtime(&timeVal);

		strftime(timeStr, 40, "%r %D", timeinfo);
		font_draw_string(600, 10, TEXT_COLOR, timeStr);

		swap_buffers();
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
