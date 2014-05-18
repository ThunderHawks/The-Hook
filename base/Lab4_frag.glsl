struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
   float alpha;
};

uniform vec4 uLightVec;
uniform vec3 uLColor;
uniform vec3 uCamPos;
uniform sampler2D uTexUnit;
uniform Material uMat;
uniform int uShadeMode;

varying vec3 vNorm; 
varying vec3 vPos;
varying vec4 vShadowPos;

void main() {
   float angleNL, angleVR, depth, dist;
   vec4 shadowPos;
   vec3 color;

   vec3 norm = normalize(vNorm);
   vec3 pos = normalize(vPos);
   vec3 light = normalize(vec3(uLightVec.x, uLightVec.y, uLightVec.z));  // Directional light
   vec3 view = normalize(uCamPos - vPos);
   vec3 refl = normalize(reflect(-light, norm));

   angleNL = clamp(dot(norm, light), 0.0, 1.0);

   angleVR = dot(refl, view);
   angleVR = angleVR < 0.0 ? 0.0 : pow(clamp(angleVR, 0.0, 1.0), uMat.shine);

   shadowPos = vShadowPos / vShadowPos.w;
   depth = texture2D(uTexUnit, shadowPos.xy).z; // Shadow map depth
   dist = vShadowPos.z - 0.005; // Distance from light to fragment

   // Diffuse lighting
/*
   if (angleNL > 0.7)
      color = uLColor * uMat.dColor * 0.85;
   else if (angleNL > 0.3)
      color = uLColor * uMat.dColor * 0.5;
   else
      color = uLColor * uMat.dColor * 0.15;
*/
   if (angleNL > 0.8)
      color = uLColor * uMat.dColor;
   else if (angleNL > 0.6)
      color = uLColor * uMat.dColor * 0.8;
   else if (angleNL > 0.4)
      color = uLColor * uMat.dColor * 0.6;
   else if (angleNL > 0.2)
      color = uLColor * uMat.dColor * 0.4;
   else
      color = uLColor * uMat.dColor * 0.2;

   // Test if the fragment is in a shadow
   if (uShadeMode == 1) {
      if (depth < dist && dist < 1.0 && abs(angleNL) >= 0.01) {
         color *= 0.3;
      }
   }
   else {
      float multiplier = 0.0;
      float offsetX, offsetY;

      // Blur the shadow by averaging the surrounding shadow map values in a 7x7 texel square
      for (offsetX = -3.0; offsetX <= 3.0; offsetX++) {
         for (offsetY = -3.0; offsetY <= 3.0; offsetY++) {
            // Get the shadow map depth at a neighboring texel
            depth = texture2D(uTexUnit, vec2(shadowPos.x + offsetX / 1600.0, shadowPos.y + offsetY / 800.0)).z;
            // If the area is not shadowed, increase the multiplier (make brighter)
            if (!(depth < dist && dist < 1.0 && abs(angleNL) >= 0.01))
               multiplier += 1.0;
         }
      }
      // Only darken the pixel if the shadow isn't negligible
      if (multiplier / 49.0 <= 0.8)
         color *= multiplier / 49.0;
   }

   // Specular lighting
   //if (angleVR > 0.95 && uMat.shine >= 100.0)
      //color += uLColor * uMat.sColor;

   // Ambient lighting
   color += uLColor * uMat.aColor;

   if (dot(view, norm) < 0.3) {
      //r = g = b = 0.0;
   }

   //color = (uLColor * uMat.dColor) + (uLColor * uMat.sColor * angleVR) + (uLColor * uMat.aColor);   

   gl_FragColor = vec4(color, uMat.alpha);
   //gl_FragColor = vec4(norm, 1.0);
   //gl_FragColor = vec4(depth, depth, depth, 1.0);
   //gl_FragColor = vec4(dist, dist, dist, 1.0);

}
