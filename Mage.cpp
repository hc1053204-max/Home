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