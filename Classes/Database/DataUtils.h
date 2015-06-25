#ifndef _DATA_UTILS_H_
#define _DATA_UTILS_H_

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include <stdlib.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class DataUtils
{
public:
	/*Ham chuyen tu number ve string
	@number float: Dau vao
	Return std::string
	*/
	static std::string convertNumberToString(float number);
	/*Ham chuyen tu string sang number
	@str std::string: Dau vao
	Return float
	*/
	static float convertStringToFloat(const std::string str);
private:

};

#endif //_DATA_UTILS_H_