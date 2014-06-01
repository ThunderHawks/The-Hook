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

varying vec2 vTexCoord;
uniform float uGuiMode;
uniform float uTextMode;

void main() {
   float angleNL, angleNH, depth, dist;
   vec4 shadowPos;
   vec3 color, halfVec;

   vec3 norm, pos, light, view;

   vec4 texColor1 = texture2D(uTexUnit, vTexCoord);

   if (uTextMode == 1.0) {
      gl_FragColor = texture2D(uTexUnit, vTexCoord);//vec4(texColor1[0], texColor1[1], texColor1[2], 1);
   } 
   else if(uGuiMode == 1.0) {
    //  if(texColor1[0] == 0 && texColor1[1] == 0 && texColor1[2] == 0) {
     //    discard;
     // }
     // else {
         gl_FragColor = vec4(texColor1[0], texColor1[1], texColor1[2], 1);
     // }
   }
   else {
      norm = normalize(vNorm);
      pos = normalize(vPos);
      light = normalize(vec3(uLightVec.x, uLightVec.y, uLightVec.z));  // Directional light
      view = normalize(uCamPos - vPos);

      angleNL = clamp(dot(norm, light), 0.0, 1.0);

      halfVec = (light + view) / length(light + view);
      angleNH = dot(norm, halfVec);
      angleNH = pow(clamp(angleNH, 0.0, 1.0), uMat.shine * 4.0);

      shadowPos = vShadowPos / vShadowPos.w;
      depth = texture2D(uTexUnit, shadowPos.xy).z; // Shadow map depth = Z VALUE OF SHADOW MAP
      dist = vShadowPos.z - 0.005; // Distance from light to fragment = FRAG DEPTH FROM LIGHT'S PERSPECTIVE

   // Diffuse lighting
/*
   if (angleNL > 0.8)
      color = uLColor * uMat.dColor;
   else if (angleNL > 0.4)
      color = uLColor * uMat.dColor * 0.7;
   else
      color = uLColor * uMat.dColor * 0.35;
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
      if (uShadeMode == 0) {
        if (depth < dist && dist < 1.0 && abs(angleNL) >= 0.01)
            color *= 0.3;
         else if (uMat.alpha == 1.0) // If not in a shadow, add specular
            color += uLColor * uMat.sColor * angleNH;
      }
      else {
         float multiplier = 0.0;
         float offsetX, offsetY;

         // Blur the shadow by averaging the surrounding shadow map values in a 3x3 texel square
         for (offsetX = -1.0; offsetX <= 1.0; offsetX++) {
            for (offsetY = -1.0; offsetY <= 1.0; offsetY++) {
               // Get the shadow map depth at a neighboring texel
               depth = texture2D(uTexUnit, vec2(shadowPos.x + offsetX / 1600.0, shadowPos.y + offsetY / 800.0)).z;
               // If the area is not shadowed, increase the multiplier (make brighter)
               if (!(depth < dist && dist < 1.0 && abs(angleNL) >= 0.01))
                  multiplier += 1.0;
            }
         }
         // Only darken the pixel if the shadow isn't negligible. Otherwise, add specular
         if (multiplier / 9.0 <= 0.8)
            color *= multiplier / 9.0;
         else if (uMat.alpha == 1.0)
            color += uLColor * uMat.sColor * angleNH;
      }

   // Ambient lighting
   color += uLColor * uMat.aColor;

   //color = (uLColor * uMat.dColor * angleNL) + (uLColor * uMat.sColor * angleNH) + (uLColor * uMat.aColor);   

   gl_FragColor = vec4(color, uMat.alpha);
   //gl_FragColor = vec4(norm, 1.0);
   //gl_FragColor = vec4(depth, depth, depth, 1.0);
   //gl_FragColor = vec4(dist, dist, dist, 1.0);
   }
}
