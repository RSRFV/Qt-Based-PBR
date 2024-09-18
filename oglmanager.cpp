#include "oglmanager.h"
#include <QPalette>
#include "resourcemanager.h"
#include "littlethings.h"
#include <QDebug>


const QVector3D CAMERA_POSITION(0.0f, 0.0f, 3.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 600;
const int OGLMANAGER_HEIGHT = 600;

Coordinate *coordinate;
Cube *cube;
Plane *plane;
Sphere *sphere;
UBufferObject *ubo;
QVector<QVector3D> lightPositions;
QVector<QVector3D> lightColors;

float clamp(float v, float min, float max){
  if(v < min)
    return min;
  else if(v > max)
    return max;

  return v;
}

OGLManager::OGLManager(QWidget *parent) : QOpenGLWidget(parent){
  this->setGeometry(10, 20, OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT);
  QSurfaceFormat newGLFormat = this->format();  //开启抗锯齿
  newGLFormat.setSamples(4);
  this->setFormat(newGLFormat);
}

OGLManager::~OGLManager(){
  if(camera)
    delete camera;

}

void OGLManager::handleKeyPressEvent(QKeyEvent *event){
  GLuint key = event->key();
  if(key >= 0 && key <= 1024)
    this->keys[key] = GL_TRUE;

}

void OGLManager::handleKeyReleaseEvent(QKeyEvent *event){
  GLuint key = event->key();
  if(key >= 0 && key <= 1024)
      this->keys[key] = GL_FALSE;
}


void OGLManager::initializeGL(){
  /*********** OGL核心 ***********/
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
  core->glEnable(GL_DEPTH_TEST);

//  qDebug() << qCos(0) << " " << qCos(90);
//  qDebug() << qCos(30.0f/180.0f*3.1415926) << " " << qCos(60.0f/180.0f*3.1415926);
//  qDebug() << qCos(qDegreesToRadians(30.0f)) << " " << qCos(qDegreesToRadians(60.0f));


  /*********** 初始化模型细节参数  *************/
  isLineMode = GL_FALSE;
//  draw_mode = TEXTURE;
  draw_mode = NORMAL;


  /*********** 键鼠响应及时间帧数操作  *************/
  for(GLuint i = 0; i != 1024; ++i) //初始化键盘按键
    keys[i] = GL_FALSE;

  deltaTime = 0.0f;
  lastFrame = 0.0f;

  isFirstMouse = GL_TRUE;
  isLeftMousePress = GL_FALSE;
  lastX = width() / 2.0f;
  lastY = height() / 2.0f;

  time.start();

  /************ 摄像机 ***********/
  camera = new Camera(CAMERA_POSITION);

  /*********** Cube  *************/
  cube = new Cube();
  cube->init();

  /*********** Cube  *************/
  sphere = new Sphere();
  sphere->init();

  /*********** Plane  *************/
  plane = new Plane();
  plane->init();


  /************ 坐标 ***********/
  coordinate = new Coordinate();
  coordinate->init();

  /************ Uniform缓冲对象 ***********/
  ubo = new UBufferObject();
  ubo->init(2 * 4 * sizeof(QVector4D));
  ubo->bindBufferRange(0, 0, 2 * 4 * sizeof(QVector4D));

   /************ 载入shader ***********/
  ResourceManager::loadShader("coordinate", ":/shaders/res/shaders/coordinate.vert", ":/shaders/res/shaders/coordinate.frag");
  ResourceManager::loadShader("cube", ":/shaders/res/shaders/cube.vert", ":/shaders/res/shaders/cube.frag");
  ResourceManager::loadShader("plane", ":/shaders/res/shaders/plane.vert", ":/shaders/res/shaders/plane.frag");
  ResourceManager::loadShader("pbrlighting", ":/shaders/res/shaders/pbrlighting.vert", ":/shaders/res/shaders/pbrlighting.frag");
  ResourceManager::loadShader("pbrTexture", ":/shaders/res/shaders/pbrTexture.vert", ":/shaders/res/shaders/pbrTexture.frag");


  /************ 载入Texture ***********/
  ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");
  ResourceManager::loadTexture("cementwall", ":/textures/res/textures/cementwall.jpg");
  ResourceManager::loadTexture("albedo_rusted_iron", ":/textures/res/textures/rusted_iron/albedo.png");
  ResourceManager::loadTexture("ao_rusted_iron", ":/textures/res/textures/rusted_iron/ao.png");
  ResourceManager::loadTexture("metallic_rusted_iron", ":/textures/res/textures/rusted_iron/metallic.png");
  ResourceManager::loadTexture("roughness_rusted_iron", ":/textures/res/textures/rusted_iron/roughness.png");
  ResourceManager::loadTexture("normal_rusted_iron", ":/textures/res/textures/rusted_iron/normal.png");


  /***********  cube shader参数 **************/
  QMatrix4x4 model;
  ResourceManager::getShader("cube").use().setMatrix4f("model", model);
  ResourceManager::getShader("cube").use().setInteger("ambientMap", 0);

  /***********  plane shader参数 **************/
  model.setToIdentity();
  model.translate(0.0f, -0.8f, 0.0f);
  model.rotate(-90.0f, QVector3D(1.0f, 0.0f, 0.0f));//顺时针转90度。
  model.scale(2.0f);
  ResourceManager::getShader("plane").use().setMatrix4f("model", model);
  ResourceManager::getShader("plane").use().setInteger("ambientMap", 0);

  /***********  pbrlighting shader参数 **************/
//  model.setToIdentity();
//  ResourceManager::getShader("pbrlighting").use().setMatrix4f("model", model);
  ResourceManager::getShader("pbrlighting").use().setVector3f("albedo", QVector3D(0.5f, 0.0f, 0.0f));
  ResourceManager::getShader("pbrlighting").use().setFloat("ao", 1.0f);
//  ResourceManager::getShader("pbrlighting").use().setFloat("metallic", 1.0f);
//  ResourceManager::getShader("pbrlighting").use().setFloat("roughness", 0.5f);

  lightPositions.push_back(QVector3D(-10.0f,  10.0f, 10.0f));
  lightPositions.push_back(QVector3D( 10.0f,  10.0f, 10.0f));
  lightPositions.push_back(QVector3D(-10.0f, -10.0f, 10.0f));
  lightPositions.push_back(QVector3D( 10.0f, -10.0f, 10.0f));
  lightColors.push_back(QVector3D(300.0f, 300.0f, 300.0f)); //光源的颜色不是我们常见的0.0-1.0,因为300.0f表示的是光源的颜色强度，
  lightColors.push_back(QVector3D(300.0f, 300.0f, 300.0f));
  lightColors.push_back(QVector3D(300.0f, 300.0f, 300.0f));
  lightColors.push_back(QVector3D(300.0f, 300.0f, 300.0f));
  for(int i = 0; i < lightPositions.size(); ++i){
    QString index;
    ResourceManager::getShader("pbrlighting").use().setVector3f("lightPositions[" + index.setNum(i) + "]", lightPositions[i]);
    ResourceManager::getShader("pbrlighting").use().setVector3f("lightColors[" + index.setNum(i) + "]", lightColors[i]);
  }

  /***********  pbrTexture shader参数 **************/
  ResourceManager::getShader("pbrTexture").use().setInteger("albedoMap", 0);
  ResourceManager::getShader("pbrTexture").use().setInteger("normalMap", 1);
  ResourceManager::getShader("pbrTexture").use().setInteger("metallicMap", 2);
  ResourceManager::getShader("pbrTexture").use().setInteger("roughnessMap", 3);
  ResourceManager::getShader("pbrTexture").use().setInteger("aoMap", 4);

  for(int i = 0; i < 1; ++i){
    QString index;
    ResourceManager::getShader("pbrTexture").use().setVector3f("lightPositions[" + index.setNum(i) + "]", QVector3D(0, 0, 10.0f));
    ResourceManager::getShader("pbrTexture").use().setVector3f("lightColors[" + index.setNum(i) + "]", QVector3D(150.0f, 150.0f, 150.0f));
  }

  /***********  坐标 shader参数 **************/
  model.setToIdentity();
  model.scale(20.0f);
  ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);


  /************ 背景颜色参数调控 ***********/
  core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  core->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void OGLManager::resizeGL(int w, int h){
  core->glViewport(0, 0, w, h);
}

void OGLManager::paintGL(){
  /*********** 计算两次帧数之间的时间间隔  ***************/
  GLfloat currentFrame = (GLfloat)time.elapsed()/100;
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  this->processInput(deltaTime);
  this->updateGL();

  if(draw_mode == NORMAL){

    /*
     * 绘制 7x7 =49个球
     * 从左下角开始，以向右为正方向 先向右排列，再向上
     * 粗糙度 从左到右，依次上升
     * 金属度 从下到上，依次上升
     */
    int nrRows = 7;
    int nrCols = 7;
    float spacing = 2.5f;
    QMatrix4x4 model;
    for(int row = 0; row < nrRows; ++row){
      ResourceManager::getShader("pbrlighting").use().setFloat("metallic", row * 1.0f/nrRows);

      for(int col = 0; col < nrCols; ++col){
        ResourceManager::getShader("pbrlighting").use().setFloat("roughness", clamp(col * 1.0f/nrCols, 0.05f, 1.0f));
        model.setToIdentity();
        model.translate((col-(nrCols/2.0f))*spacing, (row-(nrRows/2.0f))*spacing, 0.0f);
        ResourceManager::getShader("pbrlighting").use().setMatrix4f("model", model);

        sphere->draw(GL_TRUE, GL_TRUE);
      }
    }

    /***********  绘制4个光源  *********/
    core->glActiveTexture(GL_TEXTURE0);
    ResourceManager::getTexture("brickwall").bind();
    for(int i = 0; i < lightPositions.size();++i){
      QMatrix4x4 model;
      model.translate(lightPositions[i]);
      ResourceManager::getShader("cube").use().setMatrix4f("model", model);
      cube->draw(GL_TRUE);
    }

  }else if(draw_mode == TEXTURE){
    ResourceManager::getShader("pbrTexture").use().setInteger("albedoMap", 0);
    ResourceManager::getShader("pbrTexture").use().setInteger("normalMap", 1);
    ResourceManager::getShader("pbrTexture").use().setInteger("metallicMap", 2);
    ResourceManager::getShader("pbrTexture").use().setInteger("roughnessMap", 3);
    ResourceManager::getShader("pbrTexture").use().setInteger("aoMap", 4);

    core->glActiveTexture(GL_TEXTURE0);
    ResourceManager::getTexture("albedo_rusted_iron").bind();
    core->glActiveTexture(GL_TEXTURE1);
    ResourceManager::getTexture("normal_rusted_iron").bind();
    core->glActiveTexture(GL_TEXTURE2);
    ResourceManager::getTexture("metallic_rusted_iron").bind();
    core->glActiveTexture(GL_TEXTURE3);
    ResourceManager::getTexture("roughness_rusted_iron").bind();
    core->glActiveTexture(GL_TEXTURE4);
    ResourceManager::getTexture("ao_rusted_iron").bind();

    int nrRows = 7;
    int nrCols = 7;
    float spacing = 2.5f;
    QMatrix4x4 model;
    for(int row = 0; row < nrRows; ++row){
      for(int col = 0; col < nrCols; ++col){
        model.setToIdentity();
        model.translate((col-(nrCols/2.0f))*spacing, (row-(nrRows/2.0f))*spacing, 0.0f);
        ResourceManager::getShader("pbrTexture").use().setMatrix4f("model", model);

        sphere->draw(GL_TRUE, GL_TRUE);
      }
    }

    /***********  绘制1个光源  *********/
    core->glActiveTexture(GL_TEXTURE0);
    ResourceManager::getTexture("brickwall").bind();
    model.setToIdentity();
    model.translate(QVector3D(0, 0, 10.0f));
    ResourceManager::getShader("cube").use().setMatrix4f("model", model);
    cube->draw(GL_TRUE);
  }
  /*********  绘制坐标系统 ************/
  ResourceManager::getShader("coordinate").use();
  coordinate->draw();


}

void OGLManager::processInput(GLfloat dt){
  if (keys[Qt::Key_W])
    camera->processKeyboard(FORWARD, dt);
  if (keys[Qt::Key_S])
    camera->processKeyboard(BACKWARD, dt);
  if (keys[Qt::Key_A])
    camera->processKeyboard(LEFT, dt);
  if (keys[Qt::Key_D])
    camera->processKeyboard(RIGHT, dt);
  if (keys[Qt::Key_E])
    camera->processKeyboard(UP, dt);
  if (keys[Qt::Key_Q])
    camera->processKeyboard(DOWN, dt);

}

void OGLManager::updateGL(){
  if(this->isLineMode)
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  QMatrix4x4 projection, view;
  projection.perspective(camera->zoom, (GLfloat)width()/(GLfloat)height(), 0.1f, 2000.f);
  view = camera->getViewMatrix();

  ResourceManager::getShader("pbrlighting").use().setVector3f("cameraPos", camera->position);

  ResourceManager::getShader("pbrTexture").use().setVector3f("cameraPos", camera->position);


  /***********  处理Uniform Buffer相关参数 **************/
  ubo->subDataMat4(0, &projection);
  ubo->subDataMat4(4*sizeof(QVector4D), &view);
}

void OGLManager::mouseMoveEvent(QMouseEvent *event){
  GLint xpos = event->pos().x();
  GLint ypos = event->pos().y();
  if(isLeftMousePress){
    if (isFirstMouse){
      lastX = xpos;
      lastY = ypos;
      isFirstMouse = GL_FALSE;
    }
//    qDebug() << "bbb";
    GLint xoffset = xpos - lastX;
    GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera->processMouseMovement(xoffset, yoffset);

  }
}

void OGLManager::mousePressEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton)//注意是button（）不是buttons（）；
    isLeftMousePress = GL_TRUE;
}

void OGLManager::mouseReleaseEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton){ //注意是button（）不是buttons（）；
    isLeftMousePress = GL_FALSE;
    isFirstMouse = GL_TRUE;
  }
}

void OGLManager::wheelEvent(QWheelEvent *event){
  QPoint offset = event->angleDelta();
  camera->processMouseScroll(offset.y()/20.0f);
}
