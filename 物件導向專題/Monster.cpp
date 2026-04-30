#include "Monster.h"
Monster::Monster(string n, int h, int a):Role(n, h, a) {}
void Monster::useSkill(Role& target, int skillId){
    // simple monster skills
    if(skillId==1){
        cout << "[Monster] " << name << " uses Furious Charge!" << endl;
        target.takeDamage(atk);
    } else if(skillId==2){
        cout << "[Monster] " << name << " uses Bite!" << endl;
        target.takeDamage(atk+5);
    } else {
        cout << "[Monster] " << name << " uses Roar (taunt)" << endl;
        int dmg = atk - 3;
        if(dmg < 1) dmg = 1;
        target.takeDamage(dmg);
    }
}
