#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cmsnake.h"
#include "amsnake.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //经典模式
    void on_classicalModeBtn_clicked();
    //自动贪吃蛇
    void on_autoModeButton_clicked();
    //接受showMainWindow信号
    void receiveShowMainWindow();

signals:
    void showCMSnake();
    void showAMSnake();

private:
    Ui::MainWindow *ui;
	AMSnake *ams;
	CMSnake *cms;
};
#endif // MAINWINDOW_H
