#include "DxLib.h"
#include "file_setting.h"
#include "structed.h"
#include "Block.h"
#include "Player.h"
#include "Ball.h"


float score;

void getCurrentDirectory(char* currentDirectory) {
    GetCurrentDirectory(CHARBUFF, currentDirectory);
}

class GameControl {
public:
    Block* bl[BLOCK_NUM];
    Player* pl;
    Ball* ba;


    int state;
    int titlegh = LoadGraph("Title.png");
    int gameovergh = LoadGraph("GameOver.png");
    int cleargh = LoadGraph("Clear.png");
    bool pushFlag;


    char keyValue[CHARBUFF];

    bool PushSpace() {
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            if (!pushFlag) {
                pushFlag = true;
                return true;
            }
        }
        else {
            pushFlag = false;
        }
        return false;
    }

    GameControl() {
        life = 2;
        state = 0;

        pushFlag = false;

        for (int i = 0; i < BLOCK_NUM; i++) {
            bl[i] = new Block(140 + (i % 4) * 100, 10 + (i / 4) * 50);
        }
        pl = new Player();
        ba = new Ball();
    }

    ~GameControl() {
        for (int i = 0; i < BLOCK_NUM; i++) {
            delete bl[i];
        }
        delete pl;
        delete ba;
    }

    void Title() {
        DrawGraph(0, 0, titlegh, TRUE);
        if (PushSpace()) {
            state = 1;
            ba->x = 320;
            ba->y = 300;
        }
    }

    void Game() {
        for (int i = 0; i < BLOCK_NUM; i++) {
            bl[i]->All();
        }
        pl->All();
        ba->All();

        for (int i = 0; i < life; i++) {//rest life 
            DrawCircle(20 + i * (life + ba->r + 10), 20, ba->r, GetColor(255, 255, 255));
        }

        //DrawFormatString(600, 400, white.color, "%d", score);


        for (int i = 0; i < BLOCK_NUM; i++) {//judge game clear
            if (bl[i]->live)break;
            if (i == BLOCK_NUM - 1) {
                ba->vecX = 0;
                ba->vecY = 0;
                state = 3;
            }
        }

        if (ba->vecX != 0 && life > 0) {//hit bar and wall, and ceiling
            if (ba->x > WINDOW_X)ba->vecX = -1;
            if (ba->x < 0)ba->vecX = 1;
            if (ba->y < 0)ba->vecY = 1;
            if (ba->x > pl->x && ba->x<pl->x + pl->width && ba->y + ba->r > pl->y) {
                ba->vecY = -1;
            }
            if (ba->y > WINDOW_Y) {//mistake
                ba->x = 320;
                ba->y = 300;
                ba->vecX = 0;
                ba->vecY = 0;
                life--;
            }
            for (int i = 0; i < BLOCK_NUM; i++) { //judge block crash
                if (bl[i]->live) {
                    if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
                        ba->y + ba->r > bl[i]->y && ba->y + ba->r < bl[i]->y + bl[i]->height) {//from up
                        bl[i]->live = false;
                        ba->vecY *= -1;
                        score += 100;

                    }
                    if (ba->x > bl[i]->x && ba->x < bl[i]->x + bl[i]->width &&
                        ba->y - ba->r > bl[i]->y && ba->y - ba->r < bl[i]->y + bl[i]->height) {//from down
                        bl[i]->live = false;
                        ba->vecY *= -1;
                        score += 10;

                    }
                    if (ba->x + ba->r > bl[i]->x && ba->x + ba->r < bl[i]->x + bl[i]->width &&
                        ba->y > bl[i]->y && ba->y < bl[i]->y + bl[i]->height) {//from left
                        bl[i]->live = false;
                        ba->vecX *= -1;
                        score += 10;

                    }
                    if (ba->x - ba->r > bl[i]->x && ba->x - ba->r < bl[i]->x + bl[i]->width &&
                        ba->y > bl[i]->y && ba->y < bl[i]->y + bl[i]->height) {//from right
                        bl[i]->live = false;
                        ba->vecX *= -1;
                        score += 10;

                    }
                }
            }
        }
        else if (life > 0) {//restart
            DrawFormatString(260, 360, GetColor(255, 255, 255), "PUSH SPACE");
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                ba->vecX = 1;
                ba->vecY = 1;
            }
        }
        else {//gameover
            state = 2;
        }


    }

    void GameOver() {
        DrawGraph(0, 0, gameovergh, TRUE);
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            state = 0;
            life = 2;
            for (int i = 0; i < BLOCK_NUM; i++) {
                bl[i]->live = true;
            }
        }
        error = fopen_s(&fp, "testOut.txt", "w");
        if (error != 0)
            fprintf_s(stderr, "failed to open");
        else {
            fprintf(fp, "score:%lf\n", score);
        }

    }

    void GameClear() {
        DrawGraph(0, 0, cleargh, TRUE);
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            state = 0;
            life = 2;
            for (int i = 0; i < BLOCK_NUM; i++) {
                bl[i]->live = true;
            }
        }
        error = fopen_s(&fp, "testOut.txt", "w");
        if (error != 0)
            fprintf_s(stderr, "failed to open");
        else {
            fprintf(fp, "score:%lf\n", score);
        }
    }

    void All() {
        if (state == 0)Title();
        if (state == 1)Game();
        if (state == 2)GameOver();
        if (state == 3)GameClear();
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    ChangeWindowMode(TRUE);
    SetGraphMode(WINDOW_X, WINDOW_Y, 32);
    if (DxLib_Init() == -1)return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    GameControl* ga = new GameControl();

    char currentDirectory[CHARBUFF];
    getCurrentDirectory(currentDirectory);
    char section[CHARBUFF];
    sprintf_s(section, "Data1"); //iniファイル読み込みスタート
    char keyWord[CHARBUFF];
    sprintf_s(keyWord, "life");
    char settingFile[CHARBUFF];
    sprintf_s(settingFile, "%s\\data.ini", currentDirectory); //真ん中はファイル名

    while (ProcessMessage() != -1) {
        int startTime = GetNowCount();
        ScreenFlip();
        ClearDrawScreen();

        ga->All();

        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
        int endTime = GetNowCount();
        WaitTimer((1000 / 60) - (endTime - startTime));
    }



    delete ga;

    DxLib_End();
    return 0;
}