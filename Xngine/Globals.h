#pragma once
#include <windows.h>
#include <stdio.h>

#define _FILE_NAME_ (strrchr("\\" __FILE__, '\\') + 1) // obtains the filename from the _FILE path
#define LOG(format, ...) log(_FILE_NAME_, __LINE__, format, __VA_ARGS__);
#define DRAWLOG(title) Draw(title);

void log(const char file[], int line, const char* format, ...);
void Draw(const char* title, bool* p_opened = NULL);

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