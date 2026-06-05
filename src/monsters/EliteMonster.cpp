#include "EliteMonster.h"
#include <iostream>
#include "../core/Utils.h"
using namespace std;

/**
 * @brief EliteMonster 建構子
 */
EliteMonster::EliteMonster(string n, int h, int a)
    : Monster(n, h, a) {
}

/**
 * @brief 實作精英怪物的技能邏輯
 * 精英怪物的技能集比普通怪物更豐富，引入了倍率傷害與必殺技的概念：
 * 1. 強化衝鋒: 基礎傷害 + 10，穩定的輸出。
 * 2. 強力撕咬: 1.2 倍攻擊力傷害，隨著等級提升效果更明顯。
 * 3. 威壓咆哮: 基礎傷害 - 2，低傷害但具有威脅感。
 * 4. 毀滅打擊: 2 倍攻擊力傷害，屬於精英怪物的「大絕」，能瞬間造成巨大壓力。
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

/**
 * @brief 精英怪物的隨機技能選擇邏輯 (加權隨機)
 * 
 * 為了增加戰鬥的不可預測性與層次感，不使用均等隨機，而採用加權隨機：
 * - 技能 1 (普通): 40% 機率 -> 戰鬥基調
 * - 技能 2 (中強): 30% 機率 -> 增加威脅
 * - 技能 3 (弱): 20% 機率 -> 緩衝期
 * - 技能 4 (必殺): 10% 機率 -> 突發的高風險瞬間
 * 
 * @return 選中的技能 ID (1-4)
 */
int EliteMonster::getRandomSkill() {
    int roll = rand() % 100;
    if (roll < 40) return 1;      // 0-39
    if (roll < 70) return 2;      // 40-69
    if (roll < 90) return 3;      // 70-89
    return 4;                     // 90-99 (僅 10% 機率觸發毀滅打擊)
}
