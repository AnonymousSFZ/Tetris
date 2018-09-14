/* Name: Tetris                     */
/* version: 0.4                     */
/* Last Modification: 2018/9/11     */
/* Note: 加入了背景音乐，           */
/*       修复各种越界               */

#include "tetris.h"

ROCK rock, rockNext;
short gameMap[480/SIZE][400/SIZE]={0};
static int releaseFlag=1, banSpeed=0, fpsCnt=0, pauseFlag=0;
static time_t timeBegin;

/* Successfully Tested */
void initScene()
{
    int titleWidth;

    scene=newimage();
    setfont(TITLE_TEXTSIZE, 0, "宋体");
    titleWidth=textwidth("T");

    PlaySound(TEXT("bgm.wav"), NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);

    outtextrect(TITLE_MARGIN,
                TITLE_MARGIN,
                TITLE_MARGIN+titleWidth,//=SIZE
                TITLE_MARGIN+6*TITLE_TEXTSIZE,
                "TETRIS");
    line(2*TITLE_MARGIN+titleWidth, 1,
         2*TITLE_MARGIN+titleWidth, ege::getheight()-1);
    line(ege::getwidth()-SIDEBAR_WIDTH, 1,
         ege::getwidth()-SIDEBAR_WIDTH, ege::getheight()-1);
    setfont(25, 0, "宋体");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(540, 30, "Time");
    outtextxy(540, 130, "Score");
    getimage(scene, 0, 0, 640, 480);

    score=0;
    timeBegin=time(NULL);
}

