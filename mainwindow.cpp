#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	cms = nullptr;
	ams = nullptr;

	setWindowTitle("龙少贪吃蛇");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//经典模式
void MainWindow::on_classicalModeBtn_clicked()
{
    this->hide();
	cms = new CMSnake;
	QObject::connect(cms, SIGNAL(showMainWindow()), this, SLOT(receiveShowMainWindow()));
	if(!cms->play())
		receiveShowMainWindow();
}

//自动贪吃蛇
void MainWindow::on_autoModeButton_clicked()
{
	this->hide();
	ams = new AMSnake;
	QObject::connect(ams, SIGNAL(showMainWindow()), this, SLOT(receiveShowMainWindow()));
	if(!ams->play())
		receiveShowMainWindow();
}

//接受showMainWindow信号
void MainWindow::receiveShowMainWindow()
{
	if(cms != nullptr) {delete cms; cms = nullptr;}
	if(ams != nullptr) {delete ams; ams = nullptr;}
	this->show();
}
