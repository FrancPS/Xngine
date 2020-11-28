#ifndef __WindowConsole_H__
#define __WindowConsole_H__

#include "Window.h"
class WindowConsole : public Window
{
public:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

	
	//WindowConsole();
	void AddLog(const char* fmt, ...);
	bool Draw();

	void Clear() { Buf.clear(); LineOffsets.clear(); }
	bool CleanUp() { Buf.clear(); LineOffsets.clear(); return true; }
};
#endif // __WindowConsole_H__
