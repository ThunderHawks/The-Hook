struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec4 uLightVec;
uniform vec3 uLColor;
uniform Material uMat;

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec3 vNorm;
varying vec3 vPos;

void main() {
   vec4 vPosition;
   vec4 transPos, transNormal;

   /* First model transforms */
   vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
   vPosition = uViewMatrix * vPosition;
   gl_Position = uProjMatrix*vPosition;
  
   /* Calculating transformed position and normal */
   transPos = uModelMatrix * vec4(aPosition, 1);
   transNormal = uModelMatrix * vec4(aNormal, 0);
   
   /* Interpolate the position and normal and send to the fragment shader */
   vNorm = vec3(transNormal.x, transNormal.y, transNormal.z);
   vPos = vec3(transPos.x, transPos.y, transPos.z);
}
