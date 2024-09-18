#include "littlethings.h"


/******************* 1. Cube 立方体 ***********************/
Cube::Cube(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Cube::~Cube(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}

void Cube::init(){
  float vertices[] = {
    // positions        // textures        // normals
    //Back Face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, //bottom-left
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, //top-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,

    //Front Face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,

    //Left Face
    -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

    //Right Face
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    //Bottom Face
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,

    //Top Face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  };
  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Cube::draw(GLboolean isTexture, GLboolean isNormal){
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  if(isTexture){
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  }
  if(isNormal){
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
  }

  core->glDrawArrays(GL_TRIANGLES, 0, 36);
}

/******************* 2. Plane 立方体 ***********************/
Plane::Plane(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Plane::~Plane(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}

void Plane::init(){
  float vertices[] = {
    // positions        // textures        // normals
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  };
  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Plane::draw(GLboolean isTexture, GLboolean isNormal){
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  if(isTexture){
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  }
  if(isNormal){
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
  }

  core->glDrawArrays(GL_TRIANGLES, 0, 6);
}

/******************* 3. Coordinate 立方体 ***********************/
Coordinate::Coordinate(): VBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Coordinate::~Coordinate(){
  if(VBO != 0)
    core->glDeleteBuffers(1, &VBO);
}

void Coordinate::init(){

  float vertices[] = {
      // positions
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f
  };

  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


}

void Coordinate::draw(){
  core->glEnableVertexAttribArray(0);
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  core->glDrawArrays(GL_LINES, 0, 6);
}

/******************* 4. UBufferObject ubo缓冲对象 ***********************/

UBufferObject::UBufferObject(): UBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

UBufferObject::~UBufferObject(){

}

void UBufferObject::init(int ubo_size){
  core->glGenBuffers(1, &UBO);
  core->glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  core->glBufferData(GL_UNIFORM_BUFFER, ubo_size, NULL, GL_STATIC_DRAW);
  core->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBufferObject::bindBufferRange(int bindingIndex, int offset, int size){
    core->glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, this->UBO, offset, size);
}

void UBufferObject::subDataMat4(int offset, GLvoid *data){
  core->glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  core->glBufferSubData(GL_UNIFORM_BUFFER, offset, 4 * sizeof(QVector4D), data);
  core->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Sphere::Sphere(): posVBO(0){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Sphere::~Sphere(){
  if(posVBO != 0){
    core->glDeleteBuffers(1, &posVBO);
    core->glDeleteBuffers(1, &uvVBO);
    core->glDeleteBuffers(1, &norVBO);
    core->glDeleteBuffers(1, &EBO);

  }
}

void Sphere::init(){
  /*
   * 单位球的生成
   */
  QVector<QVector3D> vec_pos;
  QVector<QVector2D> vec_uv;
  QVector<QVector3D> vec_nor;

  const GLuint X_SEGMENTS = 64;
  const GLuint Y_SEGMENTS = 64;
  const float PI = 3.141592653;

  for(GLuint y = 0; y <= Y_SEGMENTS; ++y){
    for(GLuint x = 0; x <= X_SEGMENTS; ++x){
      float xSegment = x * 1.0f / X_SEGMENTS; //转为浮点数
      float ySegment = y * 1.0f / Y_SEGMENTS;
      float yPos = qCos(ySegment * PI);
      float xPos = qSin(ySegment * PI) * qCos(xSegment * 2.0f * PI);
      float zPos = qSin(ySegment * PI) * qSin(xSegment * 2.0f * PI);
      vec_pos.push_back(QVector3D(xPos, yPos, zPos));
      vec_uv.push_back(QVector2D(xSegment, ySegment));
      vec_nor.push_back(QVector3D(xPos, yPos, zPos));

    }
  }


  core->glGenBuffers(1, &posVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * vec_pos.size(), &vec_pos[0], GL_STATIC_DRAW);

  core->glGenBuffers(1, &uvVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * vec_uv.size(), &vec_uv[0], GL_STATIC_DRAW);

  core->glGenBuffers(1, &norVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, norVBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * vec_nor.size(), &vec_nor[0], GL_STATIC_DRAW);

//  绑定索引
  for(int y = 0; y < Y_SEGMENTS; ++y){
    for(int x = 0; x <= X_SEGMENTS; ++x){
      vec_indices.push_back(y     * (Y_SEGMENTS+1) + x);
      vec_indices.push_back((y+1) * (Y_SEGMENTS+1) + x);
    }
  }
  core->glGenBuffers(1, &EBO);
  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vec_indices.size(), &vec_indices[0], GL_STATIC_DRAW);

}

void Sphere::draw(GLboolean isTexture, GLboolean isNormal){
  core->glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  core->glEnableVertexAttribArray(0);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
  if(isTexture){
    core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), (void*)0);
  }
  if(isNormal){
    core->glBindBuffer(GL_ARRAY_BUFFER, norVBO);
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
  }
  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  core->glDrawElements(GL_TRIANGLE_STRIP, vec_indices.size(), GL_UNSIGNED_INT, 0); //最后一个参数指向索引数组，但仅是当不在使用索引缓冲对象的时候


}
