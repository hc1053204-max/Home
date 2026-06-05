#ifndef ITEM_H
#define ITEM_H
#include <string>
using namespace std;

/**
 * @brief ItemType 枚舉類別
 * 用於區分物品的類型：武器、防具、消耗品。
 */
enum class ItemType {
    WEAPON,     // 武器
    ARMOR,      // 防具
    CONSUMABLE  // 消耗品
};

/**
 * @brief Item 類別：遊戲中物品的基類
 * 
 * 定義了所有物品的共有屬性，如名稱、類型、屬性加成（攻擊力/生命值）及價值。
 */
class Item {
protected:
    string name;      // 物品名稱
    ItemType type;    // 物品類型
    int atkBonus;     // 攻擊力加成
    int hpBonus;      // 生命值加成
    int value;        // 物品價值/價格

public:
    /**
     * @brief Item 建構子
     * @param n 物品名稱
     * @param t 物品類型
     * @param a 攻擊力加成
     * @param h 生命值加成
     * @param v 物品價值
     */
    Item(string n, ItemType t, int a, int h, int v) 
        : name(n), type(t), atkBonus(a), hpBonus(h), value(v) {}
    virtual ~Item() {}

    // --- Getter 方法 ---
    string getName() const { return name; }
    ItemType getType() const { return type; }
    int getAtkBonus() const { return atkBonus; }
    int getHpBonus() const { return hpBonus; }
    int getValue() const { return value; }
};

#endif
