#include "Archer.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Archer 建構子
 * 設定弓箭手的初始屬性：HP 100, ATK 30
 * 設計意圖：弓箭手是典型的「玻璃大砲」，生命值最低但基礎攻擊力最高。
 */
Archer::Archer(string n):Role(n,100,30){
}

/**
 * @brief 實作弓箭手的技能邏輯
 * 弓箭手的技能設計圍繞在「高頻率攻擊」與「機率爆發」：
 * 1. 連射 (Rapid Shot): 快速發射兩支箭，造成兩次普通傷害。
 * 2. 穿透箭 (Power Shot): 蓄力攻擊，造成 1.8 倍的高額單次傷害。
 * 3. 鷹眼 (Eagle Eye): 增加精準度，有 30% 機率觸發暴擊 (額外 +50% 傷害)。
 * 4. 箭雨 (Rain of Arrows): 大規模攻擊，連續命中 3 次普通傷害，是目前最強的輸出技能。
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Archer::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }
    if(skillId==1){
        // 連射: 2次攻擊
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 連射 (2次攻擊)" + Utils::COLOR_RESET);
        target.takeDamage(atk);
        target.takeDamage(atk);
    }
    else if(skillId==2){
        // 穿透箭: 1.8x damage
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 穿透箭 (1.8倍傷害)" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk*1.8));
    }
    else if(skillId==3){
        // 鷹眼: 30% 機率暴擊
        // 實作細節：使用 rand() 生成 0-99 的隨機數，小於 30 即為暴擊
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 鷹眼 (機率暴擊)" + Utils::COLOR_RESET);
        bool crit=(rand()%100)<30;
        int dmg=atk+(crit?atk/2:0);
        if (crit) Utils::Logger::addLog(Utils::COLOR_MAGENTA + "  暴擊命中!" + Utils::COLOR_RESET);
        target.takeDamage(dmg);
    }
    else if(skillId==4){
        // 箭雨: 3次攻擊
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 箭雨 (3次攻擊)" + Utils::COLOR_RESET);
        for(int i=0; i<3; i++){
            Utils::Logger::addLog(Utils::COLOR_WHITE + "  箭雨命中 " + to_string(i+1) + ": " + to_string(atk) + " 點傷害" + Utils::COLOR_RESET);
            target.takeDamage(atk);
        }
    }
    else{
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string Archer::getSkillName(int skillId) const {
    if(skillId == 1) return "連射";
    if(skillId == 2) return "穿透箭";
    if(skillId == 3) return "鷹眼";
    if(skillId == 4) return "箭雨";
    return "Unknown Skill";
}

void Archer::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << atk * 2 << " (2次攻擊)" << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << (int)(atk * 1.8) << endl;
    cout << "3. " << getSkillName(3) << " 傷害：" << atk << " (機率暴擊)" << endl;
    cout << "4. " << getSkillName(4) << " 傷害：" << atk * 3 << " (3次攻擊)" << endl;
    cout << "-------------------------" << endl;
}
