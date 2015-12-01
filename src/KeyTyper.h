
#pragma once

#include <string>

namespace rip
{
	class KeyTyper
	{
	public:
		static void sendKeyPress(unsigned int key);
		static void sendKeyRelease(unsigned int key);

		static bool isKeyPressed(unsigned int key);
		static bool isKeyToggled(unsigned int key);

		static void typeString(const std::string& string);
		static void typeChar(char c);

	private:
		typedef struct ControlKeyStates
		{
			bool leftShift;
			bool rightShift;
			bool shift;
			bool leftCtrl;
			bool rightCtrl;
			bool ctrl;
			bool leftAlt;
			bool rightAlt;
			bool alt;
		} ControlKeyStates;

		static ControlKeyStates disableControlStates();
		static void reenableControlStates(ControlKeyStates);
	};
}
