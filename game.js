// ============================================
// 遊戲配置
// ============================================
const ROLES = {
    'Warrior': {
        name: '戰士',
        icon: '⚔️',
        hp: 120,
        atk: 15,
        skills: {
            1: { name: '斬擊', damage: 25, cost: 0 },
            2: { name: '盾擊', damage: 18, cost: 0 },
            3: { name: '狂戰', damage: 35, cost: 0 }
        }
    },
    'Mage': {
        name: '法師',
        icon: '🔥',
        hp: 80,
        atk: 22,
        skills: {
            1: { name: '火球術', damage: 30, cost: 0 },
            2: { name: '冰凍', damage: 20, cost: 0 },
            3: { name: '流星火雨', damage: 40, cost: 0 }
        }
    },
    'Archer': {
        name: '弓箭手',
        icon: '🏹',
        hp: 100,
        atk: 18,
        skills: {
            1: { name: '連射', damage: 24, cost: 0 },
            2: { name: '穿透箭', damage: 32, cost: 0 },
            3: { name: '箭雨', damage: 28, cost: 0 }
        }
    }
};

// 3關完整配置
const STAGE_CONFIG = {
    1: { 
        name: '第一關 - 新手村',
        description: '在新手村消滅所有敵人',
        smallMonsters: 2,
        smallMonsterHp: 40,
        smallMonsterAtk: 8,
        bossName: '村長的守衛隊長',
        bossIcon: '👿',
        bossHp: 100,
        bossAtk: 12
    },
    2: { 
        name: '第二關 - 黑暗森林',
        description: '在黑暗森林中打敗所有怪物',
        smallMonsters: 3,
        smallMonsterHp: 60,
        smallMonsterAtk: 12,
        bossName: '森林守護獸',
        bossIcon: '👿',
        bossHp: 150,
        bossAtk: 15
    },
    3: { 
        name: '第三關 - 龍之城堡',
        description: '進入城堡，擊敗最終的龍王Boss！',
        smallMonsters: 4,
        smallMonsterHp: 80,
        smallMonsterAtk: 15,
        bossName: '龍王 - 最終Boss',
        bossIcon: '🐉',
        bossHp: 200,
        bossAtk: 18
    }
};

const MAX_STAGE = 3;

// ============================================
// 遊戲狀態
// ============================================
let gameState = {
    selectedRole: null,
    roleData: null,
    player: null,
    monster: null,
    currentStage: 1,
    currentMonsterIndex: 0,
    totalMonstersInStage: 0,
    inBattle: false,
    actionInProgress: false,
    defeatedEnemiesInStage: 0,
    totalDefeatedEnemies: 0,
    bossFightActive: false
};

// ============================================
// 工具函數
// ============================================

function showScreen(screenId) {
    document.querySelectorAll('.screen').forEach(screen => {
        screen.classList.remove('active');
    });
    document.getElementById(screenId).classList.add('active');
}

function addLog(message, type = 'system') {
    const logContent = document.getElementById('logContent');
    const logItem = document.createElement('p');
    logItem.className = `log-item ${type}`;
    logItem.textContent = message;
    logContent.appendChild(logItem);
    logContent.scrollTop = logContent.scrollHeight;
}

function createDamageNumber(damage, isHeal = false, isCritical = false) {
    const container = document.getElementById('damageNumbers');
    const damageNum = document.createElement('div');
    damageNum.className = `damage-number ${isHeal ? 'heal' : 'damage'} ${isCritical ? 'critical' : ''}`;
    damageNum.textContent = (isHeal ? '+' : isCritical ? '暴擊 ' : '-') + damage;
    
    // 隨機位置
    const x = Math.random() * 400 + 300;
    const y = Math.random() * 300 + 400;
    damageNum.style.left = x + 'px';
    damageNum.style.top = y + 'px';
    
    container.appendChild(damageNum);
    
    setTimeout(() => damageNum.remove(), 1000);
}

function updatePlayerUI() {
    const hpPercent = (gameState.player.hp / gameState.player.maxHp) * 100;
    const hpBar = document.getElementById('playerHpBar');
    hpBar.style.width = hpPercent + '%';
    
    if (hpPercent < 30) {
        hpBar.classList.add('low');
    } else {
        hpBar.classList.remove('low');
    }
    
    document.getElementById('playerHpText').textContent = 
        gameState.player.hp + '/' + gameState.player.maxHp;
    document.getElementById('playerLevelDisplay').textContent = gameState.player.level;
    document.getElementById('playerAtk').textContent = gameState.player.atk;
    document.getElementById('playerXp').textContent = 
        gameState.player.xp + '/' + gameState.player.xpToNext;
}

