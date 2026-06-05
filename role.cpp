#include "Role.h"
#include <iostream>
#include "Utils.h"
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
    cout << Utils::COLOR_RED << "[DAMAGE] " << name << " took " << damage << " damage, HP: " << hp << "/" << maxHp << Utils::COLOR_RESET << endl;
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
        cout << Utils::COLOR_BLUE << "[INFO] " << name << " - target is already dead!" << Utils::COLOR_RESET << endl;
        return;
    }
    cout << Utils::COLOR_WHITE << "[ATTACK] " << name << " uses a normal attack, dealing " << atk << " damage" << Utils::COLOR_RESET << endl;
    target.takeDamage(atk);
}
void Role::gainXp(int amount){
    if(amount<=0){
        cout << Utils::COLOR_BLUE << "[INFO] Invalid XP amount" << Utils::COLOR_RESET << endl;
        return;
    }
    xp+=amount;
    cout << Utils::COLOR_GREEN << "[XP] " << name << " gained " << amount << " XP (total: " << xp << ")" << Utils::COLOR_RESET << endl;
    // 升級檢查
    while(xp>=xpToNext){
        xp-=xpToNext;
        level++;
        xpToNext=(int)(xpToNext*1.2);
        maxHp+=10;
        atk+=5;
        cout << "\n" << Utils::COLOR_YELLOW << Utils::COLOR_BOLD << "[Level Up] " << name << " leveled up to Lv." << level << " | HP: " << maxHp << " | ATK: " << atk << Utils::COLOR_RESET << "\n" << endl;
        // 升級時回復部分血量（而不是滿血）
        hp=min(hp+20,maxHp);
    }
}
int Role::getLevel()const{
    return level;
}
void Role::addAtk(int amount){
    if(amount<=0){
        cout << Utils::COLOR_BLUE << "[INFO] Invalid attack increase" << Utils::COLOR_RESET << endl;
        return;
    }
    atk+=amount;
    cout << Utils::COLOR_MAGENTA << "[ITEM] " << name << " Equipment: ATK +" << amount << ", current ATK: " << atk << Utils::COLOR_RESET << endl;
}
