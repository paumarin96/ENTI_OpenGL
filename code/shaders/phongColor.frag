#version 330

in vec3 vert_Normal;
in vec3 fragPos;
in vec2 fragmentUV;
uniform vec3 viewPos;
uniform vec3 lightDir;

out vec4 color; 
uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec4 objectColor;

//Material properties
uniform float ambientStrength;
uniform float specularStrength;
uniform int specularIntensity;

uniform sampler2D diffuseTexture;
uniform bool hasTexture;

struct DirectionalLight
{
	vec3 direction;

	vec4 color;
	vec4 ambientColor;
   float ambientStrength;

};


struct PointLight
{
   bool isActive;
   vec3 position;
   float strength; 

   vec4 color;
   vec4 ambientColor;
   float ambientStrength;

   float constant;
   float linear;
   float quadratic;

};

struct SpotLight
{
    bool isActive; 
	vec3 position;
	vec3 direction;
    float strength;
    float cutoff;
    float outerCutoff;

	vec4 color;
	vec4 ambientColor;
    float ambientStrength;



};


uniform DirectionalLight directionalLight;
uniform PointLight pointLight[16];
uniform SpotLight spotLight[16];

vec4 CalculateDirectionalLight(DirectionalLight light){

    //Ambient
    vec4 ambient = light.ambientStrength * light.ambientColor;

    //Diffuse
    vec3 norm = normalize(vert_Normal);
    vec3 lightDir2 = normalize(light.direction);
     
    float diff = max(dot(norm, lightDir2), 0.0);
    vec4 diffuse =  diff * light.color;

    //Specular 
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir2, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularIntensity);
    vec4 specular = specularStrength * spec * light.color;  

   return specular + ambient + diffuse;
}

vec4 CalculatePointLight(PointLight light){

    vec3 norm = normalize(vert_Normal);
    vec3 lightDirection = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation =  1.0 / (1.0 + (0.09 * distance) +  0.032 * (distance * distance));

    //Ambient
     vec4 ambient =  attenuation * light.ambientStrength * light.ambientColor;

    //Diffuse
    float diff = max(dot(norm, lightDirection), 0.0);
    vec4 diffuse = attenuation * diff * light.color * light.strength;

    //Specular 
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularIntensity);
    vec4 specular = attenuation * specularStrength * spec * light.color;  

   return specular + diffuse;

}

vec4 CalculateSpotLight(SpotLight light){

    vec3 norm = normalize(vert_Normal);

    float distance = length(light.position - fragPos);
    vec3 lightDirection = normalize(light.position - fragPos);
    
    float theta = dot(lightDirection, normalize(-light.direction));


    //Ambient
    vec4 ambient =  light.ambientStrength * light.ambientColor;


    if(theta > light.outerCutoff){
    
        float epsilon = light.cutoff - light.outerCutoff;
        float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); 


        //Diffuse
        float diff = max(dot(norm, lightDirection), 0.0);
        vec4 diffuse = intensity * diff * light.color * light.strength;

        //Specular 
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDirection, norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularIntensity);
        vec4 specular = intensity * specularStrength * spec * light.color; 
      
        return specular + diffuse;

    }

 
    return vec4(0,0,0,1);

}


void main(){
    
    if(gl_FragCoord.x%2){
       discard;
    }

   vec4 directionalLightResult = CalculateDirectionalLight(directionalLight);

   vec4 pointLightResult; 
   for(int i = 0; i < 16; i++){
      if(pointLight[i].isActive){
         pointLightResult += CalculatePointLight(pointLight[i]);
      }

   }
  
    vec4 spotLightResult;
     for(int i = 0; i < 16; i++){
      if(spotLight[i].isActive){
         spotLightResult += CalculateSpotLight(spotLight[i]);
      }

   }

   if(hasTexture){
      vec4 textureColor = texture(diffuseTexture, frag_UV);
      color =  textureColor * (spotLightResult + pointLightResult + directionalLightResult) * objectColor;
      return;
    }


   //
    //Final Result
    color =   (spotLightResult + pointLightResult + directionalLightResult) * objectColor;



}
