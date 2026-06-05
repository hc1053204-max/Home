#include "Paladin.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Paladin 建構子
 * 設定聖騎士的初始屬性：HP 200, ATK 15 (高血量低攻擊)
 */
Paladin::Paladin(string n)
    : Role(n, 200, 15) {
}

/**
 * @brief 實作聖騎士的技能邏輯
 * 1. 神聖打擊 (Holy Strike): 造成 2 倍攻擊力的強大傷害
 * 2. 神聖治癒 (Divine Heal): 恢復自身最大生命值的 25%
 * 3. 審判 (Judgment): 造成 1.5 倍攻擊力 + 10 額外固定傷害
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Paladin::useSkill(Role& target, int skillId) {
    if (skillId == 2) {
        // 神聖治癒: 自我恢復生命
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 神聖治癒 (恢復生命)" + Utils::COLOR_RESET);
        int heal = maxHp / 4; // 25% 恢復
        int oldHp = hp;
        hp += heal;
        if (hp > maxHp) hp = maxHp;
        int actualHeal = hp - oldHp;
        Utils::Logger::addLog(Utils::COLOR_GREEN + "  " + name + " 恢復了 " + to_string(actualHeal) + " HP (" + to_string(oldHp) + " -> " + to_string(hp) + ")" + Utils::COLOR_RESET);
        return;
    }

    // 除了自我恢復技能外，其他技能需要目標存活
    if (!target.isAlive()) {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if (skillId == 1) {
        // 神聖打擊: 2x damage
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 神聖打擊 (2倍傷害)" + Utils::COLOR_RESET);
        int damage = (int)(atk * 2.0);
        target.takeDamage(damage);
    }
    else if (skillId == 3) {
        // 審判: 1.5x damage + 10 bonus
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 審判 (1.5倍傷害 + 額外傷害)" + Utils::COLOR_RESET);
        int damage = (int)(atk * 1.5) + 10;
        target.takeDamage(damage);
    }
    else {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string Paladin::getSkillName(int skillId) const {
    if (skillId == 1) return "神聖打擊";
    if (skillId == 2) return "神聖治癒";
    if (skillId == 3) return "審判";
    return "Unknown Skill";
}

void Paladin::printSkills() {
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << (int)(atk * 2.0) << endl;
    cout << "2. " << getSkillName(2) << " 恢復量：" << maxHp / 4 << endl;
    cout << "3. " << getSkillName(3) << " 傷害：" << (int)(atk * 1.5) + 10 << endl;
    cout << "-------------------------" << endl;
}
