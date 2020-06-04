#include "autosnake.h"

//拷贝构造函数
AutoSnake::AutoSnake(int sideLen, int stepLen)
{
	this->sideLen = sideLen;
	this->stepLen = stepLen;

	init();
}

AutoSnake::~AutoSnake()
{
	delete food;

	delete pathBoard;
	delete path;
	delete board;
}

void AutoSnake::init()
{
	//path_board初始化
	pathBoard = new bool*[sideLen];
	for(int i = 0; i < sideLen; i++)
	{
		pathBoard[i] = new bool[sideLen];
		for(int j = 0; j < sideLen; j++)
			pathBoard[i][j] = false;
	}

	//path初始化
	path = new qint8*[sideLen];
	for(int i = 0; i < sideLen; i++)
		path[i] = new qint8[sideLen];
	for (int i = 0; i < sideLen; i += 2) {
		for (int j = 0; j < sideLen; j += 2) {
			path[i][j] = Right;
			path[i + 1][j + 1] = Left;
			path[i][j + 1] = Up;
			path[i + 1][j] = Down;
		}
	}

	//board初始化
	board = new bool*[sideLen];
	for(int i = 0; i < sideLen; i++)
	{
		board[i] = new bool[sideLen];
		for(int j = 0; j < sideLen; j++)
			board[i][j] = false;
	}

	//头插法，初始长度为2
	for (int i = 0; i < 2; i++)
	{
		snake.push_front(SnakeNode(i, 0));
		board[i][0] = true;
	}
	food = new SnakeNode;

	setPathBoard(SnakeNode(0, 0), true);
}

//创建食物
void AutoSnake::createFood()
{
	do
	{	//随机创建食物
		food->x = qrand() % sideLen;
		food->y = qrand() % sideLen;
	} while(board[food->x][food->y]);

	delPath();
	formPath();
}

/*
 * 贪吃蛇前进
 * 参数：
 *	node：前进的节点
 *	isEatFood：是否吃到食物
 */
void AutoSnake::forward(SnakeNode &node, bool isEatFood)
{
	//如果没吃到食物，则删除尾部节点
	if(!isEatFood)
	{
		setBoardFalse(snake.back());
		snake.pop_back();
		setBoardTrue(node);
		snake.push_front(node);
		delPath();
		formPath();
	}
	else
	{
		setBoardTrue(node);
		snake.push_front(node);
		if(snake.size() != sideLen*sideLen)
			createFood();		//吃满就不创建了
	}
}

/*
 * 移动
 * 返回值：
 *	0：正常前进
 *	1：撞到自己
 *	2：撞到墙
 *	4：吃到食物
 */
int AutoSnake::move()
{
	//创建前进节点
	SnakeNode nextHead = snake.front();

	//判断前进方向
	//前进节点赋值
	int tmpDir = path[nextHead.x][nextHead.y];
	nextHead.x += dir[tmpDir][0];
	nextHead.y += dir[tmpDir][1];

	//是否吃到食物
	//蛇前进
	if(nextHead.x == food->x && nextHead.y == food->y)
	{
		forward(nextHead, true);
		return 1;		//吃到食物
	}
	forward(nextHead, false);
	return 0;			//正常前进
}

//一次性将一个环添加或删除时，修改path_board
void AutoSnake::setPathBoard(SnakeNode x, bool t)
{
	pathBoard[x.x][x.y] = t;
	pathBoard[x.x + 1][x.y] = t;
	pathBoard[x.x][x.y + 1] = t;
	pathBoard[x.x + 1][x.y + 1] = t;
}

//某一极小环路是否存在蛇的身体，不存在的环路要拆除
bool AutoSnake::matrixNoPoint(SnakeNode x)
{
	return board[x.x][x.y] == false && board[x.x + 1][x.y] == false &&
		board[x.x][x.y + 1] == false && board[x.x + 1][x.y + 1] == false;
}

//两个极小环路相接的环路是否一个在路径里，一个在路径外，这是可添加路径的判断依据
bool AutoSnake::pathBoardNotAllTrue(SnakeNode x)
{
	return !(pathBoard[x.x][x.y] && pathBoard[x.x + 1][x.y + 1]);
}

//添加路径
void AutoSnake::addPathMatrix(SnakeNode x)
{
	if (x.x % 2)
	{
		if (pathBoardNotAllTrue(x))
		{
			path[x.x][x.y] = Right;
			path[x.x + 1][x.y + 1] = Left;
			setPathBoard(SnakeNode(x.x - 1, x.y), true);
			setPathBoard(SnakeNode(x.x + 1, x.y), true);
		}
	}
	else
	{
		if (pathBoardNotAllTrue(x))
		{
			path[x.x + 1][x.y] = Down;
			path[x.x][x.y + 1] = Up;
			setPathBoard(SnakeNode(x.x, x.y + 1), true);
			setPathBoard(SnakeNode(x.x, x.y - 1), true);
		}
	}
}

//判断是否为可剪除的环路
void AutoSnake::checkIfDel(SnakeNode x)
{
	if (matrixNoPoint(x))
		setPathBoard(x, false);
}

void AutoSnake::delPathMatrix(SnakeNode x)
{
	if (x.x % 2)
	{
		path[x.x][x.y] = Down;
		path[x.x + 1][x.y + 1] = Up;
		checkIfDel(SnakeNode(x.x - 1, x.y));
		checkIfDel(SnakeNode(x.x + 1, x.y));
	}
	else
	{
		path[x.x + 1][x.y] = Left;
		path[x.x][x.y + 1] = Right;
		checkIfDel(SnakeNode(x.x, x.y - 1));
		checkIfDel(SnakeNode(x.x, x.y + 1));
	}
}

//路径生成
void AutoSnake::formPath()
{
	SnakeNode head = snake.front();
	head.x = head.x & 0xfffe;
	head.y = head.y & 0xfffe;

	SnakeNode end = *food;
	end.x = end.x & 0xfffe;
	end.y = end.y & 0xfffe;

	short dx;
	if (head.x > end.x) dx = -2;
	else dx = 2;

	short dy;
	if (head.y > end.y) dy = -2;
	else dy = 2;

	while (head.x != end.x)
	{
		short x = head.x;
		head.x += dx;
		addPathMatrix(SnakeNode((x + head.x) / 2, head.y));
	}

	while (head.y != end.y)
	{
		short y = head.y;
		head.y += dy;
		addPathMatrix(SnakeNode(end.x, (head.y + y)/2));
	}
}

//删除路径
void AutoSnake::delPath()
{
	for (short i = 0; i <= sideLen - 2; i += 2)
		for (short j = 1; j < sideLen - 2; j += 2)
		{
			if (matrixNoPoint(SnakeNode(i, j - 1)) || matrixNoPoint(SnakeNode(i, j + 1)))
				delPathMatrix(SnakeNode(i, j));
			if (matrixNoPoint(SnakeNode(j - 1, i)) || matrixNoPoint(SnakeNode(j + 1, i)))
				delPathMatrix(SnakeNode(j, i));
		}
}

bool AutoSnake::isNotAllTrue(bool **tmp)
{
	for (int i = 0; i < sideLen; i++)
		for (int j = 0; j < sideLen; j++)
			if (tmp[i][j] == false)
				return true;
	return false;
}

bool AutoSnake::isValidPoint(SnakeNode tmp)
{
	return !((tmp.x & 0xfff0) || (tmp.y & 0xfff0));
}

void AutoSnake::setBoardTrue(SnakeNode X)
{
	board[X.x][X.y] = true;
}

void AutoSnake::setBoardFalse(SnakeNode X)
{
	board[X.x][X.y] = false;
}


