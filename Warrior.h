#ifndef WARRIOR_H
#define WARRIOR_H
#include "Role.h"
#include <string>
using namespace std;
class Warrior:
    public Role{
public:
    Warrior(string n);
    // 覆寫技能方法
    // skillId: 1=Heavy Strike, 2=Whirlwind, 3=War Cry
    void useSkill(Role& target,int skillId)override;
};
#endif