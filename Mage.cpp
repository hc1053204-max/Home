#include "Mage.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Mage 建構子
 * 設定法師的初始屬性：HP 80, ATK 40
 */
Mage::Mage(string n):Role(n,80,40){
}

/**
 * @brief 實作法師的技能邏輯
 * 1. 火球術 (Fireball): 造成 atk + 20 的傷害
 * 2. 冰凍 (Ice Lance): 造成 atk + 10 的傷害 (可在未來擴展冰凍效果)
 * 3. 流星火雨 (Arcane Burst): 造成 atk + 35 的強大傷害
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Mage::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if(skillId==1){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 火球術!" + Utils::COLOR_RESET);
        target.takeDamage(atk+20);
    }
    else if(skillId==2) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 冰凍!" + Utils::COLOR_RESET);
        target.takeDamage(atk+10);
    }
    else if(skillId==3){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 流星火雨!" + Utils::COLOR_RESET);
        target.takeDamage(atk+35);
    }
    else if(skillId==4){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 隕石術 (3倍傷害)!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 3.0));
    }
    else{
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string Mage::getSkillName(int skillId) const {
    if(skillId == 1) return "火球術";
    if(skillId == 2) return "冰凍";
    if(skillId == 3) return "流星火雨";
    if(skillId == 4) return "隕石術";
    return "Unknown Skill";
}

void Mage::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << atk + 20 << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << atk + 10 << endl;
    cout << "3. " << getSkillName(3) << " 傷害：" << atk + 35 << endl;
    cout << "4. " << getSkillName(4) << " 傷害：" << (int)(atk * 3.0) << endl;
    cout << "-------------------------" << endl;
}
