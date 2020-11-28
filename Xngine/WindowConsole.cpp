#include "WindowConsole.h"
#include "imgui.h"


    

void WindowConsole::AddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    ScrollToBottom = true;
}

bool WindowConsole::Draw()
{
    ImGui::Begin("CONSOLE");
    ImGui::TextUnformatted(Buf.begin());
    if (ScrollToBottom)
        ImGui::SetScrollHere(1.0f);
    ScrollToBottom = false;
    ImGui::End();

    return true;
}
