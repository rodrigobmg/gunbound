#ifndef _UNITDATAMODEL_H_
#define _UNITDATAMODEL_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"
#include "ClassBase/LayerBase.h"
#include "Database/DataUtils.h"
#include "Database/SqlUtils.h"

USING_NS_CC;
using namespace std;

class UnitDataModel : public Ref
{
public:
	static UnitDataModel* getInstance();

private:
	static UnitDataModel* _unitInfoData;

};



#endif //  _UNITDATAMODEL_H_