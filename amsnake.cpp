#include "amsnake.h"
#include "ui_amsnake.h"

AMSnake::AMSnake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AMSnake)
{
    ui->setupUi(this);

	setWindowTitle("自动贪吃蛇");
	endGamestatus = -2;		//游戏未开始
}

AMSnake::~AMSnake()
{
    delete ui;

	if(endGamestatus == -2) return;

	delete snake;

	delete bgPlaylist;
	delete bgPlayer;

	delete jbPlaylist;
	delete jbPlayer;

	delete swPlaylist;
	delete swPlayer;

	delete timer;
}

//游戏初始化
void AMSnake::initGame()
{
	//数据初始化
	score = 0;
	add = 10;
	grade = 1;
	sleeptime = 155;

	//音频初始化
	bgPlaylist = new QMediaPlaylist;
	bgPlaylist->addMedia(QUrl("qrc:/mp3/1.mp3"));
	bgPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	bgPlaylist->setCurrentIndex(1);
	bgPlayer = new QMediaPlayer;
	bgPlayer->setPlaylist(bgPlaylist);
	bgPlayer->play();

	jbPlaylist = new QMediaPlaylist;
	jbPlaylist->addMedia(QUrl("qrc:/mp3/jinbi.mp3"));
	jbPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
	jbPlaylist->setCurrentIndex(1);
	jbPlayer = new QMediaPlayer;
	jbPlayer->setPlaylist(jbPlaylist);

	swPlaylist = new QMediaPlaylist;
	swPlaylist->addMedia(QUrl("qrc:/mp3/siwang.mp3"));
	swPlaylist->setCurrentIndex(1);
	swPlayer = new QMediaPlayer;
	swPlayer->setPlaylist(swPlaylist);

	//加载金币图片
	jinbi.load(":/image/jinbi.jpg");

	//消息对话框初始化
	msgBox.setWindowTitle("帅气的龙少提示您：");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setModal(true);

	//蛇身初始化
	snake = new AutoSnake(mapLength, mapSize);

	//创建食物
	snake->createFood();

	//启动定时器
	timer = new QTimer;
	timer->start(sleeptime);
	connect(timer, SIGNAL(timeout()), this, SLOT(snakeMove()));

	ui->monLabel->setText(QString("金币：%1").arg(score));
	ui->oneLabel->setText(QString("单价：%1").arg(add));

	endGamestatus = 0;
}

//绘制游戏界面
void AMSnake::paintEvent(QPaintEvent *event)
{
	if(endGamestatus == -2) return;

	QPainter painter(this);
	QPen pen;
	QBrush brush;
	QFont font("方正舒体",12,QFont::ExtraLight,false);

	//反锯齿
	painter.setRenderHint(QPainter::Antialiasing);

	pen.setColor(qRgb(195, 195, 195));		//边框颜色
	pen.setJoinStyle(Qt::MiterJoin);
	pen.setWidth(mapSize);
	painter.setPen(pen);
	brush.setColor(qRgb(128, 255, 0));		//背景颜色
	brush.setStyle(Qt::SolidPattern);
	painter.setBrush(brush);
	painter.drawRect(mapSize / 2, mapSize / 2, (mapLength+1)*mapSize, (mapLength+1)*mapSize);

	pen.setColor(qRgb(0, 0, 255));			//网格颜色
	pen.setWidth(0);
	painter.setPen(pen);
	for (int i = 2*mapSize; i <= mapLength*mapSize; i += mapSize)
		for (int j = 2*mapSize; j <= mapLength*mapSize; j += mapSize)
			painter.drawEllipse(i - 1, j - 1, mapSize/10, mapSize/10);

	//绘制蛇身
	painter.translate(mapSize, mapSize);
	QList<SnakeNode>::iterator it = snake->snake.begin();
	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	brush.setColor(Qt::yellow);
	brush.setStyle(Qt::SolidPattern);
	painter.setBrush(brush);
	painter.drawEllipse(it->x*mapSize, it->y*mapSize, mapSize, mapSize);
	it++;
	while (it + 1 != snake->snake.end())
	{
		pen.setColor(it->color);
		painter.setPen(pen);
		brush.setColor(it->color);
		brush.setStyle(Qt::SolidPattern);
		painter.setBrush(brush);
		painter.drawEllipse(it->x*mapSize, it->y*mapSize, mapSize, mapSize);
		it++;
	}
	pen.setColor(Qt::blue);
	painter.setPen(pen);
	brush.setColor(Qt::blue);
	brush.setStyle(Qt::SolidPattern);
	painter.setBrush(brush);
	painter.drawEllipse(it->x*mapSize, it->y*mapSize, mapSize, mapSize);

	//绘制食物
	painter.drawImage(QRect(snake->food->x*mapSize, snake->food->y*mapSize, mapSize, mapSize), jinbi);

	QWidget::paintEvent(event);
}

