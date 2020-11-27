#ifndef __WindowConfig_H__
#define __WindowConfig_H__

#include "Window.h"
#include <vector>
#include <string>

class WindowConfig : public Window
{
private:
	float fpsNow = 0.f;
	std::vector<float> fps;
	int histNumElements	= 50;

	// Should not go here!
	int vIL		= 0;
	int vILU	= 0;
	int vILUT	= 0;

public:

	WindowConfig();
	bool Draw();

private:

	const char* PrintCapacitors(std::string &caps);
};
#endif // __WindowConfig_H__
