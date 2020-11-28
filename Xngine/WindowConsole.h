#ifndef __WindowConsole_H__
#define __WindowConsole_H__

#include "Window.h"
class WindowConsole : public Window
{
public:
	ImGuiTextBuffer     Buf;
	bool                ScrollToBottom;

	
	//WindowConsole();
	void AddLog(const char* fmt, ...);
	bool Draw();

	void Clear() { Buf.clear(); }
	bool CleanUp() { Buf.clear(); }
};
#endif // __WindowConsole_H__
