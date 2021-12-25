# Coloretto-Game
[C++] 變色龍遊戲 2018.11

\#改版更新中

## File
- `main.cpp` : 主要程式
- `Coloretto.h` : Coloretto class 定義
- `Player.h` : Player (使用者, 電腦玩家) class 定義

## Class
### Coloretto 牌面
關於牌的一切操作都在這個 class 裡

### Player
用繼承多開一個 `ComputerPlayer` 給電腦玩家，從 `play_game()` 函式之後可以新增 電腦玩家 的玩牌邏輯