function updateMonsterUI() {
    const hpPercent = (gameState.monster.hp / gameState.monster.maxHp) * 100;
    const hpBar = document.getElementById('monsterHpBar');
    hpBar.style.width = hpPercent + '%';
    
    if (hpPercent < 30) {
        hpBar.classList.add('low');
    } else {
        hpBar.classList.remove('low');
    }
    
    document.getElementById('monsterHpText').textContent = 
        gameState.monster.hp + '/' + gameState.monster.maxHp;
}

function calculateDamage(attacker, isSkill = false, skillId = 1) {
    let baseDamage = attacker.atk;
    if (isSkill) {
        baseDamage = attacker.skills[skillId].damage;
    }
    
    // 隨機波動 (±20%)
    const variance = 0.8 + Math.random() * 0.4;
    const damage = Math.floor(baseDamage * variance);
    
    return Math.max(damage, 1);
}

function disableActions(disabled = true) {
    gameState.actionInProgress = disabled;
    document.getElementById('attackBtn').disabled = disabled;
    document.getElementById('skillBtn').disabled = disabled;
}

// ============================================
// 職業選擇
// ============================================

function selectRole(roleName, element) {
    gameState.selectedRole = roleName;
    const roleConfig = ROLES[roleName];
    
    gameState.roleData = {
        name: roleConfig.name,
        icon: roleConfig.icon,
        hp: roleConfig.hp,
        maxHp: roleConfig.hp,
        atk: roleConfig.atk,
        level: 1,
        xp: 0,
        xpToNext: 100,
        skills: roleConfig.skills
    };
    
    gameState.player = { ...gameState.roleData };
    
    // UI 反饋
    document.querySelectorAll('.card').forEach(card => {
        card.style.opacity = '0.2';
    });
    element.style.opacity = '1';
    
    // 更新技能菜單
    updateSkillMenu();
    
    // 延遲開始遊戲
    setTimeout(() => {
        startNewGame();
    }, 800);
}

function updateSkillMenu() {
    for (let i = 1; i <= 3; i++) {
        const skill = gameState.player.skills[i];
        document.getElementById(`skill${i}Name`).textContent = skill.name;
        document.getElementById(`skill${i}Dmg`).textContent = `傷害: ${skill.damage}`;
    }
}

// ============================================
// 遊戲開始
// ============================================

function startNewGame() {
    gameState.currentStage = 1;
    gameState.totalDefeatedEnemies = 0;
    
    // 更新玩家 UI
    document.getElementById('playerName').textContent = gameState.player.name;
    document.getElementById('playerIcon').textContent = gameState.player.icon;
    document.getElementById('playerNameBattle').textContent = gameState.player.name;
    
    startStage();
}

function startStage() {
    const stageConfig = STAGE_CONFIG[gameState.currentStage];
    gameState.totalMonstersInStage = stageConfig.smallMonsters;
    gameState.currentMonsterIndex = 0;
    gameState.defeatedEnemiesInStage = 0;
    gameState.bossFightActive = false;
    
    // 更新舞台資訊
    document.getElementById('stageName').textContent = stageConfig.name;
    document.getElementById('stageDesc').textContent = stageConfig.description;
    
    addLog(`\n🗡️ ${stageConfig.name} 開始！`, 'system');
    addLog(`本關需要擊敗 ${stageConfig.smallMonsters} 隻小怪和 1 個 Boss！`, 'system');
    
    // 開始第一場戰鬥
    startNextBattle();
}

function startNextBattle() {
    const stageConfig = STAGE_CONFIG[gameState.currentStage];
    gameState.currentMonsterIndex++;
    
    // 判斷是小怪還是 Boss
    const isBoss = (gameState.currentMonsterIndex > stageConfig.smallMonsters);
    gameState.bossFightActive = isBoss;
    
    if (isBoss) {
        // Boss 戰
        gameState.monster = {
            name: stageConfig.bossName,
            icon: stageConfig.bossIcon,
            hp: stageConfig.bossHp,
            maxHp: stageConfig.bossHp,
            atk: stageConfig.bossAtk,
            level: gameState.currentStage + 1,
            isBoss: true,
            skills: createBossSkills()
        };
        
        document.getElementById('stageProgress').textContent = 
            `⚡ Boss戰 - 最終決戰`;
        
        addLog(`\n⚠️ Boss 出現！`, 'system');
        addLog(`${gameState.monster.name} 現身了！`, 'skill');
    } else {
        // 小怪戰
        gameState.monster = {
            name: `小怪 ${gameState.currentMonsterIndex}/${stageConfig.smallMonsters}`,
            icon: '👹',
            hp: stageConfig.smallMonsterHp + gameState.currentStage * 10,
            maxHp: stageConfig.smallMonsterHp + gameState.currentStage * 10,
            atk: stageConfig.smallMonsterAtk + gameState.currentStage * 2,
            level: gameState.currentStage,
            isBoss: false,
            skills: createMonsterSkills()
        };
        
        document.getElementById('stageProgress').textContent = 
            `小怪 ${gameState.currentMonsterIndex}/${stageConfig.smallMonsters}`;
        
        addLog(`\n小怪出現了！第 ${gameState.currentMonsterIndex} 個小怪！`, 'system');
    }
    
    // 每場新戰鬥不恢復血量（只在升級時恢復）
    gameState.inBattle = true;
    
    // 更新怪物 UI
    document.getElementById('monsterName').textContent = gameState.monster.name;
    document.getElementById('monsterIcon').textContent = gameState.monster.icon;
    document.getElementById('monsterAtk').textContent = gameState.monster.atk;
    
    // 重置 UI
    hideSkillMenu();
    disableActions(false);
    updatePlayerUI();
    updateMonsterUI();
    
    showScreen('battleScreen');
}

