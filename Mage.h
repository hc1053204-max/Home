#ifndef MAGE_H
#define MAGE_H
#include "Role.h"
#include <string>
using namespace std;

class Mage:
    public Role{
public:
    Mage(string n);
    // 覆寫技能方法
    // skillId: 1=Fireball, 2=Ice Lance, 3=Arcane Burst
    void useSkill(Role& target,int skillId)override;
};
#endif