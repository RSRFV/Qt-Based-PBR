﻿#include "shader.h"

void Shader::compile(const QString& vertexSource, const QString& fragmentSource, const QString& geometrySource){
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceFile(vertexSource);
    if(!success){
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
        qDebug() << vertexShader.log() << endl;
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success  = fragmentShader.compileSourceFile(fragmentSource);
    if(!success){
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
        qDebug() << fragmentShader.log() << endl;
    }

    QOpenGLShader geometryShader(QOpenGLShader::Geometry);
    if(geometrySource != NULL){
        success  = geometryShader.compileSourceFile(geometrySource);
        if(!success){
            qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED" << endl;
            qDebug() << geometryShader.log() << endl;
        }
    }


    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShader(&vertexShader);
    shaderProgram->addShader(&fragmentShader);
    if(geometrySource != NULL)
        shaderProgram->addShader(&geometryShader);
    success = shaderProgram->link();
    if(!success){
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
        qDebug() << shaderProgram->log() << endl;
    }
}

Shader::~Shader(){
    //没有析构的ShaderProgram，可tnd的自带opengl函数没有对QOpenGLShaderProgram的 delete删除函数啊

//    delete shaderProgram;
}


