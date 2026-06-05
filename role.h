#ifndef ROLE_H
#define ROLE_H
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Item.h"
using namespace std;

/**
 * @brief Role 類別：遊戲中所有角色（玩家職業與怪物）的基類
 * 
 * 提供角色基礎屬性（HP, ATK, Level, XP）以及基礎行為（攻擊、受傷、升級）。
 * 採用多型設計，將技能實作延後至子類。
 */
class Role{
protected:
    string name;        // 角色名稱
    int hp;             // 當前生命值
    int maxHp;          // 最大生命值
    int atk;            // 攻擊力
    int level;          // 等級
    int xp;             // 當前經驗值
    int xpToNext;       // 升級所需經驗值

    // 裝備系統成員
    vector<unique_ptr<Item>> inventory;
    unique_ptr<Item> equippedWeapon;
    unique_ptr<Item> equippedArmor;

public:
    /**
     * @brief 建構子
     * @param n 角色名稱
     * @param h 生命值
     * @param a 攻擊力
     */
    Role(string n,int h,int a);
    virtual~Role(){}

    /**
     * @brief 使用技能 (純虛擬函式)
     * @param target 技能目標角色
     * @param skillId 技能 ID (1-3 為職業特定技能)
     */
    virtual void useSkill(Role&target,int skillId)=0;

    /**
     * @brief 獲取技能名稱 (純虛擬函式)
     * @param skillId 技能 ID
     * @return 技能名稱字串
     */
    virtual string getSkillName(int skillId) const = 0;

    /**
     * @brief 打印該角色所有可用技能 (純虛擬函式)
     */
    virtual void printSkills()=0;

    /**
     * @brief 發動基礎物理攻擊
     * @param target 被攻擊的目標
     */
    void attack(Role& target);

    /**
     * @brief 處理受到傷害的邏輯
     * @param damage 傷害數值
     */
    void takeDamage(int damage);

    /**
     * @brief 檢查角色是否仍然存活
     * @return 存活則回傳 true
     */
    bool isAlive() const;

    // --- Getter 方法 ---
    string getName()const;
    int getHp()const;
    int getMaxHp()const;
    int getAtk()const;
    int getLevel()const;
    virtual int getSkillCount() const { return 3; }

    /**
     * @brief 獲取經驗值並檢查是否升級
     * @param amount 獲得的經驗值量
     */
    void gainXp(int amount);

    /**
     * @brief 永久增加攻擊力 (例如裝備強化)
     * @param amount 增加的數值
     */
    void addAtk(int amount);

    // --- 裝備系統 ---
    void addItem(unique_ptr<Item> item);
    void showInventory();
    void equipItem(int index);
    void useConsumable(int index);
};
#endif
