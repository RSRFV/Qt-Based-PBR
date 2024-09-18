
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;

layout (std140, binding = 0) uniform Matrices{
  mat4 projection;
  mat4 view;
};

uniform mat4 model;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;


void main(){
  TexCoords = aTex;
  WorldPos = vec3(model * vec4(aPos, 1.0f));
  Normal = mat3(model) * aNor; //没有scale的影响，所以不取逆求转置，用mat3,去除translate的影响
  gl_Position = projection * view * vec4(WorldPos, 1.0f);

}
