#ifndef MAGE_H
#define MAGE_H
#include "Role.h"
#include <string>
using namespace std;

/**
 * @brief Mage 類別：法師職業，繼承自 Role
 * 
 * 法師特點：低生命值但極高攻擊力，技能以強大的魔法傷害為主。
 */
class Mage:
    public Role{
public:
    /**
     * @brief 建構子
     * @param n 法師名稱
     */
    Mage(string n);

    /**
     * @brief 覆寫技能方法
     * skillId: 1=火球術, 2=冰凍, 3=流星火雨
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
