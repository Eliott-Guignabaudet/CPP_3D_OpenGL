#version 330 core 
out vec4 FragColor;
in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;

uniform sampler2D Texture;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambiantStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
   float specularStrength = 0.5;

   vec3 ambiant = ambiantStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec - lightColor;


   ivec2 texSize = textureSize(Texture, 0);
   if (texSize.x != 1 && texSize.y != 1) {
      vec4 textureColor = texture(Texture, TexCoord);
      FragColor = vec4(objectColor * textureColor.rgb * (ambiant + diffuse + specular), 1.0f);
   }else {
      FragColor = vec4(objectColor * (ambiant + diffuse + specular), 1.0f);
   }

};