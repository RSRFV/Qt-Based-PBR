﻿#ifndef OGLMANAGER_H
#define OGLMANAGER_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>
#include "camera.h"
#include <QMouseEvent>


enum DRAW_MODE{
  NORMAL,
  TEXTURE
};

class OGLManager : public QOpenGLWidget{
public:
  explicit OGLManager(QWidget *parent = 0);
  ~OGLManager();
  void handleKeyPressEvent(QKeyEvent *event);   //键盘按下事件
  void handleKeyReleaseEvent(QKeyEvent *event);  //键盘释放事件

  GLboolean keys[1024];//获取键盘按键，实现多键触控
  GLboolean isLineMode;
  DRAW_MODE draw_mode;
protected:
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event) override;    //滚轮事件
  void mousePressEvent(QMouseEvent *event) override;  //鼠标按下事件
  void mouseReleaseEvent(QMouseEvent *event) override; //鼠标释放事件
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();
private:
  void processInput(GLfloat dt);//摄像机键盘处理函数，
  void updateGL();//opengl 更新函数

  QOpenGLFunctions_3_3_Core *core;

  GLboolean isFirstMouse;
  GLboolean isLeftMousePress;
  GLint lastX;
  GLint lastY;

  QTime time;
  GLfloat deltaTime;
  GLfloat lastFrame;//上一帧

  Camera *camera;

};



#endif // OGLMANAGER_H
