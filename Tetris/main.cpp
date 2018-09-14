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
	setinitmode(INIT_ANIMATION);    // ����egeģʽΪ����ģʽ
	initgraph(640, 480);            // ͼ�γ�ʼ�������ڳߴ�640x480
	randomize();                    // �������ʼ���������Ҫʹ��������Ļ�
	setrendermode(RENDER_MANUAL);   // ��ͼ����ģʽ����Ϊ�ֶ���������˸

	setcaption("Tetris Beta");
    //test();
	// ������ѭ��
	mainloop();
	// �رջ�ͼ�豸
	closegraph();
	return 0;
}
