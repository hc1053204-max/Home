#ifndef MONSTER_H
#define MONSTER_H
#include "Role.h"
#include <string>
using namespace std;

/**
 * @brief Monster 類別：怪物角色，繼承自 Role
 * 
 * 怪物由 GameManager 隨機生成，擁有隨機的生命值與攻擊力。
 */
class Monster:
    public Role{
public:
    /**
     * @brief 建構子
     * @param n 怪物名稱
     * @param h 生命值
     * @param a 攻擊力
     */
    Monster(string n,int h,int a);

    /**
     * @brief 覆寫技能方法 (怪物技能較為單一)
     * skillId: 1=狂暴衝鋒, 2=撕咬, 3=咆哮
     * @param target 技能目標
     * @param skillId 技能 ID
     */
    void useSkill(Role& target,int skillId)override;

    /**
     * @brief 覆寫獲取技能名稱方法
     * @param skillId 技能 ID
     * @return 技能名稱
     */
    string getSkillName(int skillId) const override;

    /**
     * @brief 覆寫打印技能列表方法
     */
    void printSkills()override;
    int getSkillCount() const override { return 3; }
};
#endif
