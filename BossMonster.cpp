#include "BossMonster.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief BossMonster 建構子
 */
BossMonster::BossMonster(string n, int h, int a)
    : Monster(n, h, a) {
}

/**
 * @brief 實作 Boss 怪物的技能邏輯
 * 1. 毀滅衝鋒: 造成 1.5 倍攻擊力傷害
 * 2. 吞噬撕咬: 造成 基礎 + 30 傷害
 * 3. 終焉咆哮: 造成 1.2 倍攻擊力傷害
 * 4. 毀滅之光: 造成 3 倍攻擊力傷害 (毀滅級必殺技)
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void BossMonster::useSkill(Role& target, int skillId) {
    if (!target.isAlive()) {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if (skillId == 1) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[Boss技能] " + name + " 使用了 毀滅衝鋒!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 1.5));
    }
    else if (skillId == 2) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[Boss技能] " + name + " 使用了 吞噬撕咬!" + Utils::COLOR_RESET);
        target.takeDamage(atk + 30);
    }
    else if (skillId == 3) {
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[Boss技能] " + name + " 使用了 終焉咆哮!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 1.2));
    }
    else if (skillId == 4) {
        Utils::Logger::addLog(Utils::COLOR_RED + Utils::COLOR_BOLD + "[Boss大絕] " + name + " 使用了 毀滅之光!" + Utils::COLOR_RESET);
        target.takeDamage((int)(atk * 3.0));
    }
    else {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string BossMonster::getSkillName(int skillId) const {
    if (skillId == 1) return "毀滅衝鋒";
    if (skillId == 2) return "吞噬撕咬";
    if (skillId == 3) return "終焉咆哮";
    if (skillId == 4) return "毀滅之光";
    return "Unknown Skill";
}

void BossMonster::printSkills() {
    cout << "\n--- Boss 怪物 " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " (1.5倍傷害)" << endl;
    cout << "2. " << getSkillName(2) << " (基礎 + 30)" << endl;
    cout << "3. " << getSkillName(3) << " (1.2倍傷害)" << endl;
    cout << "4. " << getSkillName(4) << " (3倍傷害)" << endl;
    cout << "-------------------------" << endl;
}
