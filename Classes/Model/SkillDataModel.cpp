#include "SkillDataModel.h"

SkillDataModel* SkillDataModel::_skillInfoData = nullptr;
SkillDataModel* SkillDataModel::getInstance()
{
	if (!_skillInfoData)
	{
		_skillInfoData = new (std::nothrow) SkillDataModel();
		_skillInfoData->init();
	}

	return _skillInfoData;
}

bool SkillDataModel::init()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GET DATA FROM DATABASE
////////////////////////////////////////////////////////////////////////////////////////////////

#define DATABASE_NAME "gunbound.db3"
vector<SkillInfo> SkillDataModel::getDataSkillFromDatabase()
{
	vector<SkillInfo> allSkillInfo;

	log("GET DATA FROM DATABASE");
	// open database
	sqlite3* db = SqlUtils::openData(DATABASE_NAME);
	// select using query
	string querySQL = "select * from skill";
	vector<vector<string>> arrayInfo = SqlUtils::runQuery(db, querySQL.c_str());

	// Get data after convert
	/*
	vector<vector<string>>::iterator inter_ii;
	vector<string>::iterator inter_jj;

	for (inter_ii = arrayInfo.begin(); inter_ii != arrayInfo.end(); inter_ii++)
	{
	for (inter_jj = (*inter_ii).begin(); inter_jj != (*inter_ii).end(); inter_jj++)
	{
	log("%s", (*inter_jj).c_str());
	}

	log("==========================================");
	}
	*/


	for (auto &item : arrayInfo)
	{
		SkillInfo temp;
		temp.id = DataUtils::convertStringToFloat(item[0].c_str());
		temp.name = item[1];
		temp.aoe = DataUtils::convertStringToFloat(item[2].c_str());
		temp.targetType = DataUtils::convertStringToFloat(item[3].c_str());
		temp.mp_cost = DataUtils::convertStringToFloat(item[4].c_str());
		temp.cooldown = DataUtils::convertStringToFloat(item[5].c_str());
		temp.skillType = DataUtils::convertStringToFloat(item[6].c_str());
		temp.dameType = DataUtils::convertStringToFloat(item[7].c_str());
		temp.dameValue = DataUtils::convertStringToFloat(item[8].c_str());
		temp.duration = DataUtils::convertStringToFloat(item[9].c_str());
		temp.effect = item[10];
		temp.effectPath = item[11];
		temp.imagePath = item[12];
		temp.helpType = DataUtils::convertStringToFloat(item[13].c_str());
		temp.areaType = DataUtils::convertStringToFloat(item[14].c_str());

		// Add data to array
		allSkillInfo.push_back(temp);

		///TEST_LOG
		for (int i = 0; i < item.size(); i++)
		{
			log("%s", item[i].c_str());
		}
		log("==========================================");


	}

	return allSkillInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GET DATA UNIT BY ID
////////////////////////////////////////////////////////////////////////////////////////////////

/* Lay tu bang Skill ra 1 truong theo skillId */
SkillInfo SkillDataModel::getDataSkillById(int skillId)
{
	vector<SkillInfo> allSkillInfo = this->getDataSkillFromDatabase();

	for (int i = 0; i < allSkillInfo.size(); i++)
	{
		if (allSkillInfo[i].id == skillId)
		{
			return allSkillInfo[i];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GET UNIT SKILL BY UNIT ID
////////////////////////////////////////////////////////////////////////////////////////////////

/* Lay ra 1 phan tu skill trong bang skill theo thong tin tu bang unit_skill va unit */
vector<SkillInfo> SkillDataModel::getDataSkillBuUnitId(int unitId)
{
	vector<SkillInfo> skillList;

	// Thuc hien truy van bang cach join 3 bang unit , skill va unit_skill
	string sql = "SELECT skill.* FROM unit_skill JOIN unit ON unit.id = unit_skill.unitId JOIN skill ON skill.id = unit_skill.skillId WHERE unit.id = ";
	sql.append(DataUtils::convertNumberToString(unitId));

	sqlite3 *data = SqlUtils::openData(DATABASE_NAME);

	vector<vector<string>> arrayInfo = SqlUtils::runQuery(data, sql.c_str());
	for (auto &item : arrayInfo)
	{
		SkillInfo temp;
		temp.id = DataUtils::convertStringToFloat(item[0].c_str());
		temp.name = item[1];
		temp.aoe = DataUtils::convertStringToFloat(item[2].c_str());
		temp.targetType = DataUtils::convertStringToFloat(item[3].c_str());
		temp.mp_cost = DataUtils::convertStringToFloat(item[4].c_str());
		temp.cooldown = DataUtils::convertStringToFloat(item[5].c_str());
		temp.skillType = DataUtils::convertStringToFloat(item[6].c_str());
		temp.dameType = DataUtils::convertStringToFloat(item[7].c_str());
		temp.dameValue = DataUtils::convertStringToFloat(item[8].c_str());
		temp.duration = DataUtils::convertStringToFloat(item[9].c_str());
		temp.effect = item[10];
		temp.effectPath = item[11];
		temp.imagePath = item[12];
		temp.helpType = DataUtils::convertStringToFloat(item[13].c_str());
		temp.areaType = DataUtils::convertStringToFloat(item[14].c_str());

		// Add data to array
		skillList.push_back(temp);

		///TEST_LOG
		for (int i = 0; i < item.size(); i++)
		{
			log("%s", item[i].c_str());
		}
		log("==========================================");
	}

	return skillList;

}