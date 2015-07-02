/* Class lay tat ca du lieu tu bang Unit trong database */
#ifndef _SKILLDATAMODEL_H_
#define _SKILLDATAMODEL_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"
#include "ClassBase/LayerBase.h"
#include "Database/DataUtils.h"
#include "Database/SqlUtils.h"


USING_NS_CC;
using namespace std;

class SkillDataModel : public Ref
{
public:
	static SkillDataModel* getInstance();
	virtual bool init();

	vector<SkillInfo> getDataSkillFromDatabase();
	SkillInfo getDataSkillById(int skillId);

	vector<SkillInfo> getDataSkillByUnitId(int unitId);

private:

	static SkillDataModel* _skillInfoData;

	// Mang duoc tao ra de chua list skill duoc unit lua chon
	// Se duoc su dung tai SkillSelectScene
	CC_SYNTHESIZE(vector<SkillInfo>, _allSkillSelected, AllSkillSelected);



};


#endif // _SKILLDATAMODEL_H_