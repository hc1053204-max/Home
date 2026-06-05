#include "Warrior.h"
#include <iostream>
#include "../core/Utils.h"
using namespace std;

/**
 * @brief Warrior 建構子
 * 設定戰士的初始屬性：HP 150, ATK 25
 * 設計意圖：戰士作為前排，擁有較高的基礎 HP 和中等的攻擊力。
 */
Warrior::Warrior(string n)
    : Role(n,150,25){
}

/**
 * @brief 實作戰士的技能邏輯
 * 戰士的技能設計圍繞在「穩定輸出」與「生存恢復」之間：
 * 1. 斬擊 (Heavy Strike): 穩定的高額單次傷害 (1.5x)，適合快速擊殺低血量敵人。
 * 2. 盾擊 (Whirlwind): 透過多次攻擊 (1.2x * 2) 達成最高總傷害，模擬範圍攻擊效果。
 * 3. 狂戰 (War Cry): 提供必要的生存能力，恢復 20% 最大生命值，延長作戰時間。
 * 4. 毀滅一擊 (Destructive Blow): 極高爆發傷害 (2.5x)，是戰士最強的單體攻擊手段。
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
        // 實作細節：使用迴圈模擬多次命中，每一下都會觸發目標的 takeDamage 邏輯
        Utils::Logger::addLog(Utils::COLOR_YELLOW + "[技能] " + name + " 使用了 盾擊 (1.2倍傷害 x2)" + Utils::COLOR_RESET);
        int damage=(int)(atk*1.2);
        for(int i=0;i<2;i++){
            Utils::Logger::addLog(Utils::COLOR_WHITE + "  命中 " + to_string(i + 1) + ": " + to_string(damage) + " 點傷害" + Utils::COLOR_RESET);
            target.takeDamage(damage);
        }
    }
    else if(skillId==3){
        // 狂戰: 自我恢復 HP
        // 恢復量基於最大生命值的百分比，確保在等級提升後恢復量同步增加
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
