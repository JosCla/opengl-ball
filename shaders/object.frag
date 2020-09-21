#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform float time;
uniform bool doEmission;

struct Material {
  sampler2D texture_diffuse0;
  sampler2D texture_specular0;
  float shininess;
  sampler2D emission;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  bool isOn;
};

struct DirLight {
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

	bool isOn;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

	float constant;
	float linear;
	float quadratic;

	bool isOn;
};

#define NUM_POINT_LIGHTS 20
#define NUM_DIR_LIGHTS 20
#define NUM_SPOT_LIGHTS 20

uniform Material material;
uniform PointLight ptlight[NUM_POINT_LIGHTS];
uniform DirLight dirlight[NUM_DIR_LIGHTS];
uniform SpotLight spotlight[NUM_SPOT_LIGHTS];

// defining some functions beforehand so that main can go first
vec3 processPointLight(PointLight, vec3, vec3, vec3);
vec3 processDirLight(DirLight, vec3, vec3, vec3);
vec3 processSpotLight(SpotLight, vec3, vec3, vec3);

void main()
{
  // initial calculations
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  // getting point light component
  vec3 ptLightComp = vec3(0.0);
  for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
    ptLightComp += processPointLight(ptlight[i], norm, FragPos, viewDir);
  }

  // getting directional light compoment
	vec3 dirLightComp = vec3(0.0);
	for (int i = 0; i < NUM_DIR_LIGHTS; ++i) {
		dirLightComp += processDirLight(dirlight[i], norm, FragPos, viewDir);
	}

  // getting spotlight component
	vec3 spotLightComp = vec3(0.0);
	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) {
		spotLightComp += processSpotLight(spotlight[i], norm, FragPos, viewDir);
	}

  // adding emission maps as needed
  vec3 emission = vec3(0.0);
  if (doEmission)
  {
    if (texture(material.texture_specular0, TexCoords).r == 0.0)
    {
      float emissionBrightness = texture(material.emission, vec2(TexCoords.x, TexCoords.y + time)).g;
      emission = vec3(0.5, 0.1, 1.0) * emissionBrightness;
    }
  }

  // combining all of the vectors into one result vector
  vec3 result = ptLightComp + dirLightComp + spotLightComp + emission;
  FragColor = vec4(result, 1.0);
}

vec3 processPointLight(PointLight ptlight, vec3 norm, vec3 fragPos, vec3 viewDir)
{
  if (ptlight.isOn)
  {
    // getting ambient lighting ready
    vec3 ambient = ptlight.ambient * vec3(texture(material.texture_diffuse0, TexCoords));

    // getting diffuse lighting ready
    vec3 lightDir = normalize(ptlight.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = ptlight.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));

    // getting specular lighting ready
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = ptlight.specular * spec * vec3(texture(material.texture_specular0, TexCoords));

    // getting attenuation ready
    float distance = length(ptlight.position - fragPos);
    float attenuation = 1.0 / (ptlight.constant + (ptlight.linear * distance) + (ptlight.quadratic * distance * distance));

    // combining all of the vectors into one result vector
    vec3 result = attenuation * (ambient + diffuse + specular);
    return result;
  }
  else
  {
    return vec3(0.0);
  }
}

vec3 processDirLight(DirLight dirlight, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	if (dirlight.isOn)
	{
    // getting ambient lighting ready
    vec3 ambient = dirlight.ambient * vec3(texture(material.texture_diffuse0, TexCoords));

    // getting diffuse lighting ready
    vec3 lightDir = normalize(-dirlight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirlight.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));

    // getting specular lighting ready
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirlight.specular * spec * vec3(texture(material.texture_specular0, TexCoords));

    // combining all of the vectors into one result vector
    vec3 result = ambient + diffuse + specular;
    return result;
	}
	else
	{
		return vec3(0.0);
	}
}

vec3 processSpotLight(SpotLight spotlight, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	if (spotlight.isOn)
	{
    // getting ambient lighting ready
    vec3 ambient = spotlight.ambient * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
		float attenuation = 1.0;

    vec3 lightDir = normalize(spotlight.position - fragPos);

    // if the fragment is outside of the cutOff radius, we can just stop here
    float theta = dot(lightDir, normalize(-spotlight.direction));
    if (theta > spotlight.outerCutOff)
    {
      // doing some more calculations, to make light dim at edges
      float epsilon = spotlight.cutOff - spotlight.outerCutOff;
      float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

      // getting diffuse lighting ready
      float diff = max(dot(norm, lightDir), 0.0);
      diffuse = intensity * spotlight.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));

      // getting specular lighting ready
      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      specular = intensity * spotlight.specular * spec * vec3(texture(material.texture_specular0, TexCoords));

      // getting attenuation ready
			float distance = length(spotlight.position - fragPos);
			attenuation = 1.0 / (spotlight.constant + (spotlight.linear * distance) + (spotlight.quadratic * distance * distance));
    }

    // combining all of the vectors into one result vector
    vec3 result = attenuation * (ambient + diffuse + specular);
    return result;
	}
	else
	{
		return vec3(0.0);
	}
}
