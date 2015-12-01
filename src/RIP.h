
#pragma once

#include <string>
#include <vector>

namespace rip
{
	std::string urlencode(const std::string& value);
	std::string string_replaceall(const std::string& str, const std::string& find, const std::string& replace);
	
	std::string getLocalAppDataPath();
	
	void init();
	void reloadSettings();
	
	namespace settings
	{
		extern bool random;
		extern bool retrieve;
		extern double random_chance;
		extern std::string api_endpoint;
		extern std::vector<std::string> phrases;
		extern long long refresh_interval;
	}
}
