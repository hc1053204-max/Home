#include "GameManager.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include "Utils.h"
using namespace std;

GameManager::GameManager():stage(1){}
int GameManager::getStage()const{
    return stage;
}
void GameManager::setStage(int s){
    stage = s;
}
void GameManager::showHTMLUpdate(Role&player,Role&monster){
    int width = 60;
    cout << "\n" << Utils::COLOR_BLUE;
    Utils::printLine('-', width);
    cout << "| " << left << setw(10) << "Stage " + to_string(stage) 
         << " | " << right << setw(45) << "Battle in Progress" << " |" << endl;
    Utils::printLine('-', width);
    
    // Player stats with HP Bar
    cout << "| " << left << setw(12) << "PLAYER" << ": " 
         << left << setw(12) << player.getName() 
         << " " << Utils::generateHPBar(player.getHp(), player.getMaxHp(), 15) 
         << " " << right << setw(3) << player.getHp() << "/" << setw(3) << player.getMaxHp() 
         << " |" << endl;
         
    // Monster stats with HP Bar
    cout << "| " << left << setw(12) << "MONSTER" << ": " 
         << left << setw(12) << monster.getName() 
         << " " << Utils::generateHPBar(monster.getHp(), monster.getMaxHp(), 15) 
         << " " << right << setw(3) << monster.getHp() << "/" << setw(3) << monster.getMaxHp() 
         << " |" << endl;
         
    Utils::printLine('-', width);
    cout << Utils::COLOR_RESET << endl;
}
bool GameManager::battle(Role&player,Role& monster){
    Utils::clearScreen();
    Utils::printBoxHeader("BATTLE START", '=');
    cout << Utils::COLOR_BOLD << player.getName() << " VS " << monster.getName() << Utils::COLOR_RESET << endl;
    Utils::printLine(' ', 40);

    while(player.isAlive()&&monster.isAlive()){
        // 每回合開始前清理畫面，保持狀態面板在頂部
        Utils::clearScreen();
        showHTMLUpdate(player,monster);
        
        // 玩家回合
        int action;
        while (true) {
            action = Utils::getSafeInt("\nChoose action: (1) Attack (2) Use Skill > ");
            if (action == 1 || action == 2) break;
            cout << Utils::COLOR_RED << "Invalid choice, enter 1 or 2" << Utils::COLOR_RESET << endl;
        }
        if(action==1){
            player.attack(monster);
        }
        else if(action==2){
                player.printSkills();
                int sid;
            while (true) {
                sid = Utils::getSafeInt("Choose skill (1-3) > ");
                if (sid >= 1 && sid <= 3) break;
                cout << Utils::COLOR_RED << "Invalid skill id, enter 1-3" << Utils::COLOR_RESET << endl;
            }
                player.useSkill(monster,sid);
            }
        // 檢查怪物是否還活著
        if(!monster.isAlive()){
            break;
        }
        // 怪物回合
        cout << "\n" << Utils::COLOR_YELLOW << "[Monster Turn]" << Utils::COLOR_RESET << endl;
        int mskill=(rand()%3)+1;
        cout << monster.getName() << " uses " << Utils::COLOR_YELLOW << monster.getSkillName(mskill) << Utils::COLOR_RESET << endl;
        monster.useSkill(player,mskill);
    }
    // 戰鬥結果
    Utils::printLine(' ', 40);
    Utils::printBoxHeader("BATTLE END", '=');
    if(player.isAlive()){
        cout << Utils::COLOR_GREEN << Utils::COLOR_BOLD << "[Victory] " << player.getName() << " defeated " << monster.getName() << "!" << Utils::COLOR_RESET << endl;
        return true;
    }
    else{
        cout << Utils::COLOR_RED << Utils::COLOR_BOLD << "[Defeat] " << player.getName() << " was defeated by " << monster.getName() << "!" << Utils::COLOR_RESET << endl;
        return false;
    }
}
