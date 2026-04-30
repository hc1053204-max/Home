#include "Mage.h"
Mage::Mage(string n):
    Role(n, 80, 40){
}
void Mage::useSkill(Role& target, int skillId){
    if(skillId==1){
        cout<<"[Mage] "<<name<<" casts Fireball!"<<endl;
        target.takeDamage(atk+20);
    } else if(skillId==2){
        cout<<"[Mage] "<<name<<" casts Ice Lance (slow, lower damage)"<<endl;
        target.takeDamage(atk+10);
    } else {
        cout<<"[Mage] "<<name<<" casts Arcane Burst (big)"<<endl;
        target.takeDamage(atk+35);
    }
}
