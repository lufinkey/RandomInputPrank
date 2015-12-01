
#pragma once

#include <string>
#include <vector>

namespace rip
{
	std::string urlencode(const std::string& value);
	std::string string_replaceall(const std::string& str, const std::string& find, const std::string& replace);
	
	class MessageRetriever
	{
	public:
		static std::string getComputerName();
		
		static std::vector<std::string> retrieveMessages();
	};
}
