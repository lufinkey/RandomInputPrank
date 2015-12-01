
#include "KeyTyper.h"
#include "MessageRetriever.h"
#include <iostream>
#include <thread>

int main()
{
	std::cout << "Computer Name: " << rip::MessageRetriever::getComputerName() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while(true)
	{
		std::cout << "checking messages" << std::endl;
		std::vector<std::string> msgs = rip::MessageRetriever::retrieveMessages();
		for(size_t msgs_size=msgs.size(), i=0; i<msgs_size; i++)
		{
			std::cout << "recieved message: " << msgs.at(i) << std::endl;
			rip::KeyTyper::typeString(msgs.at(i));
		}
		if(msgs.size()==0)
		{
			std::cout << "no new messages" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
    return 0;
}
