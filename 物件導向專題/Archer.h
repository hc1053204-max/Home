#ifndef ARCHER_H
#define ARCHER_H
#include "Role.h"
class Archer:public Role{
public:
    Archer(string n);
    void useSkill(Role& target, int skillId)override; // 覆寫：三個技能
};
#endif
