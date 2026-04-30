#ifndef WARRIOR_H
#define WARRIOR_H
#include "Role.h"
class Warrior:public Role{
public:
    Warrior(string n);
    void useSkill(Role&target, int skillId)override; // 覆寫：三個技能
};
#endif