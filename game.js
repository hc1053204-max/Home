// 1. 初始化遊戲數據（模擬從 C++ GameManager 接收到的初始狀態）
const gameState = {
    stage: 1,
    player: { name: "勇者", hp: 150, maxHp: 150, level: 1 },
    monster: { name: "森林史萊姆", hp: 50, maxHp: 50 },
    isBattleOver: false
};

// 2. 更新畫面顯示 (視覺設計：實作動態血條)
function updateUI() {
    // 更新血條寬度
    const playerHpPercent = (gameState.player.hp / gameState.player.maxHp) * 100;
    const monsterHpPercent = (gameState.monster.hp / gameState.monster.maxHp) * 100;

    const pBar = document.getElementById('player-hp');
    const mBar = document.getElementById('monster-hp');

    pBar.style.width = playerHpPercent + "%";
    mBar.style.width = monsterHpPercent + "%";

    // UX 強化：血量低下變色邏輯
    if (playerHpPercent < 30) pBar.classList.add('hp-low');
    else pBar.classList.remove('hp-low');
    
    if (monsterHpPercent < 30) mBar.classList.add('hp-low');
    else mBar.classList.remove('hp-low');
}

// 3. 戰鬥日誌系統 (使用者體驗：畫面回饋感)
function logMessage(message, type = 'system') {
    const logContainer = document.getElementById('battle-log');
    const entry = document.createElement('p');
    entry.className = `log-${type}`;
    entry.textContent = `[${new Date().toLocaleTimeString()}] ${message}`;
    logContainer.prepend(entry); // 最新的訊息排在最上面
}

// 4. 模擬戰鬥動作 (對應 C++ 的 battle 邏輯)
function playerAttack() {
    if (gameState.isBattleOver) return;

    // 模擬計算傷害 (C++ OOP 核心邏輯的簡化版)
    const dmg = 15 + Math.floor(Math.random() * 10);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - dmg);
    
    logMessage(`玩家發動攻擊！造成 ${dmg} 傷害`, 'attack');
    updateUI();

    if (gameState.monster.hp <= 0) {
        logMessage(`勝利！擊敗了 ${gameState.monster.name}`, 'victory');
        gameState.isBattleOver = true;
        triggerStageClear(); // 觸發關卡切換
    } else {
        // 延遲一段時間後怪物反擊
        setTimeout(monsterTurn, 800);
    }
}

// 5. 背景切換邏輯 (視覺設計：背景圖切換)
function triggerStageClear() {
    gameState.stage++;
    // 透過變更 body class 切換背景 (對應 style.css)
    document.body.className
