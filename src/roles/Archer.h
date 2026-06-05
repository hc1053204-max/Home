#ifndef ARCHER_H
#define ARCHER_H
#include "Role.h"
#include <string>
using namespace std;

/**
 * @brief Archer 類別：弓箭手職業，繼承自 Role
 * 
 * 弓箭手特點：高攻擊力，擅長遠程快速攻擊與高爆發傷害。
 */
class Archer:
    public Role{
public:
    /**
     * @brief 建構子
     * @param n 弓箭手名稱
     */
    Archer(string n);

    /**
     * @brief 覆寫技能方法
     * skillId: 1=連射, 2=穿透箭, 3=箭雨
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
    int getSkillCount() const override { return 4; }
};
#endif
