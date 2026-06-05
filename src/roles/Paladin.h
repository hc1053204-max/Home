#ifndef PALADIN_H
#define PALADIN_H
#include "Role.h"
#include <string>
using namespace std;

/**
 * @brief Paladin 類別：聖騎士職業，繼承自 Role
 * 
 * 聖騎士特點：極高的生存能力（最高初始 HP），兼具攻擊與自我治療能力。
 */
class Paladin : public Role {
public:
    /**
     * @brief 建構子
     * @param n 聖騎士名稱
     */
    Paladin(string n);

    /**
     * @brief 覆寫技能方法
     * skillId: 1=神聖打擊, 2=神聖治癒, 3=審判
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
     * @return 技能數量 (聖騎士為 3)
     */
    int getSkillCount() const override { return 3; }
};

#endif
