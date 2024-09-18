﻿
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

layout (std140, binding = 0) uniform Matrices{
  mat4 projection;
  mat4 view;
};

uniform mat4 model;

out vec2 TexCoords;

void main(){
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  TexCoords = aTex;
}
