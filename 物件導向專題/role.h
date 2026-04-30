#ifndef ROLE_H
#define ROLE_H
#include <string>
#include <iostream>
using namespace std;
class Role{
protected:
    string name;
    int hp, maxHp, atk;
    // level / xp
    int level;
    int xp;
    int xpToNext;
public:
    Role(string n,int h,int a);
    virtual~Role(){}
    // 純虛擬函式：多型的核心，第三個參數為技能編號(1-3)
    virtual void useSkill(Role& target, int skillId)=0;
    void attack(Role& target); // 普通攻擊方法
    void takeDamage(int damage);
    bool isAlive()const;
    string getName()const;
    int getHp()const;

    // xp / level
    void gainXp(int amount);
    int getLevel() const;

    // simple equipment/bonus
    void addAtk(int amount);
};
#endif