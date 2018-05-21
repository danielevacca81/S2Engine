// MainWindow.h
// 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"

class TestScene;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void gui_createGLArea();

	TestScene      *_scene;
	Ui::MainWindow  _ui;
};

#endif // MAINWINDOW_H
