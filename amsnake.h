#ifndef AMSNAKE_H
#define AMSNAKE_H

#include "autosnake.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
class AMSnake;
}

class AMSnake : public QWidget
{
    Q_OBJECT

public:
    explicit AMSnake(QWidget *parent = nullptr);
    ~AMSnake();

	void initGame();						//游戏初始化
	void paintEvent(QPaintEvent *event);	//绘制游戏界面
	void initSnake();						//蛇身初始化
	void createFood();						//随机出现食物
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
    Ui::AMSnake *ui;

	//游戏参数声明
	int score;					//金币
	int add;					//单价
	int grade;					//难度等级
	int sleeptime;				//每次运行的时间间隔
	int endGamestatus;			//游戏结束的情况，-2:未开始; -1:暂停; 0:正常; 1:撞到墙; 2:咬到自己; 3:主动退出游戏; 5:吃满全屏，游戏胜利
	int status;					//蛇的状态
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
	qint64 bgPlayPos;			//背景音乐播放进度

	//游戏功能类型声明
	QTimer *timer;				//定时器
	QMessageBox msgBox;			//消息对话框
	AutoSnake *snake;			//蛇指针
};

#endif // AMSNAKE_H
