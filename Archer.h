#ifndef ARCHER_H
#define ARCHER_H
#include "Role.h"
#include <string>
using namespace std;
class Archer:
    public Role{
public:
    Archer(string n);
    // 覆寫技能方法
    // skillId: 1=Rapid Shot, 2=Power Shot, 3=Eagle Eye
    void useSkill(Role& target,int skillId)override;
};
#endif