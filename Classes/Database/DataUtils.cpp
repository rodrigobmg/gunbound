/*
Tham khao: http://forums.codeguru.com/showthread.php?231054-C-String-How-to-convert-a-string-into-a-numeric-type
*/
#include "Database/DataUtils.h"

std::string DataUtils::convertNumberToString(float number)
{
	std::stringstream strNum;
	strNum << number << std::endl;

	return strNum.str().c_str();
}

float DataUtils::convertStringToFloat(std::string str)
{
	std::istringstream stream(str);
	float num;
	stream >> num;

	return num;
}