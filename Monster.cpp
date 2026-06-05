#include "Monster.h"
Monster::Monster(string n,int h,int a):Role(n,h,a){}
void Monster::useSkill(Role&target,int skillId){
    // simple monster skills
    if(skillId==1){
        cout<<"[Monster]"<<name<<" uses Furious Charge!"<<endl;
        target.takeDamage(atk);
    }
    else if(skillId==2){
        cout<<"[Monster] "<<name<<" uses Bite!"<<endl;
        target.takeDamage(atk+5);
    }
    else{
        cout<<"[Monster]"<<name<<" uses Roar (taunt)"<<endl;
        int dmg=atk-3;
        if(dmg<1)dmg=1;
        target.takeDamage(dmg);
    }
}

string Monster::getSkillName(int skillId) const {
    if(skillId == 1) return "Furious Charge (狂暴衝鋒)";
    if(skillId == 2) return "Bite (撕咬)";
    if(skillId == 3) return "Roar (咆哮)";
    return "Unknown Skill";
}

void Monster::printSkills(){
    cout << "\n--- " << name << " 技能列表 ---" << endl;
    cout << "1. " << getSkillName(1) << " (基礎攻擊)" << endl;
    cout << "2. " << getSkillName(2) << " (基礎攻擊 + 5)" << endl;
    cout << "3. " << getSkillName(3) << " (基礎攻擊 - 3)" << endl;
    cout << "-------------------------" << endl;
}
