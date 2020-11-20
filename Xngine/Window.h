#ifndef __Window_H__
#define __Window_H__
class Window
{
public:

	Window()
	{
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}
};
#endif // __Window_H__
