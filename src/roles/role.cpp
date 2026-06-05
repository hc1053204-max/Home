#include "Role.h"
#include <iostream>
#include "../core/Utils.h"
using namespace std;

/**
 * @brief Role 建構子
 * 初始化角色的基本屬性。
 * 預設設定：等級 1，經驗值 0，初始升級所需經驗 100。
 */
Role::Role(string n,int h,int a):name(n),hp(h),maxHp(h),atk(a),level(1),xp(0),xpToNext(100){
}

/**
 * @brief 檢查角色是否存活
 * 只要 HP 大於 0 即視為存活。
 * @return 若 HP > 0 則回傳 true
 */
bool Role::isAlive()const{
    return hp>0;
}

/**
 * @brief 處理角色受傷邏輯
 * 包含防錯處理（防止負傷害）以及邊界限制（HP 最低為 0）。
 * @param damage 造成的傷害量
 */
void Role::takeDamage(int damage){
    if(damage<0)damage=0; // 確保傷害不為負數，防止受傷反而增加生命
    hp-=damage;
    if(hp<0)hp=0; // 限制生命值下限，避免出現負數 HP 的視覺錯誤
    Utils::Logger::addLog(Utils::COLOR_RED + "[受傷] " + name + " 受到 " + to_string(damage) + " 點傷害, 當前 HP: " + to_string(hp) + "/" + to_string(maxHp) + Utils::COLOR_RESET);
}

/**
 * @brief 處理生命恢復邏輯
 * @param amount 恢復量
 */
void Role::heal(int amount){
    if(amount<=0) return;
    int oldHp = hp;
    hp += amount;
    if(hp > maxHp) hp = maxHp; // 生命恢復不能超過最大生命值上限
    Utils::Logger::addLog(Utils::COLOR_GREEN + "[恢復] " + name + " 恢復了 " + to_string(hp - oldHp) + " 點生命, 當前 HP: " + to_string(hp) + "/" + to_string(maxHp) + Utils::COLOR_RESET);
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
 * 典型的回合制攻擊邏輯：檢查目標存活 -> 計算傷害 -> 執行扣血。
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
 * 採用指數級成長公式：每次升級所需經驗值增加 20%。
 * @param amount 獲得的經驗值量
 */
void Role::gainXp(int amount){
    if(amount<=0){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的經驗值數量" + Utils::COLOR_RESET);
        return;
    }
    xp+=amount;
    Utils::Logger::addLog(Utils::COLOR_GREEN + "[經驗] " + name + " 獲得了 " + to_string(amount) + " 點經驗 (總計: " + to_string(xp) + ")" + Utils::COLOR_RESET);
    
    // 升級檢查：使用 while 迴圈處理「一次獲得大量經驗導致連續升級」的情況
    while(xp>=xpToNext){
        xp-=xpToNext;
        level++;
        // 經驗值曲線：xpToNext = xpToNext * 1.2
        xpToNext=(int)(xpToNext*1.2); 
        maxHp+=10;                    // 等級提升帶來的基礎屬性增益
        atk+=5;                       
        Utils::Logger::addLog("\n" + Utils::COLOR_YELLOW + Utils::COLOR_BOLD + "[升級] " + name + " 提升至 Lv." + to_string(level) + " | HP: " + to_string(maxHp) + " | ATK: " + to_string(atk) + Utils::COLOR_RESET + "\n");
        
        // 升級獎勵：恢復少量生命值 (20 點)，但不能超過最大生命值
        hp=min(hp+20,maxHp);
    }
}

int Role::getLevel()const{
    return level;
}

/**
 * @brief 永久增加攻擊力
 * 適用於角色天賦或永久強化道具。
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

/**
 * @brief 將物品加入背包
 * 使用 std::move 將 unique_ptr 的所有權轉移到背包 vector 中。
 * @param item 物品指針
 */
void Role::addItem(unique_ptr<Item> item){
    Utils::Logger::addLog(Utils::COLOR_CYAN + "[獲得物品] " + item->getName() + " 已加入背包!" + Utils::COLOR_RESET);
    inventory.push_back(move(item));
}

/**
 * @brief 顯示當前背包內容與裝備狀態
 */
void Role::showInventory(){
    cout << "\n--- " << name << " 的背包 ---" << endl;
    if(inventory.empty()){
        cout << "背包空空如也。" << endl;
    } else {
        for(int i=0; i<inventory.size(); i++){
            // 根據 ItemType 枚舉將類型轉換為可讀文字
            string typeStr = (inventory[i]->getType() == ItemType::WEAPON) ? "武器" : 
                             (inventory[i]->getType() == ItemType::ARMOR) ? "防具" : "消耗品";
            cout << i + 1 << ". " << inventory[i]->getName() << " [" << typeStr << "] (ATK +" << inventory[i]->getAtkBonus() << ", HP +" << inventory[i]->getHpBonus() << ")" << endl;
        }
    }
    if(equippedWeapon) cout << "目前裝備武器: " << equippedWeapon->getName() << endl;
    if(equippedArmor) cout << "目前裝備防具: " << equippedArmor->getName() << endl;
    cout << "-------------------------" << endl;
}

/**
 * @brief 裝備背包中的物品
 * 邏輯流程：
 * 1. 驗證索引有效性。
 * 2. 根據物品類型 (Weapon/Armor) 處理屬性替換。
 * 3. 移除舊裝備加成 -> 安裝新裝備 -> 增加新加成 -> 從背包移除該物品。
 * @param index 背包中的物品索引 (1-based)
 */
void Role::equipItem(int index){
    if(index < 1 || index > inventory.size()){
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 無效的物品索引" + Utils::COLOR_RESET);
        return;
    }
    
    int idx = index - 1;
    Item* item = inventory[idx].get();
    
    if(item->getType() == ItemType::WEAPON){
        // 武器替換邏輯：先扣除原裝備的攻擊力加成，再增加新裝備的
        if(equippedWeapon) atk -= equippedWeapon->getAtkBonus();
        
        equippedWeapon = move(inventory[idx]); // 轉移所有權
        atk += equippedWeapon->getAtkBonus();
        Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[裝備] 裝備了武器 " + equippedWeapon->getName() + ", 攻擊力 +" + to_string(equippedWeapon->getAtkBonus()) + " (當前 ATK: " + to_string(atk) + ")" + Utils::COLOR_RESET);
        inventory.erase(inventory.begin() + idx);
    }
    else if(item->getType() == ItemType::ARMOR){
        // 防具替換邏輯：同步扣除最大生命值與當前生命值
        if(equippedArmor) {
            maxHp -= equippedArmor->getHpBonus();
            hp -= equippedArmor->getHpBonus();
        }
        
        equippedArmor = move(inventory[idx]); // 轉移所有權
        maxHp += equippedArmor->getHpBonus();
        hp += equippedArmor->getHpBonus();
        Utils::Logger::addLog(Utils::COLOR_MAGENTA + "[裝備] 裝備了防具 " + equippedArmor->getName() + ", 最大生命值 +" + to_string(equippedArmor->getHpBonus()) + " (當前 MaxHP: " + to_string(maxHp) + ")" + Utils::COLOR_RESET);
        inventory.erase(inventory.begin() + idx);
    }
    else {
        Utils::Logger::addLog(Utils::COLOR_BLUE + "[提示] 該物品不能裝備" + Utils::COLOR_RESET);
    }
}

/**
 * @brief 使用背包中的消耗品
 * 邏輯：驗證類型 -> 執行效果 (如恢復 HP) -> 從背包移除。
 * @param index 背包中的物品索引 (1-based)
 */
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
