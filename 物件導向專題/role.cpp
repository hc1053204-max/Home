#include "role.h"
Role::Role(string n,int h,int a): 
    name(n),hp(h),maxHp(h),atk(a),level(1),xp(0),xpToNext(100){}
bool Role::isAlive()const{
    return hp>0;
}
void Role::takeDamage(int damage){
    hp-=damage;
    if(hp<0)hp=0;
    cout<<name<<" HP:"<<hp<<endl;
}
string Role::getName()const{
    return name;
}
int Role::getHp()const{
    return hp;
}

void Role::attack(Role& target){
    cout<<name<<" 使用普通攻擊，造成 "<<atk<<" 傷害"<<endl;
    target.takeDamage(atk);
}

void Role::gainXp(int amount){
    xp += amount;
    cout<<name<<" 獲得 "<<amount<<" 經驗值 (總計:"<<xp<<")"<<endl;
    while(xp >= xpToNext){
        xp -= xpToNext;
        level++;
        xpToNext = int(xpToNext * 1.2);
        // simple level up: increase stats
        maxHp += 10;
        hp = maxHp;
        atk += 5;
        cout<<name<<" 升級了！ 等級:"<<level<<" HP/ATK 增加"<<endl;
    }
}

int Role::getLevel() const{
    return level;
}

void Role::addAtk(int amount){
    atk += amount;
    cout<<name<<" 裝備效果：攻擊力增加 "<<amount<<"，當前攻擊:"<<atk<<endl;
}
