#pragma once
#include<string>

std::string GetFileData(const std::string& _fileName);
std::string ExtractName(const std::string& _location);
void ReplaceString(std::string& _str, std::string _old, std::string _new);
std::wstring StringToWstring(const std::string& _str);