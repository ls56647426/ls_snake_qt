#ifndef SNAKENODE_H
#define SNAKENODE_H

#include <QColor>

class SnakeNode
{
public:
	int x, y;			//坐标
	QColor color;		//颜色

	SnakeNode();
	SnakeNode(int x, int y, QColor color = Qt::red);

	bool operator ==(const SnakeNode &other) const;
	bool operator !=(const SnakeNode &other) const;
};

#endif // SNAKENODE_H
