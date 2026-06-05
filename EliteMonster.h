#ifndef ELITEMONSTER_H
#define ELITEMONSTER_H
#include "Monster.h"
#include <string>
using namespace std;

/**
 * @brief EliteMonster 類別：精英怪物，繼承自 Monster
 * 
 * 精英怪物比普通怪物擁有更強的屬性以及更多的技能選擇（共 4 種技能）。
 */
class EliteMonster : public Monster {
public:
    /**
     * @brief 建構子
     * @param n 精英怪物名稱
     * @param h 生命值
     * @param a 攻擊力
     */
    EliteMonster(string n, int h, int a);

    /**
     * @brief 覆寫技能方法 (提供更強力的技能集)
     * skillId: 1=強化衝鋒, 2=強力撕咬, 3=威壓咆哮, 4=毀滅打擊
     * @param target 技能目標
     * @param skillId 技能 ID
     */
    void useSkill(Role& target, int skillId) override;

    /**
     * @brief 覆寫獲取技能名稱方法
     * @param skillId 技能 ID
     * @return 技能名稱
     */
    string getSkillName(int skillId) const override;

    /**
     * @brief 覆寫打印技能列表方法
     */
    void printSkills() override;

    /**
     * @brief 獲取該角色擁有的技能總數
     * @return 技能數量 (精英怪物為 4)
     */
    int getSkillCount() const override { return 4; }
};

#endif
