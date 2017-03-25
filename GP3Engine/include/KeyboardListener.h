#ifndef _KEYBOARDLISTENER_H
#define _KEYBOARDLISTENER_H

#include "Common.h"

class IkeyboardListener
{
public:
	virtual ~IkeyboardListener(){}
	virtual void onKeyDown(SDL_Keycode key) = 0;
	virtual void onkeyUp(SDL_Keycode key) = 0;
	virtual void mouseMove(SDL_MouseMotionEvent motion) = 0;
	virtual void mouseDown(SDL_MouseButtonEvent button) = 0;
	virtual void mouseUp(SDL_MouseButtonEvent button) = 0;
	virtual void joyMove(SDL_ControllerAxisEvent motion) = 0;
	virtual void joyButtonDown(SDL_ControllerButtonEvent button) = 0;
	virtual void joyButtonUp(SDL_ControllerButtonEvent button) = 0;


private:

};

#endif
