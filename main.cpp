#include "src/roles/Warrior.h"
#include "src/roles/Mage.h"
#include "src/roles/Archer.h"
#include "src/roles/Paladin.h"
#include "src/monsters/Monster.h"
#include "src/monsters/EliteMonster.h"
#include "src/monsters/BossMonster.h"
#include "src/core/GameManager.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iostream>
#include <limits>
#include "src/core/Utils.h"
using namespace std;

// 遊戲平衡常數
// 這些常數定義了遊戲的基礎難易度，修改這些數值可快速調整遊戲體驗
const int MAX_STAGE=5;             // 最大關卡數：決定遊戲通關條件
const int BASE_MONSTER_COUNT=2;    // 基礎小怪數量：每關最少出現的數量
const int BASE_MONSTER_HP=30;      // 基礎小怪血量：等級 1 時的基礎生命
const int BASE_MONSTER_ATK=8;      // 基礎小怪攻擊力：等級 1 時的基礎傷害

/**
 * @brief 遊戲主進入點
 * 
 * 整個遊戲採取「線性關卡推進」模式，生命週期如下：
 * [初始化] -> [角色選擇] -> [關卡循環] -> [結果判定]
 * 
 * 關卡內部邏輯：
 * 1. 隨機生成怪物群 (含 20% 精英怪) -> 玩家依次挑戰
 * 2. 挑戰關卡 Boss (最高難度目標)
 * 3. 勝利後進入 [休息區域] (管理物品、裝備、恢復生命)
 * 4. 晉級下一關，直到達到 MAX_STAGE 或角色死亡
 */
int main(){
    // 強制終端使用 UTF-8 編碼，解決 Windows 終端中文亂碼問題
    system("chcp 65001 > nul");
    
    // 初始化隨機種子，確保每次遊戲的怪物屬性與掉落物品不同
    srand((unsigned)time(nullptr));
    
    Utils::clearScreen();
    Utils::printBoxHeader("角色選擇", '#');
    
    // --- 職業選擇階段 ---
    // 採用簡單的數字選單，透過 Utils::getSafeInt 確保輸入的魯棒性
    int choice;
    while (true) {
        choice = Utils::getSafeInt("選擇角色: (1)Warrior (2)Mage (3)Archer (4)Paladin > ");
        if (choice >= 1 && choice <= 4) break;
        cout << Utils::COLOR_RED << "無效選擇，請輸入 1 到 4" << Utils::COLOR_RESET;
    }
    
    // 使用 std::unique_ptr 管理角色對象，利用多型 (Polymorphism) 
    // 讓後續的戰鬥邏輯無需關心具體職業，只需調用 Role 的虛擬函式
    unique_ptr<Role> hero;
    if(choice==1) hero=make_unique<Warrior>("Warrior");
    else if(choice==2) hero=make_unique<Mage>("Mage");
    else if(choice==3) hero=make_unique<Archer>("Archer");
    else hero=make_unique<Paladin>("Paladin");
    
    GameManager gm;
    int stage=1;
    
    // --- 遊戲主循環 (關卡進展) ---
    while(hero->isAlive() && stage<=MAX_STAGE){
        // 同步當前關卡到 GameManager 以更新界面顯示
        gm.setStage(stage);
        
        Utils::clearScreen();
        Utils::printBoxHeader("STAGE " + to_string(stage) + " BEGINS", '#');
        
        // 1. 生成小怪群
        // 屬性隨關卡線性成長：HP = 基礎 + stage * 20, ATK = 基礎 + stage * 3
        vector<unique_ptr<Monster>> smalls;
        int smallCount = BASE_MONSTER_COUNT + stage;
        for (int i = 0; i < smallCount; i++) {
            string name = "Minion" + to_string(i + 1);
            int hp = BASE_MONSTER_HP + stage * 20;
            int atk = BASE_MONSTER_ATK + stage * 3;

            // 隨機性設計：20% 機率將普通怪替換為精英怪 (屬性加成 1.5x/1.2x)
            if ((rand() % 100) < 20) { 
                smalls.push_back(make_unique<EliteMonster>("Elite " + name, (int)(hp * 1.5), (int)(atk * 1.2)));
            } else {
                smalls.push_back(make_unique<Monster>(name, hp, atk));
            }
        }
        
        // 2. 依次挑戰小怪
        bool stageWon = true;
        for (auto& m : smalls) {
            bool won = gm.battle(*hero, *m);
            if (!won) {
                cout << Utils::COLOR_RED << "\n失敗清除關卡！\n" << Utils::COLOR_RESET << endl;
                stageWon = false;
                break;
            }
            // 每擊敗一隻小怪獲得經驗值，促使玩家等級提升
            hero->gainXp(30 + stage * 10);
        }
        
        if(!hero->isAlive()) break;
        if(!stageWon) break;
        
        // 3. 挑戰關卡 Boss
        // Boss 屬性遠高於小怪，設計為該關卡的「門檻」
        BossMonster boss("Boss_"+to_string(stage), 100 + stage * 70, 15 + stage * 8);
        bool bossWon=gm.battle(*hero,boss);
        if(bossWon){
            // 擊敗 Boss 獎勵：大量經驗值 + 隨機物品
            hero->gainXp(100+stage*50);
            
            // 隨機物品掉落池：武器(40%) / 防具(40%) / 消耗品(20%)
            int roll = rand() % 100;
            if (roll < 40) {
                hero->addItem(make_unique<Item>("神聖之劍", ItemType::WEAPON, 10 + stage * 5, 0, 100));
            } else if (roll < 80) {
                hero->addItem(make_unique<Item>("鋼鐵胸甲", ItemType::ARMOR, 0, 50 + stage * 20, 80));
            } else {
                hero->addItem(make_unique<Item>("高級恢復藥水", ItemType::CONSUMABLE, 0, 100, 50));
            }
		
            // --- 休息階段 (Rest Area) ---
            // 提供玩家管理資源的時間，避免戰鬥過於緊湊導致疲勞
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
            
            // 關卡間恢復：恢復 50% 最大生命值，確保玩家有能力面對下一關
            hero->heal(hero->getMaxHp() / 2);
            
            stage++; // 晉級
        }
        else{
            cout << Utils::COLOR_RED << "\nBoss 已打敗你！\n" << Utils::COLOR_RESET;
            break;
        }
    }
    
    // --- 最終結果判定 ---
    Utils::clearScreen();
    if(hero->isAlive() && stage > MAX_STAGE){
        Utils::printBoxHeader("恭喜！", '=');
        Utils::printCentered("你已征服了所有關卡！", 40);
        Utils::printLine('=', 40);
    }
    else{
        Utils::printBoxHeader("遊戲結束", 'X');
        Utils::printCentered("你已失敗，請再試一次！", 40);
        Utils::printLine('X', 40);
    }
    
    return 0;
}
