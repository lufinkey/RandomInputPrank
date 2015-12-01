
#include "KeyTyper.h"
#include <Windows.h>

namespace rip
{
	INPUT KeyTyper_createKeyboardInput(WORD vKey, DWORD dwFlags, WORD wScan)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = wScan; // hardware scan code for key
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vKey; // virtual-key code for the "a" key
		input.ki.dwFlags = dwFlags; // 0 for key press
		return input;
	}

	void KeyTyper::sendKeyPress(unsigned int key)
	{
		INPUT input = KeyTyper_createKeyboardInput((WORD)key, 0, 0);
		SendInput(1, &input, sizeof(input));
	}

	void KeyTyper::sendKeyRelease(unsigned int key)
	{
		INPUT input = KeyTyper_createKeyboardInput((WORD)key, KEYEVENTF_KEYUP, 0);
		SendInput(1, &input, sizeof(input));
	}

	bool KeyTyper::isKeyPressed(unsigned int key)
	{
		SHORT state = GetKeyState((int)key);
		if(state & 0x8000)
		{
			return true;
		}
		return false;
	}

	bool KeyTyper::isKeyToggled(unsigned int key)
	{
		SHORT state = GetKeyState((int)key);
		if(state & 0x0001)
		{
			return true;
		}
		return false;
	}

	void KeyTyper::typeString(const std::string& str)
	{
		for(size_t str_len=str.length(), i=0; i<str_len; i++)
		{
			typeChar(str.at(i));
		}
	}

	void KeyTyper::typeChar(char c)
	{
		SHORT code = VkKeyScanA(c);
		SHORT vKey = (code & 0x00ff);
		ControlKeyStates states = disableControlStates();

		if(code & 0x0100)
			//shift key
		{
			sendKeyPress(VK_SHIFT);
		}
		if(code & 0x0200)
			//ctrl key
		{
			sendKeyPress(VK_CONTROL);
		}
		if(code & 0x0400)
			//alt key
		{
			sendKeyPress(VK_MENU);
		}


		//send key stroke
		bool keypressed = isKeyPressed(vKey);
		if(keypressed)
		{
			sendKeyRelease(vKey);
		}
		sendKeyPress(vKey);
		if(!keypressed)
		{
			sendKeyRelease(vKey);
		}


		if(code & 0x0100)
			//shift key
		{
			sendKeyRelease(VK_SHIFT);
		}
		if(code & 0x0200)
			//ctrl key
		{
			sendKeyRelease(VK_CONTROL);
		}
		if(code & 0x0400)
			//alt key
		{
			sendKeyRelease(VK_MENU);
		}

		reenableControlStates(states);
	}

	KeyTyper::ControlKeyStates KeyTyper::disableControlStates()
	{
		ControlKeyStates states;
		states.leftShift = isKeyPressed(VK_LSHIFT);
		if(states.leftShift) { sendKeyRelease(VK_LSHIFT); }

		states.rightShift = isKeyPressed(VK_RSHIFT);
		if(states.rightShift) { sendKeyRelease(VK_RSHIFT); }

		states.shift = isKeyPressed(VK_SHIFT);
		if(states.shift) { sendKeyRelease(VK_SHIFT); }

		states.leftCtrl = isKeyPressed(VK_LCONTROL);
		if(states.leftCtrl) { sendKeyRelease(VK_LCONTROL); }

		states.rightCtrl = isKeyPressed(VK_RCONTROL);
		if(states.rightCtrl) { sendKeyRelease(VK_RCONTROL); }

		states.ctrl = isKeyPressed(VK_CONTROL);
		if(states.ctrl) { sendKeyRelease(VK_CONTROL); }

		states.leftAlt = isKeyPressed(VK_LMENU);
		if(states.leftAlt) { sendKeyRelease(VK_LMENU); }

		states.rightAlt = isKeyPressed(VK_RMENU);
		if(states.rightAlt) { sendKeyRelease(VK_RMENU); }

		states.alt = isKeyPressed(VK_MENU);
		if(states.alt) { sendKeyRelease(VK_MENU); }

		return states;
	}

	void KeyTyper::reenableControlStates(KeyTyper::ControlKeyStates states)
	{
		bool leftShift = isKeyPressed(VK_LSHIFT);
		if(states.leftShift && !leftShift) { sendKeyPress(VK_LSHIFT); }

		bool rightShift = isKeyPressed(VK_RSHIFT);
		if(states.rightShift && !rightShift) { sendKeyPress(VK_RSHIFT); }

		bool shift = isKeyPressed(VK_SHIFT);
		if(states.shift && !shift) { sendKeyPress(VK_SHIFT); }

		bool leftCtrl = isKeyPressed(VK_LCONTROL);
		if(states.leftCtrl && !leftCtrl) { sendKeyPress(VK_LCONTROL); }

		bool rightCtrl = isKeyPressed(VK_RCONTROL);
		if(states.rightCtrl && !rightCtrl) { sendKeyPress(VK_RCONTROL); }

		bool ctrl = isKeyPressed(VK_CONTROL);
		if(states.ctrl && !ctrl) { sendKeyPress(VK_CONTROL); }

		bool leftAlt = isKeyPressed(VK_LMENU);
		if(states.leftAlt && !leftAlt) { sendKeyPress(VK_LMENU); }

		bool rightAlt = isKeyPressed(VK_RMENU);
		if(states.rightAlt && !rightAlt) { sendKeyPress(VK_RMENU); }

		bool alt = isKeyPressed(VK_MENU);
		if(states.alt && !alt) { sendKeyPress(VK_MENU); }
	}
}
