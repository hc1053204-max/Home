#include "Archer.h"

// 定義常數
const int ARCHER_HP = 100;
const int ARCHER_ATK = 30;
const double POWER_SHOT_MULTIPLIER = 1.8;
const int CRIT_CHANCE = 30;
const int CRIT_DAMAGE_BONUS = 50; // atk的百分比

Archer::Archer(string n)
    : Role(n, ARCHER_HP, ARCHER_ATK) {
}

void Archer::useSkill(Role& target, int skillId) {
    if(skillId == 1) {
        cout << "[Archer] " << name << " uses Rapid Shot (2 hits)" << endl;
        target.takeDamage(atk);
        target.takeDamage(atk);
    } 
    else if(skillId == 2) {
        cout << "[Archer] " << name << " uses Power Shot (1.8x)" << endl;
        target.takeDamage(int(atk * POWER_SHOT_MULTIPLIER));
    } 
    else if(skillId == 3) {
        cout << "[Archer] " << name << " uses Eagle Eye (crit chance simulated)" << endl;
        bool crit = (rand() % 100) < CRIT_CHANCE;
        int dmg = atk + (crit ? atk * CRIT_DAMAGE_BONUS / 100 : 0);
        if(crit) cout << "Critical Hit!" << endl;
        target.takeDamage(dmg);
    }
    else {
        cout << "[Error] Invalid skill ID: " << skillId << endl;
    }
}
