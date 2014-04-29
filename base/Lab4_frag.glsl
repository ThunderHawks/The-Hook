struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform vec4 uLightVec;
uniform vec3 uLColor;
uniform vec3 uCamVec;
uniform Material uMat;

varying vec3 vNorm; 
varying vec3 vPos;

void main() {
   float angleNL, angleVR, r, g, b;

   vec3 norm = normalize(vNorm);
   vec3 pos = normalize(vPos);
   vec3 light = normalize(vec3(uLightVec.r, uLightVec.g, uLightVec.b));  // Directional light
   vec3 view = normalize(uCamVec - vPos);
   vec3 refl;

   angleNL = clamp(dot(norm, light), 0.0, 1.0);

   refl = reflect(-light, norm);
   angleVR = clamp(dot(normalize(refl), view), 0.0, 1.0);

   if (angleNL > 0.7) {
      r = (uLColor.r * uMat.dColor.r);
      g = (uLColor.g * uMat.dColor.g);
      b = (uLColor.b * uMat.dColor.b);
   }
   else if (angleNL > 0.4) {
      r = (uLColor.r * uMat.dColor.r * 0.7);
      g = (uLColor.g * uMat.dColor.g * 0.7);
      b = (uLColor.b * uMat.dColor.b * 0.7);
   }
   else {
      r = (uLColor.r * uMat.dColor.r * 0.4);
      g = (uLColor.g * uMat.dColor.g * 0.4);
      b = (uLColor.b * uMat.dColor.b * 0.4);
   }
   
   if (angleVR > 0.95) {
      r += uLColor.r * uMat.sColor.r * pow(1.0, uMat.shine);
      g += uLColor.g * uMat.sColor.g * pow(1.0, uMat.shine);
      b += uLColor.b * uMat.sColor.b * pow(1.0, uMat.shine);
   }
   
   if (dot(view, norm) < 0.4) {
      //r = g = b = 0.0;
   }


   r = (uLColor.r * uMat.dColor.r * angleNL) + (uLColor.r * uMat.sColor.r * pow(angleVR, uMat.shine)) + (uLColor.r * uMat.aColor.r);
   g = (uLColor.g * uMat.dColor.g * angleNL) + (uLColor.g * uMat.sColor.g * pow(angleVR, uMat.shine)) + (uLColor.g * uMat.aColor.g);
   b = (uLColor.b * uMat.dColor.b * angleNL) + (uLColor.b * uMat.sColor.b * pow(angleVR, uMat.shine)) + (uLColor.b * uMat.aColor.b);

   gl_FragColor = vec4(r, g, b, 1.0);
   //gl_FragColor = vec4(norm, 1.0);
}
