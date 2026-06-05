#include "Monster.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Monster 建構子
 */
Monster::Monster(string n,int h,int a):Role(n,h,a){}

/**
 * @brief 實作怪物的技能邏輯
 * 1. 狂暴衝鋒 (Furious Charge): 造成基礎攻擊力傷害
 * 2. 撕咬 (Bite): 造成基礎攻擊力 + 5 的傷害
 * 3. 咆哮 (Roar): 造成較低傷害 (基礎-3)
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Monster::useSkill(Role&target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if(skillId==1){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[怪物技能] " + name + " 使用了 狂暴衝鋒!" + Utils::COLOR_RESET);
        target.takeDamage(atk);
    }
    else if(skillId==2){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[怪物技能] " + name + " 使用了 撕咬!" + Utils::COLOR_RESET);
        target.takeDamage(atk+5);
    }
    else{
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[怪物技能] " + name + " 使用了 咆哮!" + Utils::COLOR_RESET);
        int dmg=atk-3;
        if(dmg<1)dmg=1;
        target.takeDamage(dmg);
    }
}

string Monster::getSkillName(int skillId) const {
    if(skillId == 1) return "Furious Charge (狂暴衝鋒)";
    if(skillId == 2) return "Bite (撕咬)";
    if(skillId == 3) return "Roar (咆哮)";
    return "Unknown Skill";
}

void Monster::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " (基礎攻擊)" << endl;
    cout << "2. " << getSkillName(2) << " (基礎攻擊 + 5)" << endl;
    cout << "3. " << getSkillName(3) << " (基礎攻擊 - 3)" << endl;
    cout << "-------------------------" << endl;
}
