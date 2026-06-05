#include "Warrior.h"
#include <iostream>
using namespace std;

Warrior::Warrior(string n)
    : Role(n,150,25){
}
void Warrior::useSkill(Role& target,int skillId){
    if(!target.isAlive()){
        cout<<"[Warrior] Target is already dead!"<<endl;
        return;
    }

    if(skillId==1){
        // Heavy Strike: 1.5x damage
        cout<<"[Warrior]"<<name<<" uses Heavy Strike (1.5x damage)"<<endl;
        int damage=(int)(atk*1.5);
        target.takeDamage(damage);
    }
    else if(skillId==2){
        // Whirlwind: 1.2x damage x2
        cout<<"[Warrior]"<<name<<" uses Whirlwind (1.2x x2)"<<endl;
        int damage=(int)(atk*1.2);
        for(int i=0;i<2;i++){
            cout<<"Hit"<<(i + 1)<<":"<<damage<<"damage"<<endl;
            target.takeDamage(damage);
        }
    }
    else if(skillId==3){
        // War Cry: self-buff (restore HP)
        cout<<"[Warrior]"<<name<<" uses War Cry (restore HP)!"<<endl;
        int heal=maxHp/5;  // 20% 恢復
        int oldHp=hp;
        hp+=heal;
        if(hp>maxHp)hp=maxHp;
        int actualHeal=hp-oldHp;
        cout<<"  "<<name<<" 恢復 "<<actualHeal<<" HP("<<oldHp<<" -> "<<hp<<")"<<endl;
    }
    else{
        cout<<"[Warrior] Unknown skill ID: "<<skillId<<endl;
    }
}

string Warrior::getSkillName(int skillId) const {
    if(skillId == 1) return "斬擊";
    if(skillId == 2) return "盾擊";
    if(skillId == 3) return "狂戰";
    return "Unknown Skill";
}

void Warrior::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " 傷害：" << (int)(atk * 1.5) << endl;
    cout << "2. " << getSkillName(2) << " 傷害：" << (int)(atk * 1.2 * 2) << " (2次攻擊)" << endl;
    cout << "3. " << getSkillName(3) << " 恢復量：" << maxHp / 5 << endl;
    cout << "-------------------------" << endl;
}