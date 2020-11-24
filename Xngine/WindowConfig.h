#ifndef __WindowConfig_H__
#define __WindowConfig_H__

#include "Window.h"
#include <vector>
#include <string>

class WindowConfig : public Window
{
private:
	float fpsNow;
	std::vector<float> fps;
	int histNumElements;

	// Should not go here!
	int vIL;
	int vILU;
	int vILUT;

public:

	WindowConfig();
	bool Draw();
	//bool CleanUp();

private:

	const char* PrintCapacitors(std::string &caps);
};
#endif // __WindowConfig_H__
