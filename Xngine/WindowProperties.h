#ifndef __WindowProperties_H__
#define __WindowProperties_H__

#include "Window.h"
#include "ResourceModel.h"

class WindowProperties : public Window
{
public:
	ResourceModel* model;

public:
	
	WindowProperties();
	bool Draw();
	//bool CleanUp();
};
#endif // __WindowProperties_H__
