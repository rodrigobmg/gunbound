/* Class nay se duoc su dung de connect server va lay databse tu server ve local */
#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include "cocos2d.h"
#include <iostream>
#include <fstream>
/* Thu vien can khai bao de su dung lop HttpRequest */
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

#include "ClassBase/Define.h"

USING_NS_CC;
using namespace network;

class DataModel
{
public:

	static DataModel* getInstance();
	virtual bool init();

	void createDatabase();
	void serverCallback(HttpClient* client, HttpResponse* response);

private:
	static DataModel* _dataModel;

};

#endif //_DATAMODEL_H_