function createMonsterSkills() {
    return {
        1: { name: '普通攻擊', damage: 15, type: 'attack' },
        2: { name: '重擊', damage: 25, type: 'attack' },
        3: { name: '狂暴', damage: 35, type: 'attack' }
    };
}

function createBossSkills() {
    return {
        1: { name: '力量一擊', damage: 25, type: 'attack' },
        2: { name: '地獄猛擊', damage: 35, type: 'attack' },
        3: { name: '毀滅之力', damage: 45, type: 'attack' }
    };
}

// ============================================
// 玩家行動
// ============================================

function playerAttack() {
    if (gameState.actionInProgress) return;
    
    disableActions(true);
    
    const damage = calculateDamage(gameState.player);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - damage);
    
    addLog(`${gameState.player.name} 攻擊，造成 ${damage} 點傷害！`, 'damage');
    createDamageNumber(damage);
    updateMonsterUI();
    
    setTimeout(() => {
        if (gameState.monster.hp <= 0) {
            battleEnd(true);
        } else {
            monsterTurn();
        }
    }, 800);
}

function showSkillMenu() {
    const skillMenu = document.getElementById('skillMenu');
    skillMenu.style.display = skillMenu.style.display === 'none' ? 'block' : 'none';
}

function hideSkillMenu() {
    document.getElementById('skillMenu').style.display = 'none';
}

function playerUseSkill(skillId) {
    if (gameState.actionInProgress) return;
    
    disableActions(true);
    hideSkillMenu();
    
    const skill = gameState.player.skills[skillId];
    const damage = calculateDamage(gameState.player, true, skillId);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - damage);
    
    addLog(`${gameState.player.name} 使用【${skill.name}】，造成 ${damage} 點傷害！`, 'skill');
    createDamageNumber(damage);
    updateMonsterUI();
    
    setTimeout(() => {
        if (gameState.monster.hp <= 0) {
            battleEnd(true);
        } else {
            monsterTurn();
        }
    }, 800);
}

// ============================================
// 怪物行動
// ============================================

function monsterTurn() {
    addLog(`${gameState.monster.name} 發動攻擊...`);
    
    setTimeout(() => {
        const skillId = Math.floor(Math.random() * 3) + 1;
        const skill = gameState.monster.skills[skillId];
        const damage = calculateDamage(gameState.monster, true, skillId);
        
        gameState.player.hp = Math.max(0, gameState.player.hp - damage);
        
        addLog(`${gameState.monster.name} 使用【${skill.name}】，造成 ${damage} 點傷害！`, 'damage');
        createDamageNumber(damage);
        updatePlayerUI();
        
        setTimeout(() => {
            // 玩家HP歸零 - 遊戲立即結束
            if (gameState.player.hp <= 0) {
                gameOver();
            } else {
                disableActions(false);
            }
        }, 800);
    }, 500);
}

// ============================================
// 戰鬥結束邏輯
// ============================================

function battleEnd(playerWon) {
    gameState.inBattle = false;
    disableActions(true);
    
    setTimeout(() => {
        if (!playerWon) {
            // 玩家死亡 - 立即遊戲結束
            gameOver();
            return;
        }
        
        // 玩家勝利
        const isBoss = gameState.monster.isBoss;
        const reward = 30 + gameState.currentStage * 10;
        
        gameState.player.xp += reward;
        gameState.defeatedEnemiesInStage++;
        gameState.totalDefeatedEnemies++;
        
        let levelUpMessage = '';
        let didLevelUp = false;
        
        // 檢查升級
        if (gameState.player.xp >= gameState.player.xpToNext) {
            gameState.player.level++;
            gameState.player.xpToNext = Math.floor(gameState.player.xpToNext * 1.2);
            gameState.player.maxHp += 10;
            gameState.player.hp = Math.min(gameState.player.hp + 20, gameState.player.maxHp);
            gameState.player.atk += 5;
            levelUpMessage = `\n⭐ 升級到 Lv.${gameState.player.level}！\n❤️ HP +20 | 💪 ATK +5`;
            didLevelUp = true;
        }
        
        // Boss 戰利品
        if (isBoss) {
            const atkBonus = 5 + gameState.currentStage * 2;
            gameState.player.atk += atkBonus;
            levelUpMessage += `\n🎁 Boss掉落：攻擊力飾品 +${atkBonus}`;
        }
        
        showBattleResultScreen(playerWon, reward, levelUpMessage, isBoss);
    }, 800);
}

