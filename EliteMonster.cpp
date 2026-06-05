#include "EliteMonster.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief EliteMonster 建構子
 */
EliteMonster::EliteMonster(string n, int h, int a)
    : Monster(n, h, a) {
}

/**
 * @brief 實作精英怪物的技能邏輯
 * 1. 強化衝鋒: 造成 基礎 + 10 傷害
 * 2. 強力撕咬: 造成 1.2 倍攻擊力傷害
 * 3. 威壓咆哮: 造成 基礎 - 2 傷害
 * 4. 毀滅打擊: 造成 2 倍攻擊力傷害 (必殺技)
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void EliteMonster::useSkill(Role& target, int skillId) {
    if (!target.isAlive()) {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if (skillId == 1) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[精英技能] " + name + " 使用了 強化衝鋒!" + Utils::COLOR_RESET);
        target.takeDamage(atk + 10);
    }
    else if (skillId == 2) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[精英技能] " + name + " 使用了 強力撕咬!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 1.2));
    }
    else if (skillId == 3) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[精英技能] " + name + " 使用了 威壓咆哮!" + Utils::COLOR_RESET);
        target.takeDamage(atk - 2);
    }
    else if (skillId == 4) {
        Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[精英必殺] " + name + " 使用了 毀滅打擊!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 2.0));
    }
    else {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string EliteMonster::getSkillName(int skillId) const {
    if (skillId == 1) return "強化衝鋒";
    if (skillId == 2) return "強力撕咬";
    if (skillId == 3) return "威壓咆哮";
    if (skillId == 4) return "毀滅打擊";
    return "Unknown Skill";
}

void EliteMonster::printSkills() {
    cout << "\n--- 精英怪物 " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " (基礎 + 10)" << endl;
    cout << "2. " << getSkillName(2) << " (1.2倍傷害)" << endl;
    cout << "3. " << getSkillName(3) << " (基礎 - 2)" << endl;
    cout << "4. " << getSkillName(4) << " (2倍傷害)" << endl;
    cout << "-------------------------" << endl;
}
