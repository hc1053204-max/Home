#include "Archer.h"
#include <iostream>
using namespace std;
Archer::Archer(string n):Role(n,100,30){
}

void Archer::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        cout<<"[Archer] Target is dead!"<<endl;
        return;
    }
    if(skillId==1){
        cout<<"[Archer] "<<name<<"uses Rapid Shot (2 hits)"<<endl;
        target.takeDamage(atk);
        target.takeDamage(atk);
    }
    else if(skillId==2){
        cout<<"[Archer]"<<name<<"uses Power Shot (1.8x)"<<endl;
        target.takeDamage((int)(atk*1.8));
    }
    else if(skillId==3){
        cout<<"[Archer]"<<name<<"uses Eagle Eye (crit chance)"<<endl;
        bool crit=(rand()%100)<30;
        int dmg=atk+(crit?atk/2:0);
        if (crit)cout<<"Critical!"<<endl;
        target.takeDamage(dmg);
    }
    else{
        cout<<"[Archer] Unknown skill!"<<endl;
    }
}

string Archer::getSkillName(int skillId) const {
    if(skillId == 1) return "連射";
    if(skillId == 2) return "穿透箭";
    if(skillId == 3) return "箭雨";
    return "Unknown Skill";
}

void Archer::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << atk * 2 << " (2次攻擊)" << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << (int)(atk * 1.8) << endl;
    cout << "3. " << getSkillName(3) << " 傷害：" << atk << " (機率暴擊)" << endl;
    cout << "-------------------------" << endl;
}
