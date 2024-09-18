#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oglmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);   //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event);  //键盘释放事件
private:
    Ui::MainWindow *ui;
    OGLManager *oglManager;
private slots:
    void updateOGL(){
      oglManager->update();
    }

    void on_pushButton_clicked();
    void on_radioButton_clicked(bool checked);
    void on_radioButton_2_clicked(bool checked);
};

#endif // MAINWINDOW_H
