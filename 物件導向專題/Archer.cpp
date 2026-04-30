#include "Archer.h"
Archer::Archer(string n)
    :Role(n,100,30){
}
void Archer::useSkill(Role& target, int skillId){
    if(skillId==1){
        cout<<"[Archer] "<<name<<" uses Rapid Shot (2 hits)"<<endl;
        target.takeDamage(atk);
        target.takeDamage(atk);
    } else if(skillId==2){
        cout<<"[Archer] "<<name<<" uses Power Shot (1.8x)"<<endl;
        target.takeDamage(int(atk*1.8));
    } else {
        cout<<"[Archer] "<<name<<" uses Eagle Eye (crit chance simulated)"<<endl;
        // simulate crit
        bool crit = (rand()%100)<30;
        int dmg = atk + (crit? atk/2 : 0);
        if(crit) cout<<"Critical"<<endl;
        target.takeDamage(dmg);
    }
}
