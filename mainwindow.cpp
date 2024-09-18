#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this); //如果ui和oglmanager的生成顺序相反，会失去manager的鼠标焦点，鼠标操作无效化
  oglManager = new OGLManager(this);

  QTimer *timer = new QTimer(this); //每10ms刷新一次OpenGL界面，即100帧
  connect(timer, &QTimer::timeout, this, &MainWindow::updateOGL);
  timer->start(16);

  /******** 设置背景颜色 ***********/
  QPalette pal(this->palette());

  pal.setColor(QPalette::Background, QColor(99, 103, 106));
  this->setAutoFillBackground(true);
  this->setPalette(pal);
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
  oglManager->handleKeyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
  oglManager->handleKeyReleaseEvent(event);
}



void MainWindow::on_pushButton_clicked(){
  oglManager->isLineMode = !oglManager->isLineMode;
}

void MainWindow::on_radioButton_clicked(bool checked){
  if(checked)
    oglManager->draw_mode = NORMAL;
}

void MainWindow::on_radioButton_2_clicked(bool checked){
  if(checked)
    oglManager->draw_mode = TEXTURE;
}
