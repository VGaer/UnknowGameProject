#pragma once
/***
	gb2312��UTF-8��ת
***/
#include <string>
#include <vector>
#include <codecvt>

std::string gb2312_to_utf8(std::string const &strGb2312);

std::string utf8_to_gb2312(std::string const &strUtf8);
