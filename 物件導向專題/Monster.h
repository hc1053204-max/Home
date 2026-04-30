#ifndef MONSTER_H
#define MONSTER_H
#include "role.h"
class Monster:public Role{
public:
    Monster(string n,int h,int a);
    void useSkill(Role&target, int skillId)override;
};
#endif