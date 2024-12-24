#include "../hdr/Utilities.h"
#include "../hdr/Window.h"
#include <fstream>
#include <sstream>
#include <codecvt>

std::string GetFileData(const std::string& _fileName)
{
	std::ifstream file(_fileName);
	if (file)
	{
		std::stringstream data;
		data << file.rdbuf();
		return data.str();
	}
	else
	{
		std::string msg = "File " + _fileName + " Not Found";
		MessageBoxA(NULL, msg.c_str(), "Vertex Shader Error", 0);
		exit(0);
	}
	return "";
}

std::string ExtractName(const std::string& _location)
{
	std::string texture;
	std::stringstream stream(_location);
	while (std::getline(stream, texture, '/'));
	return texture;
}

void ReplaceString(std::string& _str, std::string _old, std::string _new)
{
	size_t pos = _str.find(_old);
	if (pos != std::string::npos) {
		_str = _str.replace(pos, _old.length(), _new);
	}
}

std::wstring StringToWstring(const std::string& _str)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(_str);
}

