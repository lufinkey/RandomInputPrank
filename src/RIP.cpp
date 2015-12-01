
#define _CRT_NONSTDC_NO_DEPRECATE

#include "RIP.h"
#include "posixtime.h"
#include <INIReader.h>
#include <rapidjson/document.h>
#include <iomanip>
#include <sstream>
#include <ShlObj.h>
#include <sys/stat.h>
#include <direct.h>

namespace rip
{
	std::string urlencode(const std::string& value)
	{
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;
		
		for(std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
		{
			std::string::value_type c = (*i);
			
			// Keep alphanumeric and other accepted characters intact
			if(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
			{
				escaped << c;
			}
			// Any other characters are percent-encoded
			else
			{
				escaped << '%' << std::setw(2) << int((unsigned char)c);
			}
		}
		
		return escaped.str();
	}
	
	std::string string_replaceall(const std::string& str, const std::string& find, const std::string& replace)
	{
		std::string str_new = str;
		std::string::size_type n = 0;
		while((n=str_new.find(find, n)) != std::string::npos)
		{
			str_new.replace(n, find.length(), replace);
			n += replace.size();
		}
		return str_new;
	}
	
	std::string getLocalAppDataPath()
	{
		char path_buffer[1024];
		path_buffer[0]='\0';
		SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path_buffer);
		return path_buffer;
	}
	
	void init()
	{
		timeval srand_time;
		gettimeofday(&srand_time, nullptr);
		std::srand((unsigned int)srand_time.tv_usec);
		
		std::string userdata_path = getLocalAppDataPath();
		
		userdata_path += "/lufinkey";
		mkdir(userdata_path.c_str());
		
		userdata_path += "/rip";
		mkdir(userdata_path.c_str());
		
		reloadSettings();
	}
	
	void reloadSettings()
	{
		std::string userdata_path = getLocalAppDataPath();
		
		INIReader reader(userdata_path+"/lufinkey/rip/settings.ini");
		if(reader.ParseError()==0)
		{
			settings::random = reader.GetBoolean("", "random", true);
			settings::retrieve = reader.GetBoolean("", "retrieve", true);
			settings::random_chance = reader.GetReal("", "random_chance", 0.3);
			settings::api_endpoint = reader.Get("", "api_endpoint", settings::api_endpoint);
			std::string phrases_str = reader.Get("", "phrases", "[]");
			settings::phrases.clear();
			rapidjson::Document doc;
			if(!doc.Parse<0>(phrases_str.c_str()).HasParseError() && doc.IsArray())
			{
				for(rapidjson::SizeType doc_size=doc.Size(), i=0; i<doc_size; i++)
				{
					if(doc[i].IsString())
					{
						settings::phrases.push_back(doc[i].GetString());
					}
				}
			}
			settings::refresh_interval = reader.GetInteger("", "refresh_interval", 300000);
			if(settings::refresh_interval < 1000)
			{
				settings::refresh_interval = 1000;
			}
		}
	}
}
