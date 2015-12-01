
#include "MessageRetriever.h"
#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <MsXml6.h>
#include <comutil.h>
#include <iomanip>
#include <sstream>

#include <rapidjson/document.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "msxml6.lib")

namespace rip
{
	#define API_ENDPOINT "http://fojam.users.sourceforge.net/api/inputprank"

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
	
	std::string MessageRetriever::getComputerName()
	{
		char buffer[1024];
		DWORD size = sizeof(buffer);
		BOOL result = GetComputerNameA(buffer, &size);
		if(result==0)
		{
			DWORD errorno = GetLastError();
			DWORD msg_size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 1024, NULL);
			buffer[msg_size] = '\0';
			if(msg_size!=0)
			{
				std::cerr << buffer << std::endl;
			}
			return "";
		}
		buffer[size] = '\0';
		return buffer;
	}
	
	std::vector<std::string> MessageRetriever::retrieveMessages()
	{
		CoInitialize(NULL);
		CComPtr<IXMLHTTPRequest> request;
		HRESULT hr = 0;
		hr = request.CoCreateInstance(CLSID_XMLHTTP60);
		if(!SUCCEEDED(hr))
		{
			request.Release();
			CoUninitialize();
			return std::vector<std::string>();
		}
		request->open(_bstr_t("POST"), _bstr_t(API_ENDPOINT "/retrievemsgs.php"), _variant_t(VARIANT_FALSE), _variant_t(""), _variant_t(""));
		request->setRequestHeader(_bstr_t("Content-Type"), _bstr_t("application/x-www-form-urlencoded"));
		std::string cpu_name = string_replaceall(urlencode(getComputerName()), "%20", "+");
		request->send(_variant_t(("cpu_name="+cpu_name).c_str()));
		
		long status = 0;
		hr = request->get_status(&status);
		if(status!=200)
		{
			request.Release();
			CoUninitialize();
			return std::vector<std::string>();
		}
		BSTR responseText_bstr = nullptr;
		request->get_responseText(&responseText_bstr);
		char* responseText = _com_util::ConvertBSTRToString(responseText_bstr);
		
		rapidjson::Document doc;
		if(doc.Parse<0>(responseText).HasParseError())
		{
			delete[] responseText;
			request.Release();
			CoUninitialize();
			return std::vector<std::string>();
		}
		
		if(!doc.IsArray())
		{
			delete[] responseText;
			request.Release();
			CoUninitialize();
			return std::vector<std::string>();
		}
		
		std::vector<std::string> msgs;
		for(rapidjson::SizeType arr_size=doc.Size(), i=0; i<arr_size; i++)
		{
			msgs.push_back(doc[i].GetString());
		}

		delete[] responseText;
		request.Release();
		CoUninitialize();
		return msgs;
	}
}
