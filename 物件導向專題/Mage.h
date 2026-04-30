#ifndef MAGE_H
#define MAGE_H
#include "Role.h"
class Mage:public Role{
public:
    Mage(string n);
    void useSkill(Role&target, int skillId)override; // 覆寫：三個技能
};
#endif