/* Partly--Successfully Tested */
void updateScene()
{
    int rockCnt, lineClear=0, clearCnt;
    int row=480/SIZE, col=400/SIZE;
    char timeStr[10], scoreStr[10];
    time_t timeCurrent, gameTime;

    // show next rock
    outtextxy(540, 240, "What's Next");
    if(rockNext.pattern==0xf000)
        putimage(500, 280, rockNext.img);
    else
        putimage(510, 280, rockNext.img);

    // update time
    timeCurrent=time(NULL);
    gameTime=timeCurrent-timeBegin;
    sprintf(timeStr, "%02d:%02d", gameTime/60, gameTime%60);
    outtextxy(540, 60, timeStr);

    // update gameMap
    for(int i=row-1; i>=0; i--)
    {
        clearCnt=0;
        rockCnt=0;
        for(int j=0; j<col; j++)
        {
            if(gameMap[i][j]==0)
                clearCnt++;
            if(gameMap[i][j]==1)
                rockCnt++;
        }
        if(clearCnt==col)
            break;
        if(rockCnt==col)
        {
            lineClear++;
            continue;
        }
        if(lineClear)
        {
            for(int j=0; j<col; j++)
            {
                gameMap[i+lineClear][j]=gameMap[i][j];
                gameMap[i][j]=0;
            }
        }

    }

    for(int i=1; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            if(gameMap[i][j]==1)
            {
                setfillstyle(SOLID_FILL, WHITE);
                bar(40+j*SIZE, (i+lineClear)*SIZE,
                    40+(j+1)*SIZE-1, (i+1+lineClear)*SIZE-1);
            }
        }
    }

    // update score
    fpsCnt++;
    if(fpsCnt==3600)
    {
        score+=TIMEBONUS;
        fpsCnt=0;
    }

    if(lineClear>0)
        score+=CLEARBONUS*lineClear;
    sprintf(scoreStr, "%d", score);
    outtextxy(540, 160, scoreStr);

    // pause game
    if(pauseFlag)
    {
        PlaySound(NULL,NULL,SND_FILENAME);
        pauseFlag=0;
        getch();
        PlaySound(TEXT("bgm.wav"), NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    // check game status
    for(int i=0; i<col; i++)
    {
        if(gameMap[4][i]==1)
        {
            char endStr[20];
            sprintf(endStr, "Final Score:%d", score);
            setfont(30, 0, "宋体");
            outtextxy(240, 120, "GAME OVER");
            outtextxy(240, 150, endStr);
            outtextxy(240, 180, "Press any key");
            outtextxy(240, 210, "to start a new game");
            getch();
            reset();
        }
    }
}

/* Successfully Tested */
void drawRock(int dstX, int dstY, ROCK &rockTmp)
{
    int bit, row, col, maxRow, maxCol;
    maxRow=0;
    maxCol=0;
    rockTmp.img=newimage();
    for(int i=0; i<16; i++)
    {
        bit=rockTmp.pattern&(1<<(15-i));
        bit=!!bit;
        row=i/4;
        col=i%4;
        if(bit)
        {
            setfillstyle(SOLID_FILL, WHITE);
            bar(dstX+col*SIZE, dstY+row*SIZE,
                    dstX+(col+1)*SIZE-1, dstY+(row+1)*SIZE-1);
            if(maxRow<row)
                maxRow=row;
            if(maxCol<col)
                maxCol=col;
        }
    }
    rockTmp.length=(maxCol+1)*SIZE;
    rockTmp.height=(maxRow+1)*SIZE;
    getimage(rockTmp.img, dstX, dstY, rockTmp.length, rockTmp.height);
    cleardevice();
    putimage(0, 0, scene);
}

void randomRock(ROCK &rockTmp)
{
    int rand=random(8);
    switch(rand)
    {
    case 0:
        rockTmp.pattern=0x8c00; //1000
        break;                  //1100
    case 1:
        rockTmp.pattern=0x8e00; //1000
        break;                  //1110
    case 2:
        rockTmp.pattern=0x4e00; //0100
        break;                  //1110
    case 3:
        rockTmp.pattern=0xf000; //1111
        break;
    case 4:
        rockTmp.pattern=0x2e00; //0010
        break;                  //1110
    case 5:
        rockTmp.pattern=0xc600; //1100
        break;                  //0110
    case 6:
        rockTmp.pattern=0x4c00; //0100
        break;                  //1100
    case 7:
        rockTmp.pattern=0x6c00; //0110
        break;                  //1100
    }
}

/* Successfully Tested */
void initRocks()
{
    static bool i=0;
    if(releaseFlag==0)
        return;
    else
    {
        releaseFlag=0;
        rock.top_x=200;
        rock.top_y=0;
        if(i==0)
        {
            randomRock(rock);
            i++;
        }
        else
        {
            rock.pattern=rockNext.pattern;
        }
        randomRock(rockNext);
        drawRock(200, 0, rock);
        drawRock(510, 290, rockNext);
    }
}

/* successfully tested! */
bool bottomCheck()
{
    int col[4]={0};
    int bit;
    int mapX, mapY, isOnGrid;

    if(rock.top_y+rock.height>475)
        banSpeed=1;
    if(rock.top_y+rock.height>=480)
        return false;

    for(int i=0; i<4; i++)// col i
    {
        for(int j=0; j<4; j++)// row j
        {
            bit=rock.pattern&(0x1<<(15-i-j*4));
            bit=!!bit;
            if(bit)
                col[i]=j+1;
        }
    }
    mapX=(rock.top_x-40)/SIZE;
    mapY=rock.top_y/SIZE;
    isOnGrid=rock.top_y%SIZE;
    if(!isOnGrid)// on grid
    {
        for(int i=0; i<4; i++)
        {
            if(col[i]==0)
                continue;
            if(gameMap[mapY+col[i]][mapX+i]==1)
                return false;
        }
    }
    else// not on grid
    {
        for(int i=0; i<4; i++)
        {
            if(col[i]==0)
                continue;
            if(gameMap[mapY+col[i]+1][mapX+i]==1)
                banSpeed=1;
        }
    }
    return true;
}

/* successfully tested! */
bool borderCheck(enum DIRECTION d)
{
    int row[4]={0}, bit;
    int mapX, mapY, isOnGrid;

    if(rock.top_x==40&&d==LEFT)
        return false;
    if((rock.top_x+rock.length)==440&&d==RIGHT)
        return false;

    mapX=(rock.top_x-40)/SIZE;
    mapY=rock.top_y/SIZE;
    isOnGrid=rock.top_y%SIZE;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(d==LEFT)
                bit=rock.pattern&(0x1<<(15-j-i*4));
            else
                bit=rock.pattern&(0x1<<(12+j-i*4));
            bit=!!bit;
            if(bit)
            {
                if(d==LEFT)
                    row[i]=j+1;
                else
                    row[i]=4-j;
                break;
            }
        }
    }
    for(int i=0; i<4; i++)
    {
        int x, y;// coordinate on map

        if(row[i]==0)
            continue;

        y=mapY+i;
        if(d==LEFT)
            x=mapX+row[i]-2;
        else
            x=mapX+row[i];

        if(gameMap[y][x]==1)
            return false;

        if(isOnGrid)// not on grid
        {
            if(gameMap[y+1][x]==1)
                return false;
        }
    }
    return true;
}

