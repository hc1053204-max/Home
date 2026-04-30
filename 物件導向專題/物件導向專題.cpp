#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Monster.h"
#include "GameManager.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iostream>
using namespace std;

int main() {
    srand((unsigned)time(nullptr));
    int choice;
    cout << "選擇職業:(1)戰士(2)法師(3)射手:";
    cin >> choice;
    Role* hero = nullptr;
    if (choice == 1)hero = new Warrior("戰士");
    else if (choice == 2)hero = new Mage("法師");
    else hero = new Archer("射手");

    GameManager gm;
    const int maxStage = 3;
    int stage = 1;
    while (hero->isAlive() && stage <= maxStage) {
        int s = stage;
        cout << "\n=== Stage " << s << " 開始 ===" << endl;
        // spawn small monsters
        vector<Monster> smalls;
        int smallCount = 2 + s; // more monsters on later stages
        for (int i = 0;i < smallCount;i++) {
            smalls.emplace_back("小怪" + to_string(i + 1), 30 + s * 10, 8 + s * 2);
        }

        // fight small monsters one by one
        for (auto& m : smalls) {
            bool won = gm.battle(*hero, m);
            if (!won) break;
            // reward xp for small monster
            hero->gainXp(30 + s * 10);
        }
        if (!hero->isAlive()) break;

        // boss
        Monster boss("StageBoss_" + to_string(s), 80 + s * 60, 12 + s * 8);
        bool bossWon = gm.battle(*hero, boss);
        if (bossWon) {
            // boss rewards: xp + equipment (simple atk bonus)
            hero->gainXp(100 + s * 50);
            cout << "獲得掉落：攻擊力飾品 (+" << 5 + s * 2 << ")" << endl;
            hero->addAtk(5 + s * 2);
            // advance to next stage
            stage++;
        }
        else {
            break;
        }
    }

    if (hero->isAlive()) {
        cout << "\n恭喜，你通關所有關卡！" << endl;
    }
    else {
        cout << "\n遊戲結束，請再接再厲。" << endl;
    }

    delete hero;
}