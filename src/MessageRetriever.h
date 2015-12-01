
#pragma once

#include <string>
#include <vector>

namespace rip
{
	class MessageRetriever
	{
	public:
		static std::string getComputerName();
		static std::vector<std::string> retrieveMessages();
	};
}
