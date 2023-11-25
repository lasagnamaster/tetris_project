#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <vector>
using namespace sf;

///////// Field /////////
const int LENGTH = 10;
const int HEIGHT = 20;

int field[HEIGHT][LENGTH] = {0};

const int shapes[7][4]{
    1,3,5,7, // I 0
    2,4,5,7, // Z 1
    3,5,4,6, // S 2
    3,5,4,7, // T 3
    2,5,3,7, // L 4
    3,5,7,6, // J 5
    2,3,4,5, // 0 6
};

struct Point {
    int x, y;
} a[4], b[4];

///////// BOX //////////

class Box {
private:
    int color{ 0 };
    Sprite pic;
    Texture texture;
    int atBorders;
    bool lowest = false;

public:
    Box() {};
    Box(int x, int y, int color);
    ~Box() {};

    void initTexture();
    void draw(RenderTarget& window, int extX, int extY);
    int atBordersCheck();
    bool atBottomCheck();
    bool atBottomCheck(bool collisionCheck);

    int x{ 0 };
    int y{ 0 };
    bool dead = false;
};

std::vector<Box*> boxes;
std::vector<Box*> frozen_boxes;

Box::Box(int x, int y, int color): x(x), y(y), color(color) {
    initTexture();
}

void Box::initTexture() {
    texture.loadFromFile("images/tiles32.png");
    pic.setTexture(texture);
    pic.setTextureRect(IntRect(color * 32, 0, 32, 32));
}

void Box::draw(RenderTarget& window, int extX, int extY) {
    pic.setPosition(extX + x * 32, extY + y * 32);
    window.draw(pic);
}

int Box::atBordersCheck() {
    if (x == 0 or field[y][x - 1] != 0) {
        atBorders = -1; return -1;
    }
    else if (x == 9 or field[y][x+1]) { atBorders = 1; return 1; }
    else { atBorders = 0; return 0; }
}

bool Box::atBottomCheck() {
    
    if (y+1 >= HEIGHT or field[y+1][x]!=0) {
        return true;
    }
    return false;
}

bool Box::atBottomCheck(bool collusionCheck) {

    if (field[y][x] != 0) {
        return true;
    }
    return false;
}

////////// TETROMINO ////////////

class Tetromino {
private:
    int x{ 4 };
    int y{ 0 };
    int num{ 0 };
    Box *data[4];
    bool moveableRN = true;
    int rotateNum = 0;
    int lines_sum = 0;

    int extX{ 40 };
    int extY{ 40 };

public:
    Tetromino() {};
    Tetromino(int color, int num);
    Tetromino(int color, int num, int extX, int extY);
    ~Tetromino() { for (int i = 0; i < 4; i++) { data[i]->dead = true;
    frozen_boxes.clear();
    } }

    void move(const int dirX);
    float move(float time, const float delay);
    int vnePolya();

    void rotate();
    void draw(RenderTarget& window);
    void mapUpdate();

    int getNum() { return num; }

    bool timeToDie = false;
    bool moveableAtAll = true;

};

void Tetromino::mapUpdate() {
    field[HEIGHT][LENGTH] = { 0 };
    for (int i = 0; i < boxes.size(); i++) {
        field[boxes[i]->y][boxes[i]->x] = 1;
    }
    int k = HEIGHT - 1;
    int lines = 0;
    int lines_num[4];
    for (int i = k; i > 0; i--) { 
        int count = 0;

        // updating the field when the lines are cleared

        for (int j = 0; j < LENGTH; j++) {
            if (field[i][j]) count++;
            field[k][j] = field[i][j];
        }
        if (count < LENGTH) k--;

        //deleting the boxes when the lines are cleared

        else {
            lines_num[lines] = i;
            lines++;
            for (int p = boxes.size() - 1; p >= 0; p--) {
                if (boxes[p]->y == i) {
                    delete boxes[p];
                    boxes.erase(boxes.begin() + p);
                }
            }
        }
    }

    // pushing the rest of them down

    for (int j = lines-1; j >= 0; j--) {
        for (int p = 0; p < boxes.size(); p++) {
            if (boxes[p]->y <= lines_num[j]) {
                boxes[p]->y += 1;
            }
        }
    }
}

