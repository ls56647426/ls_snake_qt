#include "snake.h"

Snake::Snake()
{
	this->mapLength = 34;
	this->mapSize = 20;

	init();
}

//拷贝构造函数
Snake::Snake(int mapLength, int mapSize)
{
	this->mapLength = mapLength;
	this->mapSize = mapSize;

	init();
}

Snake::~Snake()
{
	delete food;
}

void Snake::init()
{
	SnakeNode *tail;		//蛇尾指针
	int i;

	//为了防止每次产生的随机数相同，种子设置为time
	QTime time = QTime::currentTime();
	int base = time.msec() + time.second() * 1000;
	qsrand(base);

	//从蛇尾开始，头插法，以x,y设定开始的位置
	//初始长度为4
	for (i = 0; i < 2; i++)
	{
		tail = new SnakeNode;
		tail->x = (i+1)*mapSize;
		tail->y = mapSize;
		tail->color = qRgb(255, 0, 0);
		snake.push_front(*tail);
	}

	//蛇移动方向向右
	dir = Right;
}

void Snake::createFood()
{
	SnakeNode *food_1 = nullptr;

	//创建食物
	food_1 = new SnakeNode;
	if (food_1 == nullptr) return createFood();

	do
	{	//保证其为20的倍数，使得食物能与蛇头对其
		food_1->x = qrand() % (mapLength - 3)*mapSize + mapSize;
	} while (food_1->x % mapSize);
	do
	{	//x，y分开随机，减少循环次数
		food_1->y = qrand() % (mapLength - 3)*mapSize + mapSize;
	} while (food_1->y % mapSize);
	food_1->color = qRgb(qrand() % 256, qrand() % 256, qrand() % 256);

	for(QList<SnakeNode>::iterator it = snake.begin(); it != snake.end(); it++)
	{
		if(food_1->x == it->x && food_1->y == it->y)
		{
			//判断蛇身是否与食物重合
			free(food_1);
			createFood();
			return;
		}
	}

	//加载食物
	food = food_1;
}

/*
 * 贪吃蛇前进
 * 参数：
 *	node：前进的节点
 *	reward：是否吃到食物
 */
void Snake::forward(SnakeNode *node, bool isEatFood)
{
	//头部添加前进节点
	snake.push_front(*node);
	//如果没吃到食物，则删除尾部节点
	if(!isEatFood)
		snake.pop_back();
}

/*
 * 移动
 * 返回值：
 *	0：正常前进
 *	1：撞到自己
 *	2：撞到墙
 *	4：吃到食物
 */
int Snake::move()
{
	//创建前进节点
	SnakeNode *nextHead = new SnakeNode;
	if(nextHead == nullptr) return move();

	//判断前进方向
	//前进节点赋值
	switch(dir)
	{
	case Up:		//上
		nextHead->x = snake.front().x;
		nextHead->y = snake.front().y - mapSize;
		break;
	case Down:		//下
		nextHead->x = snake.front().x;
		nextHead->y = snake.front().y + mapSize;
		break;
	case Left:		//左
		nextHead->x = snake.front().x - mapSize;
		nextHead->y = snake.front().y;
		break;
	case Right:		//右
		nextHead->x = snake.front().x + mapSize;
		nextHead->y = snake.front().y;
		break;
	default:
		break;
	}
	nextHead->color = qRgb(255, 0, 0);

	//是否吃到食物
	//蛇前进
	if(nextHead->x == food->x && nextHead->y == food->y)
	{
		forward(nextHead, true);
		return 4;		//吃到食物
	}
	else forward(nextHead, false);

	//是否撞到自己或墙
	if (nextHead->x < mapSize || nextHead->x > (mapLength - 2)*mapSize || nextHead->y < mapSize ||
			nextHead->y > (mapLength - 2)*mapSize)
		return 1;		//撞到墙了
	for(QList<SnakeNode>::iterator it = snake.begin() + 1; it != snake.end(); it++)
		if(nextHead->x == it->x && nextHead->y == it->y)
			return 2;	//撞到自己

	return 0;			//正常前进
}
