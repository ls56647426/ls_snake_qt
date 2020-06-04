#include "snakenode.h"

SnakeNode::SnakeNode()
{

}

SnakeNode::SnakeNode(int x, int y, QColor color)
{
	this->x = x;
	this->y = y;
	this->color = color;
}

bool SnakeNode::operator ==(const SnakeNode &other) const
{
	return (this->x == other.x && this->y == other.y);
}

bool SnakeNode::operator !=(const SnakeNode &other) const
{
	return !(*this == other);
}
