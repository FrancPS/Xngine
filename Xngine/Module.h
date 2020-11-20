#ifndef __Module_H__
#define __Module_H__

#include "Globals.h"

class Application;

struct Event;

class Module
{
public:

	Module()
	{
	}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void ReceiveEvent(const Event& event)
	{ }
};
#endif // __Module_H__
