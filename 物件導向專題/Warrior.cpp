#include "Warrior.h"
Warrior::Warrior(string n):
    Role(n,150,25){}
void Warrior::useSkill(Role& target, int skillId){
    if(skillId==1){
        cout<<"[Warrior] "<<name<<" uses Heavy Strike (1.5x)"<<endl;
        target.takeDamage(int(atk*1.5));
    } else if(skillId==2){
        cout<<"[Warrior] "<<name<<" uses Whirlwind (1.2x x2)"<<endl;
        target.takeDamage(int(atk*1.2));
        target.takeDamage(int(atk*1.2));
    } else {
        cout<<"[Warrior] "<<name<<" uses War Cry (buff self)"<<endl;
        // simple buff: restore some hp
        // restore 20% maxHp
        int heal = maxHp/5;
        hp += heal;
        if(hp>maxHp) hp = maxHp;
        cout<<name<<" 恢復 "<<heal<<" HP"<<endl;
    }
}
