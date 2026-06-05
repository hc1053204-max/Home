#include "Mage.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Mage 建構子
 * 設定法師的初始屬性：HP 80, ATK 40
 * 設計意圖：法師是典型的「高攻低血」職業，依賴強大的技能傷害來快速結束戰鬥。
 */
Mage::Mage(string n):Role(n,80,40){
}

/**
 * @brief 實作法師的技能邏輯
 * 法師的技能設計圍繞在「固定加值傷害」與「極高倍率爆發」：
 * 1. 火球術 (Fireball): 基礎攻擊力 + 20 點固定傷害，在低等級時非常強大。
 * 2. 冰凍 (Ice Lance): 基礎攻擊力 + 10 點固定傷害 (未來可擴展為降低目標速度或凍結回合)。
 * 3. 流星火雨 (Arcane Burst): 基礎攻擊力 + 35 點強大固定傷害。
 * 4. 隕石術 (Meteor): 造成 3 倍攻擊力的毀滅性傷害，是所有職業中單次傷害最高的技能之一。
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Mage::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if(skillId==1){
        // 火球術: atk + 20
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 火球術!" + Utils::COLOR_RESET);
        target.takeDamage(atk+20);
    }
    else if(skillId==2) {
        // 冰凍: atk + 10
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 冰凍!" + Utils::COLOR_RESET);
        target.takeDamage(atk+10);
    }
    else if(skillId==3){
        // 流星火雨: atk + 35
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 流星火雨!" + Utils::COLOR_RESET);
        target.takeDamage(atk+35);
    }
    else if(skillId==4){
        // 隕石術: 3.0x damage
        // 設計意圖：法師的終極技能，以極高風險（低 HP）換取極高收益。
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
