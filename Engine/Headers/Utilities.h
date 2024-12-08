#pragma once
#include<string>

std::string GetFileData(std::string _fileName);
std::string ExtractName(std::string _location);
void ReplaceString(std::string& _str, std::string _old, std::string _new);