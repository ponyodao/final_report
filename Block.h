#pragma once
class Block {
public:

    int x, y;
    int width, height;
    int gh;
    bool live;

    void View() {
        if (live) {
            DrawGraph(x, y, gh, TRUE);
        }
    }

    Block(int setX, int setY) {
        x = setX;
        y = setY;
        gh = LoadGraph("Block.png");
        GetGraphSize(gh, &width, &height);
        live = true;
    }

    void All() {
        View();
    }
};