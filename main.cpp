#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Paladin.h"
#include "Monster.h"
#include "EliteMonster.h"
#include "BossMonster.h"
#include "GameManager.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iostream>
#include <limits>
#include "Utils.h"
using namespace std;

// 遊戲平衡常數
const int MAX_STAGE=5;             // 最大關卡數
const int BASE_MONSTER_COUNT=2;    // 基礎小怪數量
const int BASE_MONSTER_HP=30;      // 基礎小怪血量
const int BASE_MONSTER_ATK=8;      // 基礎小怪攻擊力

/**
 * @brief 遊戲主進入點
 * 流程：職業選擇 -> 關卡循環 (擊敗小怪 -> 擊敗 Boss) -> 遊戲結果
 */
int main(){
    // 強制終端使用 UTF-8 編碼，解決 Windows 終端中文亂碼問題
    system("chcp 65001 > nul");
    
    // 初始化隨機種子
    srand((unsigned)time(nullptr));
    
    Utils::clearScreen();
    Utils::printBoxHeader("CHARACTER SELECTION", '#');
    
    // --- 職業選擇階段 ---
    int choice;
    while (true) {
        choice = Utils::getSafeInt("Choose Class: (1)Warrior (2)Mage (3)Archer (4)Paladin > ");
        if (choice >= 1 && choice <= 4) break;
        cout << Utils::COLOR_RED << "Invalid choice, please try again\n" << Utils::COLOR_RESET;
    }
    
    // 使用智能指針管理角色，實現多型
    unique_ptr<Role> hero;
    if(choice==1) hero=make_unique<Warrior>("Warrior");
    else if(choice==2) hero=make_unique<Mage>("Mage");
    else if(choice==3) hero=make_unique<Archer>("Archer");
    else hero=make_unique<Paladin>("Paladin");
    
    GameManager gm;
    int stage=1;
    
    // --- 遊戲主循環 (關卡進展) ---
    while(hero->isAlive() && stage<=MAX_STAGE){
        // 同步當前關卡到 GameManager 以更新界面
        gm.setStage(stage);
        
        Utils::clearScreen();
        Utils::printBoxHeader("STAGE " + to_string(stage) + " BEGINS", '#');
        
        // 生成小怪
        vector<unique_ptr<Monster>> smalls;
        int smallCount = BASE_MONSTER_COUNT + stage;
        for (int i = 0; i < smallCount; i++) {
            string name = "Minion" + to_string(i + 1);
            int hp = BASE_MONSTER_HP + stage * 15;
            int atk = BASE_MONSTER_ATK + stage * 3;

            if ((rand() % 100) < 20) { // 20% 機率生成精英怪
                smalls.push_back(make_unique<EliteMonster>("Elite " + name, (int)(hp * 1.5), (int)(atk * 1.2)));
            } else {
                smalls.push_back(make_unique<Monster>(name, hp, atk));
            }
        }

        // 擊敗小怪
        bool stageWon = true;
        for (auto& m : smalls) {
            bool won = gm.battle(*hero, *m);
            if (!won) {
                cout << Utils::COLOR_RED << "\nFAILED TO CLEAR THE STAGE!\n" << Utils::COLOR_RESET << endl;
                stageWon = false;
                break;
            }
            hero->gainXp(30 + stage * 10);
        }
        
        if(!hero->isAlive()) break;
        if(!stageWon) break;
        
        // Boss 戰
        BossMonster boss("StageBoss_"+to_string(stage), 100 + stage * 70, 15 + stage * 10);
        bool bossWon=gm.battle(*hero,boss);
        if(bossWon){
            // 擊敗 Boss 獲得大量經驗與隨機物品獎勵
            hero->gainXp(100+stage*50);
            
            int roll = rand() % 100;
            if (roll < 40) {
                hero->addItem(make_unique<Item>("神聖之劍", ItemType::WEAPON, 10 + stage * 5, 0, 100));
            } else if (roll < 80) {
                hero->addItem(make_unique<Item>("鋼鐵胸甲", ItemType::ARMOR, 0, 50 + stage * 20, 80));
            } else {
                hero->addItem(make_unique<Item>("高級恢復藥水", ItemType::CONSUMABLE, 0, 100, 50));
            }

            // 休息階段：管理背包與裝備
            bool resting = true;
            while(resting){
                Utils::clearScreen();
                Utils::printBoxHeader("REST AREA", '#');
                cout << "1. 查看背包與裝備\n2. 更換裝備\n3. 使用消耗品\n4. 進入下一關\n";
                int rChoice = Utils::getSafeInt("選擇行動 > ");
                if(rChoice == 1) {
                    hero->showInventory();
                }
                else if(rChoice == 2) {
                    hero->showInventory();
                    int idx = Utils::getSafeInt("選擇要裝備的物品索引 (或 0 返回) > ");
                    if(idx != 0) hero->equipItem(idx);
                }
                else if(rChoice == 3) {
                    hero->showInventory();
                    int idx = Utils::getSafeInt("選擇要使用的消耗品索引 (或 0 返回) > ");
                    if(idx != 0) hero->useConsumable(idx);
                }
                else if(rChoice == 4) {
                    resting = false;
                }
                
                if(rChoice != 4) {
                    cout << "\n按 Enter 鍵繼續...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
            }
            stage++; // 晉級到下一關
        }
        else{
            cout << Utils::COLOR_RED << "\nTHE BOSS HAS CRUSHED YOU!\n" << Utils::COLOR_RESET;
            break;
        }
    }
    
    // --- 遊戲結束判定 ---
    Utils::clearScreen();
    if(hero->isAlive() && stage > MAX_STAGE){
        Utils::printBoxHeader("CONGRATULATIONS!", '=');
        Utils::printCentered("You have conquered all stages!", 40);
        Utils::printLine('=', 40);
    }
    else{
        Utils::printBoxHeader("GAME OVER", 'X');
        Utils::printCentered("Better luck next time...", 40);
        Utils::printLine('X', 40);
    }
    
    return 0;
}
