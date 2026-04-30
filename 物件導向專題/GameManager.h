#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Role.h"

class GameManager {
private:
    int stage;
public:
    GameManager();
    // battle returns true if player won
    bool battle(Role& player, Role& monster);
    void showHTMLUpdate(Role& player, Role& monster);
    int getStage() const;
};
#endif