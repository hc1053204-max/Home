#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Role.h"
class GameManager{
private:
    int stage;
    // 私有方法：顯示戰鬥介面
    void showHTMLUpdate(Role& player,Role& monster);
public:
    GameManager();
    // battle returns true if player won
    bool battle(Role& player, Role&monster);
    // Getter
    int getStage()const;
    void setStage(int s);
};
#endif