#ifndef AUTOSNAKE_H
#define AUTOSNAKE_H


#include <QColor>
#include <QList>
#include <QDebug>

#include "snakenode.h"

class AutoSnake
{
public:
	enum Move{Left = 0, Right, Up, Down};
	const qint8 dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

	QList<SnakeNode> snake;		//蛇
	SnakeNode *food;			//食物
	int sideLen;				//边长
	int stepLen;				//步长
	bool **pathBoard;			//点是否在环路上
	qint8 **path;				//每个点上的移动方向，存储L,R,D,U
	bool **board;

	AutoSnake(int sideLen = 16, int stepLen = 20);
	~AutoSnake();
	void init();									//初始化
	void createFood();								//创建食物
	void forward(SnakeNode &node, bool isEatFood);	//蛇前进
	int move();										//蛇移动
	void setPathBoard(SnakeNode x, bool t);			//一次性将一个环添加或删除时，修改path_board
	bool matrixNoPoint(SnakeNode x);				//某一极小环路是否存在蛇的身体，不存在的环路要拆除
	bool pathBoardNotAllTrue(SnakeNode x);			//两个极小环路相接的环路是否一个在路径里，一个在路径外，这是可添加路径的判断依据
	void addPathMatrix(SnakeNode x);				//添加路径
	void checkIfDel(SnakeNode x);					//判断是否为可剪除的环路
	void delPathMatrix(SnakeNode x);				//
	void formPath();								//路径生成
	void delPath();									//路径删除
	bool isNotAllTrue(bool **tmp);					//不全为true
	bool isValidPoint(SnakeNode tmp);				//
	void setBoardTrue(SnakeNode X);					//设该点为true
	void setBoardFalse(SnakeNode X);				//设该点为false
};


#endif // AUTOSNAKE_H
