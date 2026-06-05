#ifndef ROLE_H
#define ROLE_H
#include <string>
#include <iostream>
using namespace std;

class Role{
protected:
    string name;
    int hp,maxHp,atk;
    int level;
    int xp;
    int xpToNext;
public:
    Role(string n,int h,int a);
    virtual~Role(){}

    // 純虛擬函式：多型的核心
    // skillId: 1-3 為職業特定技能（由子類實現）
    virtual void useSkill(Role&target,int skillId)=0;
    virtual string getSkillName(int skillId) const = 0;
    virtual void printSkills()=0;
    // 基礎攻擊
    void attack(Role& target);
    // 傷害處理
    void takeDamage(int damage);
    bool isAlive() const;
    // Getter 方法
    string getName()const;
    int getHp()const;
    int getMaxHp()const;
    int getAtk()const;
    int getLevel()const;
    // 經驗值與升級
    void gainXp(int amount);
    // 裝備與強化
    void addAtk(int amount);
};
#endif