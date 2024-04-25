#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>

unsigned char buffer[32];
const char *menuItems[] = { "AAAAAAAAAAAA", "BBBBBBB", "CCCCCCC", "DDDDDDDDDDD"};
const uint8_t menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
uint8_t currentSelection = 0;
uint8_t previousSelection = 0xFF; // 初期値は無効な選択肢に設定

void drawMenu() {
    printf("IP:192.168.1.2", "SSID");
    for (uint8_t i = 0; i < menuSize; i++) {
        printf("  %s\n", menuItems[i]); // 全ての項目を通常状態で描画
    }
}

void redrawSelection() {
    if (previousSelection != 0xFF) {
        printf("\x1B[%d;1H  %s\n", previousSelection + 1, menuItems[previousSelection]); // 前回の選択を通常状態で再描画
    }
    printf("\x1B[%d;1H> %s\n", currentSelection + 1, menuItems[currentSelection]); // 現在の選択をハイライトして再描画
}

void updateSelection(uint8_t joypadState) {
    previousSelection = currentSelection;
    if (joypadState & J_UP) {
        if (currentSelection > 0) currentSelection--;
    } else if (joypadState & J_DOWN) {
        if (currentSelection < menuSize - 1) currentSelection++;
    }
}

void main(void) {
    DISPLAY_ON; // ゲームボーイのディスプレイをオンにする
    drawMenu(); // 初期メニュー描画
    redrawSelection(); // 初期選択をハイライト

    while(1) {
        uint8_t joypadState = joypad();
        if (joypadState & (J_UP | J_DOWN)) {
            updateSelection(joypadState);
            redrawSelection(); // 選択が変更された場合のみ矢印を移動
        }
        delay(100); // デバウンスのための小さな遅延
    }
}
