#ifndef SNAKE_H
#define SNAKE_H

#include <QColor>
#include <QTime>
#include <QList>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "snakenode.h"

class Snake
{
public:
	enum Move{Up = 1, Down, Left, Right};

	QList<SnakeNode> snake;		//蛇
	int dir;					//移动方向
	SnakeNode *food;			//食物
	int mapLength;
	int mapSize;

	Snake();
	Snake(int mapLength, int mapSize);
	~Snake();
	void init();									//初始化
	void createFood();								//创建食物
	void forward(SnakeNode *node, bool isEatFood);	//蛇前进
	int move();										//蛇移动

};

#endif // SNAKE_H
