#include "GameManager.h"

GameManager::GameManager() : stage(1) {}
int GameManager::getStage() const { return stage; }

void GameManager::showHTMLUpdate(Role& player, Role& monster) {
    // 模擬將數據傳給 HTML 介面
    cout << "\n[HTML 介面更新] 關卡:" << stage
        << " | " << player.getName() << " HP:" << player.getHp()
        << " vs " << monster.getName() << " HP:" << monster.getHp() << endl;
}

bool GameManager::battle(Role& player, Role& monster) {
    while (player.isAlive() && monster.isAlive()) {
        showHTMLUpdate(player, monster);
        int action = 0;
        cout << "選擇動作: (1)普通攻擊 (2)使用技能 > ";
        cin >> action;
        if (action == 1) {
            player.attack(monster);
        } else {
            int sid = 1;
            cout << "選擇技能 (1-3) > ";
            cin >> sid;
            if(sid<1 || sid>3) sid = 1;
            player.useSkill(monster, sid);
        }
        if (monster.isAlive()) {
            // monster chooses a random skill
            int mskill = (rand()%3)+1;
            monster.useSkill(player, mskill);
        }
    }
    if (player.isAlive()) {
        cout << "\n[Victory] " << player.getName() << " defeated " << monster.getName() << "!" << endl;
        return true;
    } else {
        cout << "\n[Defeat] " << player.getName() << " was defeated." << endl;
        return false;
    }
}
