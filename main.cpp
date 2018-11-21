#include "linux-kbhit.h"
#include "stdio.h"
#include <signal.h>
#include <unistd.h>
#include <iostream>

using namespace std;

static sig_atomic_t my_end = 0;
static void sighandler(int signo)
{
    my_end = 1;
    printf("good beye!\n");
}

const int map[15][10] = {
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

/*
 *   *
 *   *
 *   ***
 *
 * */
int kShape0[4][4][4] = {
    {
        { 0, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
    },
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
    },
};


/*
 *   *
 *   * *
 *     *
 *
 *
 * */
int kShap1[4][4][4] = {
    {
        { 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 },
    },
};


/*
 *     *
 *   * *
 *   *
 *
 *
 * */
int kShap2[4][4][4] = {
    {
        { 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
    {
        { 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 },
    },
};


int blkr = 5;
int blkc = 1;
int rotation = 0;

struct Data
{
	int row;
	int col;
    int rotation;
	int rowbk;
	int colbk;
    int rotationbk;
};

Data block;

enum Dir
{
	kNone,
	kLeftUp,
	kLeftDown,
	kRightUp,
	kRightDown,
};

int main()
{
    int (&cur_block)[4][4][4] = kShape0;

    term_setup(sighandler);

	block.row = 5;
	block.col = 1;
    block.rotation = 0;
	block.rowbk = block.row;
	block.colbk = block.col;
    block.rotationbk = block.rotation;

	while (!my_end)
	{
        kbhit();
		system("clear");
		//砖块坐标备份
        block.rowbk = block.row;
        block.colbk = block.col;
        block.rotationbk = block.rotation;

		if (keydown(LEFT))
		{
            block.col--;
		}
		else if (keydown(RIGHT))
		{

            block.col++;
		}
		else if (keydown(DOWN))
		{
            block.row++;
		}
		else if (keydown(UP))
		{
            block.rotation++;
			block.rotation = block.rotation % 4;
		}

        bool collsion = false;

        //方块碰墙检测
        for ( int i = 0; i < 4; i++ ){
            for ( int j = 0; j < 4; j++ ){
                //砖块内部坐标，从上往下，最左往右扫描
                int inner_row = i;
                int inner_col = j;
                //坐标转换到map
                int maprow = block.row - 3 + inner_row;
                int mapcol = block.col + j;
                if (1 == cur_block[block.rotation][inner_row][inner_col]
                        && 1 == map[maprow][mapcol]){
                    //还原坐标
                    block.row = block.rowbk;
                    block.col = block.colbk;
                    block.rotation = block.rotationbk;
                    collsion = true;
                    break;
                }
            }
            if (collsion)
                break;
        }

		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 10; j++)
			{
                int startx = block.row - 3;
                int endy = block.col + 3;
                bool draw_block = false;

                if (i >= startx && i <= block.row
                        && j >= block.col && j <= endy) {
                    int ridx = i - startx;
                    int cidx = j - block.col;

                    if (1 == cur_block[block.rotation][ridx][cidx]){
                        draw_block = true;
                    }
                }

				if (1 == map[i][j])
				{
					cout << "■ ";
				}
                else if (draw_block) {
					cout << "■ ";
                }
				else
				{
					cout << "  ";
				}
			}
			cout << endl;
		}
        usleep(32 * 1000);
	}


    term_restore();

    return 0;
}
