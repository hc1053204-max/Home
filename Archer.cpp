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
