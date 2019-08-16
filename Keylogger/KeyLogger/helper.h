#ifndef _HELPER_H_
#define _HELPER_H_

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

namespace Helper
{
	template <class T>

	std::string toString(const T &);

	struct DateTime
	{
		int day, month, year, hour, minute, second;

		DateTime()
		{
			time_t sec;
			time(&sec);

			struct tm *info = localtime(&sec);

			day = info->tm_mday;
			month = info->tm_mon + 1;
			year = 1900 + info->tm_year;
			minute = info->tm_min;
			hour = info->tm_hour;
			second = info->tm_sec;
		}

		DateTime(int day, int month, int year, int hour, int minute, int second) :
			day(day), month(month), year(year), hour(hour), 
			minute(minute), second(second) {}

		DateTime(int day, int month, int year) : 
			day(day), month(month), year(year), hour(0), minute(0), second(0) {}

		DateTime Now() const
		{
			return DateTime();
		}

		// Gets Data in string format
		std::string getDateString() const
		{
			return std::string(day < 10 ? "0" : "") + toString(day) +
				   std::string(month < 10 ? ".0" : ".") + toString(month) + "." +
				   toString(year);
		}

		// Gets Time in string format
		std::string getTimeString(const std::string &sep = ":") const
		{
			return std::string(hour < 10 ? "0" : "") + toString(hour) + sep +
				   std::string(minute < 10 ? "0" : "") + toString(minute) + sep +
				   std::string(second < 10 ? sep : "") + toString(second);
		}

		// Gets both Data and Time in string format
		std::string getDateTimeString(const std::string &sep = ":") const
		{
			return getDateString() + " " + getTimeString(sep);
		}
	};

	template <class T>

	// Converts data to a string type
	std::string toString(const T &data)
	{
		std::ostringstream outStr;
		outStr << data;

		return outStr.str();
	}

	// Writes data to text file
	void writeLogFile(const std::string &logStr)
	{
		std::ofstream outFile("LogFile.txt", std::ios::app);
		outFile << "[" << Helper::DateTime().getDateTimeString() << "]" <<
			"\n" << logStr << std::endl << "\n";

		outFile.close();
	}
}

#endif // _HELPER_H_
