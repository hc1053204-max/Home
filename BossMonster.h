#ifndef BOSSMONSTER_H
#define BOSSMONSTER_H
#include "Monster.h"
#include <string>
using namespace std;

/**
 * @brief BossMonster 類別：Boss 級怪物，繼承自 Monster
 * 
 * Boss 怪物擁有極高的生命值與毀滅性的攻擊技能，是每個關卡的最終挑戰。
 */
class BossMonster : public Monster {
public:
    /**
     * @brief 建構子
     * @param n Boss 名稱
     * @param h 生命值
     * @param a 攻擊力
     */
    BossMonster(string n, int h, int a);

    /**
     * @brief 覆寫技能方法 (提供毀滅性的技能集)
     * skillId: 1=毀滅衝鋒, 2=吞噬撕咬, 3=終焉咆哮, 4=毀滅之光
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
     * @return 技能數量 (Boss 怪物為 4)
     */
    int getSkillCount() const override { return 4; }
};

#endif
