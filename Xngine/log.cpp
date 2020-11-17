#pragma once
#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"
#include "imgui.h"
#include <vector>

std::vector<const char*> Buf;
bool ScrollToBottom;
bool imguiInitialised = false;

void Clear() { Buf.clear(); }

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	Buf.push_back(tmp_string2);
	OutputDebugString(tmp_string2);

    ScrollToBottom = true;

	if (imguiInitialised) {
		Draw("CONSOLE");
	}


}

void Draw(const char* title, bool* p_opened) // this should go inside the log function, but main logs give error
{
	ImGui::Begin(title, p_opened);
	for (unsigned int i = 0; i < Buf.size(); ++i)
		ImGui::TextUnformatted(Buf[i]);
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}
