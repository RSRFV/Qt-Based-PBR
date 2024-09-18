#ifndef LITTLETHINGS_H
#define LITTLETHINGS_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QtMath>
#include <QDebug>
/*
  这个类存储了一些 小玩意，为这些单独小玩意单独建类又好麻烦
  目前这些小玩意有：
  1.cube立方体
  2.plane平面
  3.Coordinate坐标系
  4.uniform buffer object 主要保证着色器中view与projection矩阵的唯一性，不再重复指定，
  5.sphere 球体
*/

/************* 1. Cube立方体 ******************/

class Cube{
public:
  Cube();
  ~Cube();
  void init();
  void draw(GLboolean isTexture = GL_FALSE,GLboolean isNormal = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************* 2. plane二维平面 ******************/

class Plane{
public:
  Plane();
  ~Plane();
  void init();
  void draw(GLboolean isTexture = GL_FALSE,GLboolean isNormal = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************ 3. 坐标类，一个简单的显示xyz坐标的类 **************/
class Coordinate{
public:
  Coordinate();
  ~Coordinate();
  void init();
  void draw();
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint VBO;
};

/************ 4. ubo类，一个uniform buffer object类 **************/
class UBufferObject{
public:
  UBufferObject();
  ~UBufferObject();
  void init(int ubo_size);
  void bindBufferRange(int bindingIndex, int offset, int size);
  void subDataMat4(int offset, GLvoid* data);
private:
  QOpenGLFunctions_3_3_Core *core;
  GLuint UBO;
};

/************* 5. Sphere 球体 ******************/

class Sphere{
public:
  Sphere();
  ~Sphere();
  void init();
  void draw(GLboolean isTexture = GL_FALSE,GLboolean isNormal = GL_FALSE);
private:
  QOpenGLFunctions_3_3_Core *core;
  QVector<GLuint> vec_indices;

  GLuint posVBO;
  GLuint uvVBO;
  GLuint norVBO;

  GLuint EBO;
};
#endif // LITTLETHINGS_H