/* successfully tested! */
void rotateRock(enum DIRECTION d)
{
    int x, y, isOnGrid;// coordinate
    int emptyLine=0, patt=0;// pattern
    int ptn[4][4]={0}, ptnNew[4][4]={0};// pattern

    x=(rock.top_x-40)/SIZE;
    y=rock.top_y/SIZE;
    isOnGrid=rock.top_y%SIZE;

    for(int i=0; i<4; i++)
    {
        int line;
        if(d==RIGHT)
            line=rock.pattern&(0xf<<(i*4));
        else
            line=rock.pattern&(0x1111<<i);
        if(line==0)
            emptyLine++;
        else
            break;
    }
    for(int i=0; i<16; i++)
    {
        int bit, row, col;
        bit=rock.pattern&(1<<(15-i));
        bit=!!bit;
        if(bit)
        {
            row=i/4;
            col=i%4;
            ptn[row][col]=1;
        }
    }
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(d==RIGHT)
            {
                if(3-i-emptyLine>=0)
                {
                    ptnNew[j][3-i-emptyLine]=ptn[i][j];
                    patt+=ptnNew[j][3-i-emptyLine]<<(15-j*4-(3-i-emptyLine));
                }
            }
            else
            {
                if(3-j-emptyLine>=0)
                {
                    ptnNew[3-j-emptyLine][i]=ptn[i][j];
                    patt+=ptnNew[3-j-emptyLine][i]<<(15-(3-j-emptyLine)*4-i);
                }
            }
        }
    }

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(ptnNew[i][j]==1)
            {
                if(x+j>=400/SIZE)
                    return;
                if(y+i+1>=480/SIZE)
                    return;
                if(gameMap[y+i][x+j]==1)
                    return;// fail to rotate
                if(isOnGrid)// not on grid
                {
                    if(gameMap[y+i+1][x+j]==1)
                        return;// fail to rotate
                }
            }
        }
    }

    rock.pattern=patt;
    drawRock(rock.top_x, rock.top_y, rock);
}

void updateRock()
{
    static int noFallCnt=0, speed=NORM_SPEED, cnt=0;
    if(kbhit())
    {
        int keybd;// keyboard
        keybd=getch();

        switch(keybd)
        {
        case 'd':
            if(borderCheck(RIGHT))
                rock.top_x+=SIZE;
            break;
        case 'a':
            if(borderCheck(LEFT))
                rock.top_x-=SIZE;
            break;
        case 's':
            if(!banSpeed)
                speed=HIGH_SPEED;
            break;
        case 'e':
            rotateRock(RIGHT);
            break;
        case 'q':
            rotateRock(LEFT);
            break;
        case 'p':
            pauseFlag=1;
            break;
        case 'm':
            cnt=cnt^1;
            if(cnt)
                PlaySound(NULL,NULL,SND_FILENAME);
            else
                PlaySound(TEXT("bgm.wav"), NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
            break;
        }
    }

    if(bottomCheck())
    {
        rock.top_y+=speed;
        speed=NORM_SPEED;
    }
    else
    {
        noFallCnt++;
        if(noFallCnt==60)
        {
            score+=ROCKSCORE;
            noFallCnt=0;
            releaseFlag=1;
            banSpeed=0;

            for(int i=0; i<16; i++)
            {
                int bit, row, col;
                bit=rock.pattern&(1<<(15-i));
                bit=!!bit;
                if(bit)
                {
                    // row, col are used as
                    // tmp argument in this section
                    row=i/4;
                    col=i%4;
                    row=rock.top_y/SIZE+row;
                    col=(rock.top_x-40)/SIZE+col;
                    gameMap[row][col]=1;
                }
            }
        }
    }
    putimage_transparent(NULL, rock.img, rock.top_x, rock.top_y, BLACK);
}

void reset()
{
    for(int i=0; i<480/SIZE; i++)
    {
        for(int j=0; j<400/SIZE; j++)
        {
            gameMap[i][j]=0;
        }
    }
    score=0;
    fpsCnt=0;
    timeBegin=time(NULL);
}

void welcomePage()
{
    int cnt=0;
    char flashStr[25]="Press any key to start.";
    char welcomeStr[210]="Welcome to Tetris!\n"
        "This is a Beta version of the game\n"
        "Thanks for playing. Have fun!\n\n"
        "How to play:\n"
        "Move: Press 'a' or 'd'\n"
        "Rotate: Press 'q' or 'e'\n"
        "Accelerate: Press 's'\n"
        "Pause: Press 'p'\n"
        "Mute: Press 'm'\n\n";

    setfont(25, 0, "宋体");
    for( ; 1; delay_fps(2) )
    {
        cnt=cnt^1;
        cleardevice();

        outtextrect(30,20,640,480,welcomeStr);
        if(cnt)
            outtextxy(30, 300, flashStr);
        if(kbhit())
            break;
    }
    cleardevice();
}
