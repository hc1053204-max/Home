#include "Mage.h"
#include <iostream>
using namespace std;
Mage::Mage(string n):Role(n,80,40){
}
void Mage::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        cout<<"[Mage] Target is already dead!"<<endl;
        return;
    }

    if(skillId==1){
        cout<<"[Mage]"<<name<<" casts Fireball!"<<endl;
        target.takeDamage(atk+20);
    }
    else if(skillId==2) {
        cout<<"[Mage]"<<name<<" casts Ice Lance(slow,lower damage)"<<endl;
        target.takeDamage(atk+10);
        // 可以在這裡加 debuff 效果
    }
    else if(skillId==3){
        cout<<"[Mage]"<<name<<" casts Arcane Burst (big damage)"<<endl;
        target.takeDamage(atk+35);
    }
    else{
        cout<<"[Mage] Unknown skill ID: "<<skillId<<endl;
    }
}

string Mage::getSkillName(int skillId) const {
    if(skillId == 1) return "火球術";
    if(skillId == 2) return "冰凍";
    if(skillId == 3) return "流星火雨";
    return "Unknown Skill";
}

void Mage::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << atk + 20 << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << atk + 10 << endl;
    cout << "3. " << getSkillName(3) << " 傷害：" << atk + 35 << endl;
    cout << "-------------------------" << endl;
}