#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;


#define JAPANESE_FONT_1_HEAVY "fonts/AozoraMinchoHeavy.ttf"
#define JAPANESE_FONT_1_BOLD "fonts/AozoraMincho-bold.ttf"
#define JAPANESE_FONT_1_REGULAR "fonts/AozoraMinchoRegular.ttf"

#define PI 3.1415926535897932385f
#define DEG_RAD (PI / 180)
#define RAD_DEG (180 / PI)


struct UserInfo
{
	std::string _name;
};

/* UnitInfo la struct luu tru thong tin character info cua old version */
struct UnitInfoOld
{
	std::string _name;
	int _unitId;
	std::string _imagePath;
	int _hp;
	int _attack;
	int _defence;
};

/* UnitInfo la struct luu tru thong tin character info cua new version */
struct UnitInfo{
	int id;
	std::string name;
	int hp; // health point: Chi so mau cua nhan vat
	int hp_restore;
	int mp; // mana point: Diem phep thuat
	int mp_restore;
	int attack_dame;
	int defence;
	int attack_sight;
	int move_speech;
	int attribute;
	int type;
	int attack_delay;
	std::string imagePath;
};



struct SkillInfo
{
	string name;
	int id;
	int aoe;
	int targetType;
	int mp_cost;
	int cooldown;
	int skillType;
	int dameType;
	int dameValue;
	int duration; // khoang thoi gian
	string effect;
	string effectPath;
	string imagePath;
	int helpType;
	int areaType;
};


#endif // _DEFINE_H_