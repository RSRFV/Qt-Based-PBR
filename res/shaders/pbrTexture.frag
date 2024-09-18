
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

//uniform vec3 albedo;// 反射率，一般是材质本身的颜色
//uniform float metallic; //金属度
//uniform float roughness;// 粗糙度
//uniform float ao;//环境光遮蔽颜色，主要是边缘的暗颜色，比如砖缝


uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

//lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 cameraPos; //摄像机位置

const float PI = 3.14159265359;

//获得法线，没推导出来，囧= =
vec3 getNormalFromMap(){
  vec3 tangentNormal = texture2D(normalMap, TexCoords).xyz * 2.0 - 1.0;

  vec3 Q1  = dFdx(WorldPos);
  vec3 Q2  = dFdy(WorldPos);
  vec2 st1 = dFdx(TexCoords);
  vec2 st2 = dFdy(TexCoords);

  vec3 N   = normalize(Normal);
  vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
  vec3 B  = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  return normalize(TBN * tangentNormal);
}

//D,正态分布函数，与中间向量h一致的微平面的概率
float DistributionGGX(vec3 N, vec3 H, float roughness){
  float a = roughness * roughness;  //Epic采用的光照模型，在D,与G几何遮蔽函数中，用粗糙度的平方，会更加自然。
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0f);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = NdotH2 * (a2 - 1.0f) + 1.0f;
  denom = PI * denom * denom; //分母

  return nom / denom; //防止denom等于0.0f
}

//F 菲涅尔方程，返回 反射光的比率
vec3 fresnelSchlick(vec3 H, vec3 V, vec3 F0){
  float HdotV = clamp(dot(H, V), 0.0f, 1.0f); //将这个值卡在0.0f到1.0f之间

  return F0 + (1.0f - F0) * pow((1.0f - HdotV), 5.0f);
}


//G 几何遮蔽函数，平面上的微平面有可能挡住其他的微平面，从而减少表面所反射的光线
float GeometrySchlickGGX(float NdotV, float roughness){
//  float a = roughness * roughness;//作者说的是在几何遮蔽函数中 采用roughness的平方，他的代码却没有用。。。。思考再三，我还是先写上吧
//  float r = roughness + 1.0f;
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f; //用的平行光的K


  float nom = NdotV; //分子
  float denom = NdotV * (1.0f - k) + k;
  return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
  float NdotV = max(dot(N, V), 0.0f);
  float NdotL = max(dot(N, L), 0.0f);
  float ggx1 = GeometrySchlickGGX(NdotV, roughness);
  float ggx2 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

void main(){


  vec3 albedo     = pow(texture2D(albedoMap, TexCoords).rgb, vec3(2.2)); //albedo一般是在srgb空间中创作的，故先转为线性空间
  float metallic  = texture2D(metallicMap, TexCoords).r;
  float roughness = texture2D(roughnessMap, TexCoords).r;
  float ao        = texture2D(aoMap, TexCoords).r;

  vec3 N = getNormalFromMap();
//    vec3 N = Normal;

  vec3 V = normalize(cameraPos - WorldPos);
  /*
    对于非电解质材料，菲涅尔方程的F0 = 0.04,比如塑料
  */
  vec3 F0 = vec3(0.04f, 0.04f, 0.04f);
  F0 = mix(F0, albedo, metallic);

  vec3 L0 = vec3(0.0f, 0.0f, 0.0f);
  for(int i = 0; i < 1; ++i){ //四个光源
    //1. 衰减的光照辐射强度，即Li(P, Wi)
    float distancePL = length(lightPositions[i] - WorldPos);//光源与点的距离
    float attenuation = 1.0f / (distancePL * distancePL);//衰变，在gamma校正后，用二次衰变显得比较真实
    vec3 radiance = lightColors[i] * attenuation; //辐射

    //2.计算BRDF，
    vec3 L = normalize(lightPositions[i] - WorldPos);
    vec3 H = normalize(V + L);
    float D = DistributionGGX(N, H, roughness);
    vec3 F = fresnelSchlick(H, V, F0);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 nom = D * F * G;//分子
    float denom = 4 * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
    vec3 specular = nom / max(denom, 0.001f);

    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD *= 1.0f - metallic; //如果是导电的金属，则没有被折射的漫反射，均为反射

    vec3 diffuse = kD * albedo / PI;
    L0 += (diffuse + specular) * radiance * max(dot(N, L), 0.0f);
  }

  vec3 ambient = vec3(0.03f) * albedo * ao;
  vec3 color = ambient + L0;

  //HDR
  color = color / (vec3(1.0f) + color);
  //gamma correct
  color = pow(color, vec3(1.0f/2.2f));

  FragColor = vec4(color, 1.0f);

}
