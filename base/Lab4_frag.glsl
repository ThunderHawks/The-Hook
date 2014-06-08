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
uniform sampler2D uTexSampler;
uniform Material uMat;
uniform int uShadeMode;

varying vec3 vNorm; 
varying vec3 vPos;
varying vec4 vShadowPos;
varying vec2 vUV;

varying vec2 vTexCoord;
uniform float uGuiMode;
uniform float uMatMode;
uniform float uTextMode;

float Gaussian (float x) {
   float stdDev = 5.0;
	return (1.0 / sqrt(2.0 * 3.141592 * stdDev)) * exp(-((x * x) / (2.0 * stdDev * stdDev)));	
}

void main() {
   float angleNL, angleNH, depth, dist;
   vec4 shadowPos;
   vec3 color, halfVec;

   vec3 norm, pos, light, view;

   vec4 texColor1 = texture2D(uTexUnit, vTexCoord);

   if(uGuiMode == 1.0) {
      //Almost completely black
      if(uTextMode == 1.0) {
         if(texColor1[0] > 0.2 && texColor1[1] > 0.2 && texColor1[2] > 0.2) {
            discard;
         }
         else {
            gl_FragColor = vec4(texColor1[0], texColor1[1], texColor1[2], 1);
         }
      }
      //Only pure white is gone
      if(uTextMode == 1.1) {
         if(texColor1[0] < 0.5 && texColor1[1] < 0.5 && texColor1[2] > 0.7) {
            discard;
         }
         else {
            gl_FragColor = vec4(texColor1[0], texColor1[1], texColor1[2], 1);
         }
      }
      else if(uMatMode == 1.0) {
         gl_FragColor = vec4(uMat.aColor.r, uMat.aColor.g, uMat.aColor.b, 1);      
      }
      else {
         gl_FragColor = vec4(texColor1[0], texColor1[1], texColor1[2], 1);
      }
   }
   else if (uTextMode == 2.0) {
      // Fill the bloom map, taking fog into account
      float fogDist = min(length(vPos - uCamPos), 350.0) / 350.0;
      gl_FragColor = mix(vec4(uMat.aColor, 1.0), vec4(0.0, 0.0, 0.0, 0.0), pow(fogDist, 5.0));
   }
   else if (uTextMode == 3.0) {
      // Blur the bloom map horizontally
      float blurSize = 1.0/200.0;
      vec4 blur = vec4(0.0);
      vec4 clear = vec4(1.0, 1.0, 1.0, 0.0);
      // If the fragment is in the glowing light, use the light color
      if (texture2D(uTexUnit, vTexCoord).a != 0.0) {
         blur = texture2D(uTexUnit, vTexCoord);
         blur = vec4(blur.rgb, blur.a * 0.75);
      }     
      // Otherwise, blur the bloom map    
      else {
         float count, negRange, posRange;
         vec4 temp;
         bool onLeft = false, onRight = false;

         negRange = -5.0;
         posRange = 5.0;

         // Fixing edge cases so that the blur doesn't appear on the other side of the screen
         if (vTexCoord.x + negRange*blurSize < 0.0)
            negRange = -vTexCoord.x / blurSize;
         else if (vTexCoord.x + posRange*blurSize > 1.0)
            posRange = (1.0 - vTexCoord.x) / blurSize;

         // Take the weighted Gaussian distribution
         for (count = negRange; count <= posRange; count++) {
            temp = texture2D(uTexUnit, vec2(vTexCoord.x + count*blurSize, vTexCoord.y));
            if (count < 0.0 && temp != clear)
               onLeft = true;
            else if (count > 0.0 && temp != clear)
               onRight = true;
            if (temp != clear)   // Comment this out to have white glow
               blur += temp * Gaussian(count);
         }
         if (onLeft && onRight)
            blur = vec4(0.0);
      }
      gl_FragColor = blur;
   }
   else if (uTextMode == 5.0) {
      // Add bloom light to the scene
      gl_FragColor += texColor1;
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
      depth = texture2D(uTexUnit, shadowPos.xy).z; // Shadow map depth
      dist = vShadowPos.z - 0.005; // Distance from light to fragment

      // Diffuse lighting
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

      // Apply fog
      float fogDistance = min(length(vPos - uCamPos), 290.0) / 290.0;
      if (uMat.aColor == vec3(0.0, 0.0, 0.0) && uMat.dColor == vec3(0.0, 0.0, 0.0) && uMat.sColor == vec3(0.0, 0.0, 0.0))
         color = mix(vec3(0.0, 0.0, 0.0), vec3(0.7, 0.8, 0.9), pow(fogDistance, 5.0));
      else
         color = mix(color, vec3(0.7, 0.8, 0.9), pow(fogDistance, 5.0));

      vec3 texColor = texture2D(uTexSampler, vUV).rgb;

      gl_FragColor = vec4(color, uMat.alpha);
     //gl_FragColor *= vec4(texColor, 1.0);
     //gl_FragColor = vec4(norm, 1.0);
      //gl_FragColor = vec4(depth, depth, depth, 1.0);
      //gl_FragColor = vec4(dist, dist, dist, 1.0);
   }
}
