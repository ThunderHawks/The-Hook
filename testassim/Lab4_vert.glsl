/*uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix1;
uniform vec3 uColor;
uniform vec3 uLColor;

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aLightVec;

varying vec3 vColor;
varying vec4 vPosition;

void main() {

  vPosition = uModelMatrix1* vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  vPosition = uViewMatrix* vPosition;
  gl_Position = uProjMatrix*vPosition;
  //gl_FrontColor = vec4(uColor.r, uColor.g, uColor.b, 1.0);
  //vColor = aNormal;
  //vColor = vec3(uColor.r, uColor.g, uColor.b);
}*/
struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix1;

uniform float uShadeMode;
uniform Material uMat;
attribute vec3 aPosition;
uniform vec4 aLightVec;
uniform vec3 uLColor;
attribute vec3 aNormal;
uniform vec3 uCamVec;

varying vec3 vColor;
varying vec3 camVec;

varying vec3 normal;
varying vec4 light;
varying vec4 view;
varying vec4 reflection;
varying vec4 vPosition;

uniform vec3 uColor;

void main() {
  vec4 R, V, L, N, T; //reflection, view, light, normal, temp

  /* First model transforms */
  vPosition = uModelMatrix1* vec4(aPosition, 1.0);

  //Transform light space
  vec4 tempLight = uViewMatrix * aLightVec;

   vPosition = uViewMatrix * vPosition;
   gl_Position = uProjMatrix * vPosition;

   camVec = vec3(uCamVec.x - vPosition.x, uCamVec.y - vPosition.y, uCamVec.z - vPosition.z);

   L = normalize(tempLight);
   L = vec4(-1.0 * L.x, -1.0 * L.y, L.z, 0.0);
   
   N = uModelMatrix1 * vec4(aNormal, 0);

   //Diffusion

   if(uShadeMode == 0.0) {
      N = -1.0 * normalize(N);
   } 
   else if(uShadeMode == 1.0) {
      N = normalize(N);
   }
   R = reflect(L, N);
   R = normalize(R);
   V = vec4(normalize(camVec), 0.0);
     
   //Pass aNormal to frag
   normal = aNormal;
   view = V;
   light = aLightVec;
   reflection = R;      

   vColor == uMat.dColor + uMat.sColor + uMat.aColor;
}
