#include "UnitDataModel.h"

UnitDataModel* UnitDataModel::_unitInfoData = nullptr;
UnitDataModel* UnitDataModel::getInstance()
{
	if (_unitInfoData)
	{
		_unitInfoData = new (std::nothrow) UnitDataModel();
		_unitInfoData->init();
	}
	
	return _unitInfoData;	
}

bool UnitDataModel::init()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GET DATA FROM DATABASE
////////////////////////////////////////////////////////////////////////////////////////////////

#define DATABASE_NAME "gunbound.db3"

vector<UnitInfo> UnitDataModel::getDataUnitFromDatabase()
{
	vector<UnitInfo> allUnitInfo;

	log("GET DATA FROM DATABASE");
	// open database
	sqlite3* db = SqlUtils::openData(DATABASE_NAME);
	// select using query
	string querySQL = "select * from unit";
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
		UnitInfo temp;
		temp.id = DataUtils::convertStringToFloat(item[0].c_str());
		temp.name = item[1];
		temp.hp = DataUtils::convertStringToFloat(item[2].c_str());
		temp.hp_restore = DataUtils::convertStringToFloat(item[3].c_str());
		temp.mp = DataUtils::convertStringToFloat(item[4].c_str());
		temp.mp_restore = DataUtils::convertStringToFloat(item[5].c_str());
		temp.attack_dame = DataUtils::convertStringToFloat(item[6].c_str());
		temp.defence = DataUtils::convertStringToFloat(item[7].c_str());
		temp.attack_sight = DataUtils::convertStringToFloat(item[8].c_str());
		temp.move_speech = DataUtils::convertStringToFloat(item[9].c_str());
		temp.attribute = DataUtils::convertStringToFloat(item[10].c_str());
		temp.type = DataUtils::convertStringToFloat(item[11].c_str());
		temp.imagePath = item[12].c_str();
		temp.attack_delay = DataUtils::convertStringToFloat(item[13].c_str());

		// Add data to array
		allUnitInfo.push_back(temp);

		///TEST_LOG
		for (int i = 0; i < item.size() ; i++)
		{
		log("%s", item[i].c_str());
		}
		log("==========================================");
		

	}


	return allUnitInfo;

}

////////////////////////////////////////////////////////////////////////////////////////////////
// GET DATA UNIT BY ID
////////////////////////////////////////////////////////////////////////////////////////////////

UnitInfo UnitDataModel::getDataUnitById(int unitId)
{
	vector<UnitInfo> allUnitInfo = this->getDataUnitFromDatabase();

	for (int i = 0 ; i < allUnitInfo.size() ; i++)
	{
		if (allUnitInfo[i].id == unitId)
		{
			return allUnitInfo[i];
		}
	}
}