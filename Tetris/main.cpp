#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "tetris.h"

PIMAGE scene;
long score;

void mainloop()
{
	welcomePage();
	initScene();
	for ( ; is_run(); delay_fps(60) )
	{
		cleardevice();

		putimage(0, 0, scene);
		initRocks();
		updateRock();
		updateScene();
	}
}

int main(void)
{
	setinitmode(INIT_ANIMATION);    // 设置ege模式为动画模式
	initgraph(640, 480);            // 图形初始化，窗口尺寸640x480
	randomize();                    // 随机数初始化，如果需要使用随机数的话
	setrendermode(RENDER_MANUAL);   // 绘图更新模式设置为手动，避免闪烁

	setcaption("Tetris Beta");
    //test();
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	closegraph();
	return 0;
}
