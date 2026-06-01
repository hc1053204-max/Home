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

const STAGE_CONFIG = {
    1: { monsters: 2, monsterHp: 40, monsterAtk: 8 },
    2: { monsters: 3, monsterHp: 60, monsterAtk: 12 },
    3: { monsters: 4, monsterHp: 80, monsterAtk: 15 }
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
    defeatedEnemies: 0
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

function createDamageNumber(damage, isHeal = false) {
    const container = document.getElementById('damageNumbers');
    const damageNum = document.createElement('div');
    damageNum.className = `damage-number ${isHeal ? 'heal' : 'damage'}`;
    damageNum.textContent = (isHeal ? '+' : '-') + damage;
    
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
    document.getElementById('playerLevel').textContent = gameState.player.level;
    document.getElementById('playerAtk').textContent = gameState.player.atk;
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
    
    // 添加隨機變異 (±20%)
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
        card.style.opacity = '0.3';
    });
    element.style.opacity = '1';
    
    // 延遲開始遊戲
    setTimeout(() => {
        startNewStage();
    }, 500);
}

// ============================================
// 戰鬥系統
// ============================================

function startNewStage() {
    gameState.currentStage = 1;
    gameState.defeatedEnemies = 0;
    gameState.currentMonsterIndex = 0;
    
    // 更新玩家 UI
    document.getElementById('playerName').textContent = gameState.player.name;
    document.getElementById('playerIcon').textContent = gameState.player.icon;
    
    startNextBattle();
}

function startNextBattle() {
    const stageConfig = STAGE_CONFIG[gameState.currentStage];
    
    gameState.totalMonstersInStage = stageConfig.monsters;
    gameState.currentMonsterIndex++;
    
    // 生成怪物
    const monsterIndex = gameState.currentMonsterIndex;
    const isBoss = (monsterIndex === stageConfig.monsters);
    
    if (isBoss) {
        gameState.monster = {
            name: `Stage ${gameState.currentStage} Boss`,
            icon: '👹',
            hp: 80 + gameState.currentStage * 60,
            maxHp: 80 + gameState.currentStage * 60,
            atk: 12 + gameState.currentStage * 8,
            level: gameState.currentStage + 1,
            isBoss: true,
            skills: createMonsterSkills()
        };
    } else {
        gameState.monster = {
            name: `小怪 ${monsterIndex}`,
            icon: '👹',
            hp: stageConfig.monsterHp + gameState.currentStage * 10,
            maxHp: stageConfig.monsterHp + gameState.currentStage * 10,
            atk: stageConfig.monsterAtk + gameState.currentStage * 2,
            level: gameState.currentStage,
            isBoss: false,
            skills: createMonsterSkills()
        };
    }
    
    // 重置玩家血量（新戰鬥）
    gameState.player.hp = gameState.player.maxHp;
    
    gameState.inBattle = true;
    
    // 更新舞台信息
    document.getElementById('stageName').textContent = 
        `Stage ${gameState.currentStage}${gameState.monster.isBoss ? ' - BOSS' : ''}`;
    document.getElementById('stageProgress').textContent = 
        `${gameState.currentMonsterIndex}/${gameState.totalMonstersInStage}`;
    
    // 更新怪物 UI
    document.getElementById('monsterName').textContent = gameState.monster.name;
    document.getElementById('monsterIcon').textContent = gameState.monster.icon;
    document.getElementById('monsterAtk').textContent = gameState.monster.atk;
    
    // 清空日誌
    document.getElementById('logContent').innerHTML = '';
    addLog(`戰鬥開始！${gameState.player.name} vs ${gameState.monster.name}`);
    
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

// ============================================
// 玩家行動
// ============================================

function playerAttack() {
    if (gameState.actionInProgress) return;
    
    disableActions(true);
    
    const damage = calculateDamage(gameState.player);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - damage);
    
    addLog(`${gameState.player.name} 攻擊 ${gameState.monster.name}，造成 ${damage} 點傷害！`, 'damage');
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
    
    addLog(`${gameState.player.name} 使用 【${skill.name}】，造成 ${damage} 點傷害！`, 'skill');
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
    addLog(`${gameState.monster.name} 的回合...`);
    
    setTimeout(() => {
        const skillId = Math.floor(Math.random() * 3) + 1;
        const skill = gameState.monster.skills[skillId];
        const damage = calculateDamage(gameState.monster, true, skillId);
        
        gameState.player.hp = Math.max(0, gameState.player.hp - damage);
        
        addLog(`${gameState.monster.name} 使用 【${skill.name}】，造成 ${damage} 點傷害！`, 'damage');
        createDamageNumber(damage);
        updatePlayerUI();
        
        setTimeout(() => {
            if (gameState.player.hp <= 0) {
                battleEnd(false);
            } else {
                disableActions(false);
            }
        }, 800);
    }, 500);
}

