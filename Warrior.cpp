#include "Warrior.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Warrior 建構子
 * 設定戰士的初始屬性：HP 150, ATK 25
 */
Warrior::Warrior(string n)
    : Role(n,150,25){
}

/**
 * @brief 實作戰士的技能邏輯
 * 1. 斬擊 (Heavy Strike): 造成 1.5 倍攻擊力的傷害
 * 2. 盾擊 (Whirlwind): 造成 1.2 倍攻擊力的傷害，且連續攻擊 2 次
 * 3. 狂戰 (War Cry): 恢復最大生命值的 20%
 * @param target 技能目標
 * @param skillId 技能 ID
 */
void Warrior::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }

    if(skillId==1){
        // 斬擊: 1.5x damage
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 斬擊 (1.5倍傷害)" + Utils::COLOR_RESET);
        int damage=(int)(atk*1.5);
        target.takeDamage(damage);
    }
    else if(skillId==2){
        // 盾擊: 1.2x damage x2
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 盾擊 (1.2倍傷害 x2)" + Utils::COLOR_RESET);
        int damage=(int)(atk*1.2);
        for(int i=0;i<2;i++){
            Utils::Logger::addLog(Utils::COLOR_WHITE + "  命中 " + to_string(i + 1) + ": " + to_string(damage) + " 點傷害" + Utils::COLOR_RESET);
            target.takeDamage(damage);
        }
    }
    else if(skillId==3){
        // 狂戰: 自我恢復 HP
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 狂戰 (恢復生命)" + Utils::COLOR_RESET);
        int heal=maxHp/5;  // 20% 恢復
        int oldHp=hp;
        hp+=heal;
        if(hp>maxHp)hp=maxHp;
        int actualHeal=hp-oldHp;
        Utils::Logger::addLog(Utils::COLOR_GREEN + "  " + name + " 恢復了 " + to_string(actualHeal) + " HP (" + to_string(oldHp) + " -> " + to_string(hp) + ")" + Utils::COLOR_RESET);
    }
    else if(skillId==4){
        // 毀滅一擊: 2.5x damage
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 毀滅一擊 (2.5倍傷害)" + Utils::COLOR_RESET);
        int damage=(int)(atk*2.5);
        target.takeDamage(damage);
    }
    else{
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 未知的技能 ID: " + to_string(skillId) + Utils::COLOR_RESET);
    }
}

string Warrior::getSkillName(int skillId) const {
    if(skillId == 1) return "斬擊";
    if(skillId == 2) return "盾擊";
    if(skillId == 3) return "狂戰";
    if(skillId == 4) return "毀滅一擊";
    return "Unknown Skill";
}

void Warrior::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << (int)(atk * 1.5) << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << (int)(atk * 1.2 * 2) << " (2次攻擊)" << endl;
    cout << "3. " << getSkillName(3) << " 恢復量：" << maxHp / 5 << endl;
    cout << "4. " << getSkillName(4) << " 傷害：" << (int)(atk * 2.5) << endl;
    cout << "-------------------------" << endl;
}
