
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
		static bool random = true;
		static bool retrieve = true;
		static double random_chance = 0.3;
		static std::string api_endpoint = "http://fojam.users.sourceforge.net/api/inputprank";
		static std::vector<std::string> phrases = std::vector<std::string>();
		static long long refresh_interval = 300000;
	}
}
