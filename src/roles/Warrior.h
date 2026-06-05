#ifndef WARRIOR_H
#define WARRIOR_H
#include "Role.h"
#include <string>
using namespace std;

/**
 * @brief Warrior 類別：戰士職業，繼承自 Role
 * 
 * 戰士特點：高生命值、高生存能力，技能以強力單體攻擊與自我恢復為主。
 */
class Warrior:
    public Role{
public:
    /**
     * @brief 建構子
     * @param n 戰士名稱
     */
    Warrior(string n);

    /**
     * @brief 覆寫技能方法
     * skillId: 1=斬擊, 2=盾擊, 3=狂戰
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
