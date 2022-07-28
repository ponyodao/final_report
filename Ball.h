#pragma once
class Ball {
public:
    int x, y, r;
    int vecX, vecY;
    int speed;

    Ball() {
        x = 320;
        y = 300;
        r = 5;
        vecX = 0;
        vecY = 0;
        speed = 5;
    }

    void Move() {
        x += speed * vecX;
        y += speed * vecY;
    }

    void View() {
        DrawCircle(x, y, r, GetColor(255, 255, 255));
    }

    void All() {
        Move();
        View();
    }
};
