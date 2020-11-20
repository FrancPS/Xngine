#ifndef __Globals_H__
#define __Globals_H__

#include <windows.h>
#include <stdio.h>

#define _FILE_NAME_ (strrchr("\\" __FILE__, '\\') + 1) // obtains the filename from the _FILE path
#define LOG(format, ...) log(_FILE_NAME_, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define VSYNC true
#define RESIZABLE true
#define TITLE "Xngine"
#define DEGTORAD 3.14159/180

#endif // __Globals_H__