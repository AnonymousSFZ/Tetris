#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#define SIZE            20
#define TIMEBONUS       20
#define ROCKSCORE       5
#define CLEARBONUS      50
#define NORM_SPEED      2
#define HIGH_SPEED      4
#define TITLE_TEXTSIZE  40
#define TITLE_MARGIN    10
#define SIDEBAR_WIDTH   200

typedef struct
{
    int top_x;          // border of the rock
    int top_y;
    int length;
    int height;
    int pattern;  // bit pattern of the rock
    PIMAGE img;
}ROCK;

enum DIRECTION
{
    LEFT, RIGHT
};

extern PIMAGE scene;
extern long score;

void welcomePage();
void randomRock(ROCK&);
void initScene();
void updateScene();
void reset();

void drawRock(int dstX, int dstY, ROCK &rockTmp);
void initRocks();

bool bottomCheck();
bool borderCheck(enum DIRECTION);

void updateRock();
void rotateRock(enum DIRECTION);