Tetromino::Tetromino(int color, int num) : num(num) {
    for (int i = 0; i < 4; i++) {
        int x_temp = shapes[num][i] % 2 + x;
        int y_temp = shapes[num][i] / 2 + y;
        a[i].x = x_temp; a[i].y = y_temp;
        data[i] = new Box(x_temp, y_temp, num);
        boxes.push_back(data[i]);
    }
}

Tetromino::Tetromino(int color, int num, int EextX, int EextY) : num(num) {
    extX = EextX - x * 32, extY = EextY - y * 32; 
    moveableAtAll = false;
    for (int i = 0; i < 4; i++) {
        int x_temp = shapes[num][i] % 2 + x;
        int y_temp = shapes[num][i] / 2 + y;
        a[i].x = x_temp; a[i].y = y_temp;
        data[i] = new Box(x_temp, y_temp, num);
        frozen_boxes.push_back(data[i]);
    }
}

void Tetromino::draw(RenderTarget& window) {
    if (moveableAtAll) {
        for (int i = 0; i < boxes.size(); i++) {
            boxes[i]->draw(window, extX, extY);
        }
    }
    else {
        for (int i = 0; i < frozen_boxes.size(); i++) {
            frozen_boxes[i]->draw(window, extX, extY);
        }
    }

};

void Tetromino::move(const int dirX) {
    moveableRN = true;
    for (int i = 0; i < 4; i++) {
        if (data[i]->atBordersCheck() == -1 and dirX == -1) moveableRN = false; //checking if we're at borders (left)
        else if (data[i]->atBordersCheck() == 1 and dirX == 1) moveableRN = false; //right
    };
    if (moveableRN) {
        for (int i = 0; i < 4; i++) { 
            data[i]->x += dirX; 
            a[i].x += dirX;
        }
    }
}

float Tetromino::move(float timer, const float delay) {
    bool atBottom = false; //Checking if every box at bottom

        //if we're in air we move lower
    for (int i = 0; i < 4; i++) {
        if (data[i]->atBottomCheck()) { atBottom = true; timeToDie = true; }
    }
    if (!atBottom) {
        for (int i = 0; i < 4; i++) {
            data[i]->y += 1;
            a[i].y += 1;
        }
        timer = 0;
    }
    if (atBottom) {
        mapUpdate();
    }
    
    return timer;

}

void Tetromino::rotate() {
    if (num != 6) {
        int cx = data[1]->x;
        int cy = data[1]->y;
        Point p = a[1];
        int needToMoveX = 0;
        int needToMoveY = 0;
        rotateNum += 1;
        for (int i = 0; i < 4; i++) { b[i].x = 0; b[i].y = 0; }
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            int x1 = data[i]->y - cy;
            int y1 = data[i]->x - cx;
            data[i]->x = cx - x1;
            data[i]->y = cy + y1;

            int x1_a = a[i].y - p.y;
            int y1_a = a[i].x - p.x;
            a[i].x = p.x - x1_a; a[i].y = p.y + y1_a;

            if (rotateNum == 4) rotateNum -= 4;
            if (num == 0) {
                if (rotateNum == 2) { needToMoveY = 1; }
                else if (rotateNum == 3) { needToMoveX = -1; }
                else if (rotateNum == 0) { needToMoveY = -1; }
                else if (rotateNum == 1) { needToMoveX = 1; };
            }
        }
        

        if (needToMoveX) {
            for (int i = 0; i < 4; i++) {
                data[i]->x += needToMoveX;
                a[i].x += needToMoveX;
            }
        }
        if (needToMoveY) {
            for (int i = 0; i < 4; i++) {
                data[i]->y += needToMoveY;
                a[i].y += needToMoveY;
            }
        }

        while (vnePolya()) {
            int dx = vnePolya();
            for (int i = 0; i < 4; i++) {
                data[i]->x += dx;
                a[i].x += dx;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (data[i]->atBottomCheck(true)) {
                for (int j = 0; j < 4; j++) {
                    a[j] = b[j];
                    data[j]->x = a[j].x; data[j]->y = a[j].y;
                }
                break;
            }
        }
    }
}

int Tetromino::vnePolya() {
    for (int i = 0; i < 4; i++) {
        if (data[i]->x < 0) return 1;
        else if (data[i]->x > 9) return -1;
    }
    return 0;
}

////////// ADDITIONAL FUNCTIONS ///////////
