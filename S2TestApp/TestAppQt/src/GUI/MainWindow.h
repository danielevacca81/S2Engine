// MainWindow.h
// 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

#include "GLResourcesLoader.h"


#include <QMainWindow>

class TestScene;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( QWidget *parent = Q_NULLPTR );
	~MainWindow();

private:
	void createScenes();

private slots:
	void initResources();

private:
	GLResourcesLoader _res;
	TestScene      *_sceneRight;
	TestScene      *_sceneLeft;
	Ui::MainWindow  _ui;
};

#endif // MAINWINDOW_H
