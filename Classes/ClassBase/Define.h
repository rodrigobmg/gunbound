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


#define SKILL_TAG_1 1
#define SKILL_TAG_2 2
#define SKILL_TAG_3 3
#define SKILL_TAG_4 4

#define MOVEKEY "MOVE_MODE"
#define MOVE_CIRCLE_LOCATION "MOVE_CIRCLE_LOCATION"
#define MOVE_CIRCLE_TYPE "MOVE_CIRCLE_TYPE"
#define MOVE_AUTO 1
#define MOVE_MANUAL 2
#define MOVE_CIRCLE 3
#define MOVE_CIRCLE_LEFT 4
#define MOVE_CIRCLE_RIGHT 5
#define MOVE_CIRCLE_TIME 6
#define MOVE_CIRCLE_DISTANCE 7
#define MINICIRCLE_SCALE 1/3;

#define LONGTAP_MOVE_ACTION 11
#define MOVE_DISTANCE 300

#define ROCK_TAG 2
#define TREE_TAG 3

#define WORMHOLE_ROTATE_ACTION 1122
#define BOUND_BORDER_TAG 1202


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