uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 eyePos;
uniform vec3 lightPos;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 tangent;

varying vec2 v_texCoord;
varying vec3 v_lightWS, v_viewWS;
varying mat3 tangentToWorldSpace;//3x3 matrix that converts vertex in tangent space into worldSpace
varying mat3  worldToTangentSpace;//3x3 matrix that converts vertex in world space into tangent Space
void main() {
   // you should fill in this function
   
   vec4 posWS=worldMat * vec4(position, 1.0);//object space to world space
   gl_Position = projMat * viewMat * posWS;//to clip space
   v_texCoord = texCoord;
   
   vec3 nor = mat3(worldMat) * normal;//vertex normal N
   vec3 tan = mat3(worldMat) * normalize(tangent);//tangent T
   vec3 bin = cross(nor, tan);//binormal B
   worldToTangentSpace = mat3(tan.x, bin.x, nor.x,
   									tan.y, bin.y, nor.y,
   								tan.z, bin.z, nor.z);
   //rotation matrix
   tangentToWorldSpace = mat3(tan.x, tan.y, tan.z,
   								bin.x, bin.y, bin.z,
   									nor.x, nor.y, nor.z);
   
   
  
   v_lightWS = (lightPos - posWS.xyz);//light vector in world space
   v_viewWS =(eyePos - posWS.xyz);//view direction vector in world space
}