//游戏结束
void AMSnake::endGame()
{
	//停止背景音乐
	bgPlayer->stop();
	timer->stop();

	switch (endGamestatus)
	{	//游戏结束的情况，-2:未开始; -1:暂停; 0:正常; 3:主动退出游戏; 5:吃满全屏，游戏胜利
	case 3:
		swPlayer->play();
		msgBox.setText("您的游戏已经结束了！");
		msgBox.setIcon(QMessageBox::Icon::NoIcon);
		msgBox.exec();
		swPlayer->stop();
		break;
	case 5:
		msgBox.setText("恭喜你，吃满全屏，游戏胜利！");
		msgBox.setIcon(QMessageBox::Icon::NoIcon);
		msgBox.exec();
		break;
	default:
		break;
	}

	//关闭当前游戏窗口，并返回主菜单页面
	this->close();
	emit showMainWindow();
}

//蛇移动
void AMSnake::snakeMove()
{
	if(endGamestatus == -2) return;

	switch(snake->move())
	{
	case 1:
		jbPlayer->stop();
		jbPlayer->play();
		score += add;
		ui->monLabel->setText(QString("金币：%1").arg(score));
		if(snake->snake.size() == mapLength*mapLength)
		{	//吃满全屏
			endGamestatus = 5;
			endGame();
			return;
		}
		break;
	default:
		break;
	}

	update();
}

//按键按下事件
void AMSnake::keyReleaseEvent(QKeyEvent *event)
{
	if(endGamestatus == -2) return;

	switch(event->key()){
	case Qt::Key_Space:
		if(endGamestatus == -1){
			timer->start(sleeptime);
			endGamestatus = 0;
		}else if(endGamestatus == 0){
			timer->stop();
			endGamestatus = -1;
		}
		break;
	case Qt::Key_Escape:
		endGamestatus = 3;
		endGame();
		break;
	case Qt::Key_F1:
		if (sleeptime > 30)
		{	//速度不能太快
			sleeptime = sleeptime - 30;
			timer->stop();
			timer->start(sleeptime);
			add = add + 2;
			ui->oneLabel->setText(QString("单价：%1").arg(add));
		}
		break;
	case Qt::Key_F2:
		//松手检测
		if (sleeptime < 270)
		{	//单价不能小于0
			sleeptime = sleeptime + 30;
			timer->stop();
			timer->start(sleeptime);
			add = add - 2;
			ui->oneLabel->setText(QString("单价：%1").arg(add));
		}
		break;
	default:
		break;
	}
}

//游戏开始
bool AMSnake::play()
{
	bool isOK;
	QString mapLenStr;
	for (;;)
	{
		mapLenStr = QInputDialog::getText(this, "初始条件设置", "请输入贪吃蛇地图的边长（必须为偶数）：",
										QLineEdit::Normal, "16", &isOK);
		if(isOK)
		{
			if(mapLenStr.toInt() & 1)
			{	//如果是奇数，报错并重新获取
				QMessageBox::information(this, "帅气的龙少提示你：", mapLenStr + "是奇数，请输入偶数！",
										QMessageBox::Yes, QMessageBox::Yes);
				continue;
			}
			else
			{
				mapLength = mapLenStr.toInt();
				mapSize = 680 / (mapLength+2);
				initGame();
				this->show();
				return true;
			}
		}
		else
		{	//如果取消了，直接退出自动贪吃蛇
			QMessageBox::information(this, "帅气的龙少提示你：", "退出成功！",
									QMessageBox::Yes, QMessageBox::Yes);
			return false;
		}
	}
}

//静音复选框
void AMSnake::on_soundOnOffCheckBox_stateChanged(int arg1)
{
	switch(arg1)
	{
	case Qt::CheckState::Checked:		//选中
		//开启静音
		bgPlayer->stop();
		jbPlayer->stop();
		swPlayer->stop();
		bgPlayer->setPlaylist(nullptr);
		jbPlayer->setPlaylist(nullptr);
		swPlayer->setPlaylist(nullptr);
		//复选框内容改为取消静音
		ui->soundOnOffCheckBox->setText("取消静音");
		focusNextPrevChild(true);
		break;
	case Qt::CheckState::Unchecked:		//未选中
		//取消静音
		bgPlayer->setPlaylist(bgPlaylist);
		jbPlayer->setPlaylist(jbPlaylist);
		swPlayer->setPlaylist(swPlaylist);
		bgPlayer->play();
		//复选框内容改为静音
		ui->soundOnOffCheckBox->setText("静音");
		focusNextPrevChild(true);
		break;
	default:
		break;
	}
}
