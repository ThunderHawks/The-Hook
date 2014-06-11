struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform vec4 uLightVec;
uniform vec3 uLColor;
uniform vec3 uCamPos;
uniform Material uMat;

varying vec3 vNorm; 
varying vec3 vPos;

void main() {
   float angleNL, angleVR, r, g, b;

   vec3 norm = normalize(vNorm);
   vec3 pos = normalize(vPos);
   vec3 light = normalize(vec3(uLightVec.x, uLightVec.y, uLightVec.z));  // Directional light
   vec3 view = normalize(uCamPos - vPos);
   vec3 refl;

   angleNL = clamp(dot(norm, light), 0.0, 1.0);

   refl = normalize(reflect(-light, norm));
   angleVR = dot(refl, view);
   angleVR = angleVR < 0.0 ? 0.0 : pow(clamp(angleVR, 0.0, 1.0), uMat.shine);

   if (angleNL > 0.8) {
      r = (uLColor.r * uMat.dColor.r);
      g = (uLColor.g * uMat.dColor.g);
      b = (uLColor.b * uMat.dColor.b);
   }
   else if (angleNL > 0.6) {
      r = (uLColor.r * uMat.dColor.r * 0.8);
      g = (uLColor.g * uMat.dColor.g * 0.8);
      b = (uLColor.b * uMat.dColor.b * 0.8);
   }
   else if (angleNL > 0.4) {
      r = (uLColor.r * uMat.dColor.r * 0.6);
      g = (uLColor.g * uMat.dColor.g * 0.6);
      b = (uLColor.b * uMat.dColor.b * 0.6);
   }
   else if (angleNL > 0.2) {
      r = (uLColor.r * uMat.dColor.r * 0.4);
      g = (uLColor.g * uMat.dColor.g * 0.4);
      b = (uLColor.b * uMat.dColor.b * 0.4);
   }
   else {
      r = (uLColor.r * uMat.dColor.r * 0.2);
      g = (uLColor.g * uMat.dColor.g * 0.2);
      b = (uLColor.b * uMat.dColor.b * 0.2);
   }
 
   if (angleVR > 0.95 && uMat.shine >= 100.0) {
      r += uLColor.r * uMat.sColor.r * pow(1.0, uMat.shine);
      g += uLColor.g * uMat.sColor.g * pow(1.0, uMat.shine);
      b += uLColor.b * uMat.sColor.b * pow(1.0, uMat.shine);
   }

   r += uLColor.r * uMat.aColor.r;
   g += uLColor.g * uMat.aColor.g;
   b += uLColor.b * uMat.aColor.b;

   if (dot(view, norm) < 0.3) {
      //r = g = b = 0.0;
   }


   //r = (uLColor.r * uMat.dColor.r * angleNL) + (uLColor.r * uMat.sColor.r * angleVR) + (uLColor.r * uMat.aColor.r);
   //g = (uLColor.g * uMat.dColor.g * angleNL) + (uLColor.g * uMat.sColor.g * angleVR) + (uLColor.g * uMat.aColor.g);
   //b = (uLColor.b * uMat.dColor.b * angleNL) + (uLColor.b * uMat.sColor.b * angleVR) + (uLColor.b * uMat.aColor.b);

   gl_FragColor = vec4(r, g, b, 1.0);
   //gl_FragColor = vec4(norm, 1.0);
}