// ============================================
// 戰鬥結束
// ============================================

function battleEnd(playerWon) {
    gameState.inBattle = false;
    disableActions(true);
    
    setTimeout(() => {
        if (playerWon) {
            const reward = 30 + gameState.currentStage * 10;
            
            gameState.player.xp += reward;
            gameState.defeatedEnemies++;
            
            let levelUpMessage = '';
            if (gameState.player.xp >= gameState.player.xpToNext) {
                gameState.player.level++;
                gameState.player.xpToNext = Math.floor(gameState.player.xpToNext * 1.2);
                gameState.player.maxHp += 10;
                gameState.player.hp = gameState.player.maxHp;
                gameState.player.atk += 5;
                levelUpMessage = `\n⭐ 升級到 Lv.${gameState.player.level}！`;
            }
            
            // Boss 戰利品
            if (gameState.monster.isBoss) {
                const atkBonus = 5 + gameState.currentStage * 2;
                gameState.player.atk += atkBonus;
                levelUpMessage += `\n🎁 獲得掉落：攻擊力飾品 +${atkBonus}`;
            }
            
            showResultScreen(true, reward, levelUpMessage);
        } else {
            showResultScreen(false, 0, '');
        }
    }, 500);
}

function showResultScreen(won, reward, extraMessage) {
    const resultScreen = document.getElementById('resultScreen');
    const resultIcon = document.getElementById('resultIcon');
    const resultTitle = document.getElementById('resultTitle');
    const resultDetails = document.getElementById('resultDetails');
    const nextBtn = document.getElementById('nextBtn');
    const restartBtn = document.getElementById('restartBtn');
    
    if (won) {
        resultIcon.textContent = '🎉';
        resultTitle.textContent = '勝利！';
        resultDetails.innerHTML = `
            <p>你擊敗了 ${gameState.monster.name}！</p>
            <p>獲得 ${reward} XP</p>
            ${extraMessage}
        `;
        
        nextBtn.style.display = 'block';
        restartBtn.style.display = 'none';
        
        // 檢查是否完成本階段
        if (gameState.currentMonsterIndex >= gameState.totalMonstersInStage) {
            nextBtn.textContent = gameState.currentStage >= MAX_STAGE ? '查看結果' : '下一階段';
        } else {
            nextBtn.textContent = '下一場戰鬥';
        }
    } else {
        resultIcon.textContent = '💀';
        resultTitle.textContent = '失敗...';
        resultDetails.innerHTML = `
            <p>你被 ${gameState.monster.name} 擊敗了...</p>
            <p>已擊敗敵人：${gameState.defeatedEnemies}</p>
            <p>到達階段：${gameState.currentStage}</p>
        `;
        
        nextBtn.style.display = 'none';
        restartBtn.style.display = 'block';
    }
    
    showScreen('resultScreen');
}

function nextBattle() {
    if (gameState.currentMonsterIndex >= gameState.totalMonstersInStage) {
        // 本階段完成
        if (gameState.currentStage >= MAX_STAGE) {
            showCompleteScreen();
            return;
        }
        
        gameState.currentStage++;
        gameState.currentMonsterIndex = 0;
    }
    
    startNextBattle();
}

function showCompleteScreen() {
    const finalStats = document.getElementById('finalStats');
    finalStats.innerHTML = `
        <p>最終等級：Lv.${gameState.player.level}</p>
        <p>最終攻擊力：${gameState.player.atk}</p>
        <p>最終 HP：${gameState.player.maxHp}</p>
        <p>累計經驗：${gameState.player.xp}</p>
    `;
    
    showScreen('completeScreen');
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
        defeatedEnemies: 0
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
