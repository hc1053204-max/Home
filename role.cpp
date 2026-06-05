#include "Role.h"
#include <iostream>
#include "Utils.h"
using namespace std;

/**
 * @brief Role 建構子
 * 初始化角色的基本屬性，預設等級為 1，經驗值 0，升級所需經驗 100。
 */
Role::Role(string n,int h,int a):name(n),hp(h),maxHp(h),atk(a),level(1),xp(0),xpToNext(100){
}

/**
 * @brief 檢查角色是否存活
 * @return 若 HP > 0 則回傳 true
 */
bool Role::isAlive()const{
    return hp>0;
}

/**
 * @brief 處理角色受傷邏輯
 * @param damage 造成的傷害量
 */
void Role::takeDamage(int damage){
    if(damage<0)damage=0; // 確保傷害不為負數
    hp-=damage;
    if(hp<0)hp=0; // 生命值最低為 0
    Utils::Logger::addLog(Utils::COLOR_RED + "[受傷] " + name + " 受到 " + to_string(damage) + " 點傷害, 當前 HP: " + to_string(hp) + "/" + to_string(maxHp) + Utils::COLOR_RESET);
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

/**
 * @brief 發動普通攻擊
 * 檢查目標是否存活，若存活則造成等同於自身 atk 的傷害。
 * @param target 攻擊目標
 */
void Role::attack(Role& target){
    if(!target.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] " + name + " - 目標已經死亡!" + Utils::COLOR_RESET);
        return;
    }
    Utils::Logger::addLog(Utils::COLOR_WHITE + "[攻擊] " + name + " 發動普通攻擊,造成 " + to_string(atk) + " 點傷害" + Utils::COLOR_RESET);
    target.takeDamage(atk);
}

/**
 * @brief 獲取經驗值並處理升級邏輯
 * @param amount 獲得的經驗值量
 */
void Role::gainXp(int amount){
    if(amount<=0){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的經驗值數量" + Utils::COLOR_RESET);
        return;
    }
    xp+=amount;
    Utils::Logger::addLog(Utils::COLOR_GREEN + "[經驗] " + name + " 獲得了 " + to_string(amount) + " 點經驗 (總計: " + to_string(xp) + ")" + Utils::COLOR_RESET);
    
    // 升級檢查：使用 while 迴圈處理一次獲得大量經驗導致連續升級的情況
    while(xp>=xpToNext){
        xp-=xpToNext;
        level++;
        xpToNext=(int)(xpToNext*1.2); // 每次升級所需經驗增加 20%
        maxHp+=10;                    // 升級增加最大生命值
        atk+=5;                       // 升級增加攻擊力
        Utils::Logger::addLog("\n" + Utils::COLOR_YELLOW + Utils::COLOR_BOLD + "[升級] " + name + " 提升至 Lv." + to_string(level) + " | HP: " + to_string(maxHp) + " | ATK: " + to_string(atk) + Utils::COLOR_RESET + "\n");
        
        // 升級時回復部分血量（最高至最大生命值）
        hp=min(hp+20,maxHp);
    }
}

int Role::getLevel()const{
    return level;
}

/**
 * @brief 增加攻擊力
 * 用於處理裝備、藥水等強化效果。
 * @param amount 增加的數值
 */
void Role::addAtk(int amount){
    if(amount<=0){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的攻擊力增加" + Utils::COLOR_RESET);
        return;
    }
    atk+=amount;
    Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[物品] " + name + " 裝備效果: 攻擊力 +" + to_string(amount) + ", 當前攻擊力: " + to_string(atk) + Utils::COLOR_RESET);
}

void Role::addItem(unique_ptr<Item> item){
    Utils::Logger::addLog(Utils::COLOR_CYAN + "[獲得物品] " + item->getName() + " 已加入背包!" + Utils::COLOR_RESET);
    inventory.push_back(move(item));
}

void Role::showInventory(){
    cout << "\n--- " << name << " 的背包 ---" << endl;
    if(inventory.empty()){
        cout << "背包空空如也。" << endl;
    } else {
        for(int i=0; i<inventory.size(); i++){
            string typeStr = (inventory[i]->getType() == ItemType::WEAPON) ? "武器" : 
                             (inventory[i]->getType() == ItemType::ARMOR) ? "防具" : "消耗品";
            cout << i + 1 << ". " << inventory[i]->getName() << " [" << typeStr << "] (ATK +" << inventory[i]->getAtkBonus() << ", HP +" << inventory[i]->getHpBonus() << ")" << endl;
        }
    }
    if(equippedWeapon) cout << "目前裝備武器: " << equippedWeapon->getName() << endl;
    if(equippedArmor) cout << "目前裝備防具: " << equippedArmor->getName() << endl;
    cout << "-------------------------" << endl;
}

void Role::equipItem(int index){
    if(index < 1 || index > inventory.size()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的物品索引" + Utils::COLOR_RESET);
        return;
    }
    
    int idx = index - 1;
    Item* item = inventory[idx].get();
    
    if(item->getType() == ItemType::WEAPON){
        // 移除舊武器加成
        if(equippedWeapon) atk -= equippedWeapon->getAtkBonus();
        
        equippedWeapon = move(inventory[idx]);
        atk += equippedWeapon->getAtkBonus();
        Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[裝備] 裝備了武器 " + equippedWeapon->getName() + ", 攻擊力 +" + to_string(equippedWeapon->getAtkBonus()) + " (當前 ATK: " + to_string(atk) + ")" + Utils::COLOR_RESET);
        inventory.erase(inventory.begin() + idx);
    }
    else if(item->getType() == ItemType::ARMOR){
        // 移除舊防具加成
        if(equippedArmor) {
            maxHp -= equippedArmor->getHpBonus();
            hp -= equippedArmor->getHpBonus();
        }
        
        equippedArmor = move(inventory[idx]);
        maxHp += equippedArmor->getHpBonus();
        hp += equippedArmor->getHpBonus();
        Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[裝備] 裝備了防具 " + equippedArmor->getName() + ", 最大生命值 +" + to_string(equippedArmor->getHpBonus()) + " (當前 MaxHP: " + to_string(maxHp) + ")" + Utils::COLOR_RESET);
        inventory.erase(inventory.begin() + idx);
    }
    else {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 該物品不能裝備" + Utils::COLOR_RESET);
    }
}

void Role::useConsumable(int index){
    if(index < 1 || index > inventory.size()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的物品索引" + Utils::COLOR_RESET);
        return;
    }
    
    int idx = index - 1;
    if(inventory[idx]->getType() != ItemType::CONSUMABLE){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 該物品不是消耗品" + Utils::COLOR_RESET);
        return;
    }
    
    Utils::Logger::addLog(Utils::COLOR_GREEN + "[使用] 使用了 " + inventory[idx]->getName() + ", 恢復 HP " + to_string(inventory[idx]->getHpBonus()) + Utils::COLOR_RESET);
    hp = min(hp + inventory[idx]->getHpBonus(), maxHp);
    inventory.erase(inventory.begin() + idx);
}
