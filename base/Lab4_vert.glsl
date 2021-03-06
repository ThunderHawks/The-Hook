struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
   float alpha;
};

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uLightProjMatrix;
uniform mat4 uLightViewMatrix;

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aUVVertex;
attribute float aPointSize;

varying vec3 vNorm;
varying vec3 vPos;
varying vec4 vShadowPos;
varying vec2 vUV;

varying vec2 vTexCoord;
attribute vec2 aTexCoord;

attribute vec2 textPos;

uniform float uMGuiMode;
uniform float uTextMode;
uniform float uMatMode;

// animation stuff here
uniform int animFlag;
attribute float numWeights;
uniform mat4 bonesMatrix[30];
attribute vec4 weights;
attribute vec4 joints;



void main() {
   vec4 transPos, transNormal;
   mat4 VMNDCtoTex = mat4(0.38, 0.0, 0.0, 0.0,
                          0.0, 0.485, 0.0, 0.0,
                          0.0, 0.0, 0.5, 0.0,
                          0.38, 0.485, 0.5, 1.0);
   mat4 NDCtoTex = mat4(0.5, 0.0, 0.0, 0.0,
                        0.0, 0.5, 0.0, 0.0,
                        0.0, 0.0, 0.5, 0.0,
                        0.5, 0.5, 0.5, 1.0);

   mat4 animationMatrix = mat4(1.0);

   //more animation stuff here
   if (animFlag == 1) {
      animationMatrix =
         weights[0] * bonesMatrix[int(joints[0])] +
         weights[1] * bonesMatrix[int(joints[1])] +
         weights[2] * bonesMatrix[int(joints[2])] +
         weights[3] * bonesMatrix[int(joints[3])];
   } else {
      animationMatrix = mat4(1.0);
   }

//  if (uTextMode == 1.0) {
//    vTexCoord = aTexCoord;
//    gl_Position = vec4(textPos, 0, 1);
//  } else {
     /* First model transforms */
     gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * animationMatrix * vec4(aPosition, 1);
    
     /* Calculating transformed position and normal */
     transPos = uModelMatrix * vec4(aPosition, 1);
     transNormal = uModelMatrix * vec4(aNormal, 0);
     
     /* Interpolate the position and normal and send to the fragment shader */
     vNorm = vec3(transNormal.x, transNormal.y, transNormal.z);
     vPos = vec3(transPos.x, transPos.y, transPos.z);
     /* Vertex location in texture coordinates */
     vShadowPos = NDCtoTex * uLightProjMatrix * uLightViewMatrix * transPos;

     vTexCoord = aTexCoord;
     vUV = aUVVertex;

     gl_PointSize = aPointSize*15.0/length(gl_Position);
//   }
}
