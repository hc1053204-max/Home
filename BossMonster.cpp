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
 * Boss 技能設計為「壓制性輸出」，旨在通過巨大的數值壓力強迫玩家使用治療或戰術行動：
 * 1. 毀滅衝鋒: 1.5 倍攻擊力傷害，強力的基礎攻擊。
 * 2. 吞噬撕咬: 基礎 + 30 傷害，在低等級 Boss 戰中極為致命。
 * 3. 終焉咆哮: 1.2 倍攻擊力傷害，穩定的輸出。
 * 4. 毀滅之光: 3 倍攻擊力傷害。這是遊戲中最危險的技能，一旦觸發，低 HP 玩家將直接被擊敗。
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
        // 毀滅之光：極高傷害
        // 視覺處理：使用 COLOR_RED + COLOR_BOLD 強化其威脅感
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

/**
 * @brief Boss 怪物的隨機技能選擇邏輯
 * 
 * Boss 的隨機分佈比精英怪物更均勻 (30% / 30% / 30% / 10%)，
 * 使得大絕 (毀滅之光) 的出現頻率維持在 10%，既能帶來驚喜/恐懼，又不至於讓戰鬥變得毫無意義。
 * 
 * @return 選中的技能 ID (1-4)
 */
int BossMonster::getRandomSkill() {
    int roll = rand() % 100;
    if (roll < 30) return 1;      // 0-29
    if (roll < 60) return 2;      // 30-59
    if (roll < 90) return 3;      // 60-89
    return 4;                     // 90-99 (10% 機率)
}
