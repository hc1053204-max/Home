#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Monster.h"
#include "GameManager.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iostream>
#include <limits>
#include "Utils.h"
using namespace std;

// 定義常數
const int MAX_STAGE=3;
const int BASE_MONSTER_COUNT=2;
const int BASE_MONSTER_HP=30;
const int BASE_MONSTER_ATK=8;

int main(){
    // 強制終端使用 UTF-8 編碼，解決中文亂碼問題
    system("chcp 65001 > nul");
    
    srand((unsigned)time(nullptr));
    
    Utils::clearScreen();
    Utils::printBoxHeader("CHARACTER SELECTION", '#');
    // 選擇職業
    int choice;
    while (true) {
        choice = Utils::getSafeInt("Choose Class: (1)Warrior (2)Mage (3)Archer > ");
        if (choice >= 1 && choice <= 3) break;
        cout << Utils::COLOR_RED << "Invalid choice, please try again\n" << Utils::COLOR_RESET;
    }
    
    unique_ptr<Role>hero;
    if(choice==1)hero=make_unique<Warrior>("Warrior");
    else if(choice==2)hero=make_unique<Mage>("Mage");
    else hero=make_unique<Archer>("Archer");
    
    GameManager gm;
    int stage=1;
    
    while(hero->isAlive()&& stage<=MAX_STAGE){
        // 同步當前關卡到 GameManager
        gm.setStage(stage);
        
        Utils::clearScreen();
        Utils::printBoxHeader("STAGE " + to_string(stage) + " BEGINS", '#');
        
        // 生成小怪
        vector<Monster> smalls;
        int smallCount=BASE_MONSTER_COUNT+stage;
        for(int i=0;i<smallCount;i++){
            smalls.emplace_back("Minion" + to_string(i + 1),BASE_MONSTER_HP+stage*10,BASE_MONSTER_ATK+stage*2);
        }
        
        // 擊敗小怪
        bool stageWon=true;
        for(auto&m:smalls){
            bool won=gm.battle(*hero,m);
            if(!won){
                cout << Utils::COLOR_RED << "\nFAILED TO CLEAR THE STAGE!\n" << Utils::COLOR_RESET;
                stageWon=false;
                break;
            }
            hero->gainXp(30+stage*10);
        }
        
        if(!hero->isAlive())break;
        if(!stageWon)break;
        
        // Boss 戰
        Monster boss("StageBoss_"+to_string(stage),80+stage*60,12+stage* 8);
        bool bossWon=gm.battle(*hero,boss);
        if(bossWon){
            hero->gainXp(100+stage*50);
            int atkBonus=5+stage*2;
            cout << Utils::COLOR_MAGENTA << "[ITEM] Received: Attack Accessory (+" << atkBonus << " ATK)" << Utils::COLOR_RESET << endl;
            hero->addAtk(atkBonus);
            stage++;
        }
        else{
            cout << Utils::COLOR_RED << "\nTHE BOSS HAS CRUSHED YOU!\n" << Utils::COLOR_RESET;
            break;
        }
    }
    
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
}
