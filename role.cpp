#include "Role.h"
#include <iostream>
using namespace std;

Role::Role(string n,int h,int a):name(n),hp(h),maxHp(h),atk(a),level(1),xp(0),xpToNext(100){
}
bool Role::isAlive()const{
    return hp>0;
}
void Role::takeDamage(int damage){
    if(damage<0)damage=0;
    hp-=damage;
    if(hp<0)hp=0;
    cout<<name<<"took"<<damage<<"damage,HP:"<<hp<<endl;
}
string Role::getName()const{
    return name;
}
int Role::getHp()const{
    return hp;
}
int Role::getMaxHp()const{
    return maxHp;
}
int Role::getAtk()const{
    return atk;
}
void Role::attack(Role& target){
    if(!target.isAlive()){
        cout<<name<<" - target is already dead!"<<endl;
        return;
    }
    cout<<name<<"uses a normal attack,dealing"<<atk<<" damage"<<endl;
    target.takeDamage(atk);
}
void Role::gainXp(int amount){
    if(amount<=0){
        cout<<"Invalid XP amount"<<endl;
        return;
    }
    xp+=amount;
    cout<<name<<"gained"<<amount<<" XP (total: "<<xp<<")"<<endl;
    // 升級檢查
    while(xp>=xpToNext){
        xp-=xpToNext;
        level++;
        xpToNext=(int)(xpToNext*1.2);
        maxHp+=10;
        atk+=5;
        cout<<"\n[Level Up]"<< name<<" leveled up to Lv."<<level<<" | HP: "<<maxHp<<" | ATK: "<<atk<<"\n"<<endl;
        // 升級時回復部分血量（而不是滿血）
        hp=min(hp+20,maxHp);
    }
}
int Role::getLevel()const{
    return level;
}
void Role::addAtk(int amount){
    if(amount<=0){
        cout<<"Invalid attack increase"<<endl;
        return;
    }
    atk+=amount;
    cout<<name<<"Equipment: ATK +"<< amount<<", current ATK: "<<atk<<endl;
}