#include "UnitDataModel.h"

UnitDataModel* UnitDataModel::_unitInfoData = nullptr;
UnitDataModel* UnitDataModel::getInstance()
{
	if (_unitInfoData)
	{
		_unitInfoData = new (std::nothrow) UnitDataModel();
		return _unitInfoData;
	}
	CC_SAFE_DELETE(_unitInfoData);
	return nullptr;

	
}