#pragma once
class Player {
public:
    int x, y, width, height;
    int speed;
    int gh;

    Player() {
        x = 320;
        y = 550;
        speed = 10;
        gh = LoadGraph("Player.png");
        width = 80;
        height = 20;
    }

    void Move() {
        if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640) {
            x += speed;
        }
        if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
            x -= speed;
        }
    }

    void View() {
        DrawExtendGraph(x, y, x + width, y + height, gh, TRUE);
    }

    void All() {
        Move();
        View();
    }

};