struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix1;
//
uniform int animFlag;
attribute float numWeights;
uniform mat4 bonesMatrix[30];
attribute vec3 weights;
attribute vec3 joints;
//
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
//
  mat4 animationMatrix = mat4(1.0);
  if (animFlag == 1) {
  		for (int i = 0; i < int(numWeights); i++) {
  			animationMatrix += weights[i] * bonesMatrix[int(joints[i])];
  		}
	   /*animationMatrix =
		 weights[0] * bonesMatrix[int(joints[0])] +
		 weights[1] * bonesMatrix[int(joints[1])] +
		 weights[2] * bonesMatrix[int(joints[2])];*/
  }
//
  /* First model transforms */
  vPosition = uModelMatrix1 * animationMatrix * vec4(aPosition, 1.0);

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
