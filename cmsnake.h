#ifndef CMSNAKE_H
#define CMSNAKE_H

#include "snake.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
class CMSnake;
}

class CMSnake : public QWidget
{
    Q_OBJECT

public:
    explicit CMSnake(QWidget *parent = nullptr);
    ~CMSnake();

	void initGame();						//游戏初始化
	void paintEvent(QPaintEvent *event);	//绘制游戏界面
	void endGame();							//游戏结束
	void keyReleaseEvent(QKeyEvent *event);	//按键按下事件
	bool play();							//开始游戏

private slots:
	//蛇移动
	void snakeMove();
	//是否静音
	void on_soundOnOffCheckBox_stateChanged(int arg1);

signals:
	void showMainWindow();

private:
    Ui::CMSnake *ui;

	//游戏参数声明
	int score;					//金币
	int add;					//单价
	int grade;					//难度等级
	int sleeptime;				//每次运行的时间间隔
	int endGamestatus;			//游戏结束的情况，-2:未开始; -1:暂停; 0:正常; 1:撞到墙; 2:咬到自己; 3:主动退出游戏;
	int mapLength;				//地图的边长
	int mapSize;				//地图每一块的大小

	//游戏资源声明
	QImage jinbi;				//金币图片
	QMediaPlaylist *bgPlaylist;	//加载背景音乐
	QMediaPlayer *bgPlayer;		//背景音乐播放
	QMediaPlaylist *jbPlaylist;	//加载金币音效
	QMediaPlayer *jbPlayer;		//金币音效播放
	QMediaPlaylist *swPlaylist;	//加载死亡音效
	QMediaPlayer *swPlayer;		//死亡音效播放

	Snake *snake;				//蛇指针
	QTimer *timer;				//定时器
	QMessageBox msgBox;			//消息对话框
};

#endif // CMSNAKE_H
