#include "Archer.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Archer 建構子
 * 設定弓箭手的初始屬性：HP 100, ATK 30
 */
Archer::Archer(string n):Role(n,100,30){
}

/**
 * @brief 實作弓箭手的技能邏輯
 * 1. 連射 (Rapid Shot): 連續造成 2 次普通攻擊傷害
 * 2. 穿透箭 (Power Shot): 造成 1.8 倍攻擊力的強大傷害
 * 3. 箭雨 (Eagle Eye): 基礎傷害，但有 30% 機率觸發暴擊 (額外 +50% 傷害)
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Archer::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }
    if(skillId==1){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 連射 (2次攻擊)" + Utils::COLOR_RESET);
        target.takeDamage(atk);
        target.takeDamage(atk);
    }
    else if(skillId==2){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 穿透箭 (1.8倍傷害)" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk*1.8));
    }
    else if(skillId==3){
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 鷹眼 (機率暴擊)" + Utils::COLOR_RESET);
        bool crit=(rand()%100)<30;
        int dmg=atk+(crit?atk/2:0);
        if (crit) Utils::Logger::addLog(Utils::COLOR_MAGENTA + "  暴擊命中!" + Utils::COLOR_RESET);
        target.takeDamage(dmg);
    }
    else if(skillId==4){
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
