#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "../roles/role.h"

/**
 * @brief GameManager 類別：控制遊戲流程與戰鬥邏輯
 * 
 * 負責管理當前關卡、處理玩家與怪物的回合制戰鬥以及更新戰鬥界面。
 */
class GameManager{
private:
    int stage; // 當前關卡

    /**
     * @brief 更新並顯示戰鬥狀態面板（包含 HP 條與近期日誌）
     * @param player 玩家角色
     * @param monster 怪物角色
     */
    void showHTMLUpdate(Role& player,Role& monster);

public:
    /**
     * @brief 建構子
     */
    GameManager();

    /**
     * @brief 執行一場戰鬥
     * @param player 玩家角色
     * @param monster 怪物角色
     * @return 玩家獲勝則回傳 true，否則回傳 false
     */
    bool battle(Role& player, Role&monster);

    // --- Getter & Setter ---
    int getStage()const;
    void setStage(int s);
};
#endif
