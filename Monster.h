#ifndef MONSTER_H
#define MONSTER_H
#include "Role.h"
#include <string>
using namespace std;

class Monster:
    public Role{
public:
    // 建構函數：名字、血量、攻擊力
    Monster(string n,int h,int a);
    // 怪物技能（覆寫基類）
    void useSkill(Role& target,int skillId)override;
    string getSkillName(int skillId) const override;
    void printSkills()override;
};
#endif