function showBattleResultScreen(won, reward, extraMessage, isBoss) {
    const resultScreen = document.getElementById('resultScreen');
    const resultIcon = document.getElementById('resultIcon');
    const resultTitle = document.getElementById('resultTitle');
    const resultDetails = document.getElementById('resultDetails');
    const nextBtn = document.getElementById('nextBtn');
    const restartBtn = document.getElementById('restartBtn');
    
    resultIcon.textContent = '🎉';
    resultTitle.textContent = '戰鬥勝利！';
    
    if (isBoss) {
        resultDetails.innerHTML = `
            <p>✨ 你擊敗了 ${gameState.monster.name}！</p>
            <p>獲得 ${reward} XP</p>
            <p>❤️ 當前 HP: ${gameState.player.hp}/${gameState.player.maxHp}</p>
            ${extraMessage}
        `;
    } else {
        resultDetails.innerHTML = `
            <p>你擊敗了 ${gameState.monster.name}！</p>
            <p>獲得 ${reward} XP</p>
            <p>❤️ 當前 HP: ${gameState.player.hp}/${gameState.player.maxHp}</p>
            ${extraMessage}
        `;
    }
    
    nextBtn.style.display = 'block';
    restartBtn.style.display = 'none';
    
    if (isBoss && gameState.currentStage >= MAX_STAGE) {
        nextBtn.textContent = '🏆 查看通關結果';
    } else if (isBoss) {
        nextBtn.textContent = '進入下一關';
    } else {
        nextBtn.textContent = '繼續戰鬥';
    }
    
    showScreen('resultScreen');
}

function nextBattle() {
    const stageConfig = STAGE_CONFIG[gameState.currentStage];
    
    // 檢查是否完成本關（擊敗所有小怪 + Boss）
    if (gameState.bossFightActive) {
        // 剛剛擊敗了 Boss
        if (gameState.currentStage >= MAX_STAGE) {
            // 所有關都完成 - 通關！
            showGameWinScreen();
            return;
        } else {
            // 進入下一關
            gameState.currentStage++;
            startStage();
            return;
        }
    }
    
    // 繼續本關的戰鬥（還有更多小怪或 Boss）
    startNextBattle();
}

function showGameWinScreen() {
    const finalStats = document.getElementById('finalStats');
    finalStats.innerHTML = `
        <p>🏆 恭喜通關所有 3 關！</p>
        <p>━━━━━━━━━━━━━━━━</p>
        <p>最終等級：Lv.${gameState.player.level}</p>
        <p>最終攻擊力：${gameState.player.atk}</p>
        <p>最終 HP：${gameState.player.maxHp}</p>
        <p>累計經驗：${gameState.player.xp}</p>
        <p>擊敗敵人總數：${gameState.totalDefeatedEnemies}</p>
    `;
    
    showScreen('completeScreen');
}

function gameOver() {
    gameState.inBattle = false;
    disableActions(true);
    
    const gameOverStats = document.getElementById('gameOverStats');
    gameOverStats.innerHTML = `
        <p>到達階段：第 ${gameState.currentStage} 關</p>
        <p>小怪擊敗數：${gameState.defeatedEnemiesInStage}</p>
        <p>總敵人擊敗數：${gameState.totalDefeatedEnemies}</p>
        <p>最終等級：Lv.${gameState.player.level}</p>
        <p>最終攻擊力：${gameState.player.atk}</p>
    `;
    
    showScreen('gameOverScreen');
}

function restartGame() {
    // 重置遊戲狀態
    gameState = {
        selectedRole: null,
        roleData: null,
        player: null,
        monster: null,
        currentStage: 1,
        currentMonsterIndex: 0,
        totalMonstersInStage: 0,
        inBattle: false,
        actionInProgress: false,
        defeatedEnemiesInStage: 0,
        totalDefeatedEnemies: 0,
        bossFightActive: false
    };
    
    // 重置 UI
    document.querySelectorAll('.card').forEach(card => {
        card.style.opacity = '1';
    });
    
    document.getElementById('logContent').innerHTML = '';
    document.getElementById('skillMenu').style.display = 'none';
    
    showScreen('selectionScreen');
}

// ============================================
// 初始化
// ============================================

document.addEventListener('DOMContentLoaded', () => {
    showScreen('selectionScreen');
});
