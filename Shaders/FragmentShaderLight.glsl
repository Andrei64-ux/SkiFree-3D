#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

uniform int is_spot_light;
uniform float angle;

out vec4 out_color;

void main()
{
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect (-L, N));

	// TODO: define ambient light component, compute diffuse light componen, specular light component
	float ambient_light = 0.25 * material_kd;
	float diffuse_light = material_kd * max(dot(N, L), 0);
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	// TODO: compute light
	float light = ambient_light;
	float cut_off = angle;
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);
	float dist = distance(light_position, world_position);
	float factor_attenuation = 1 / (pow(dist, 2) + 1);
	
	if (is_spot_light == 1) {

		if (spot_light > spot_light_limit) {	 
			
			float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} 

	} else {
		light = ambient_light + factor_attenuation * (diffuse_light + specular_light);
	}


	// TODO: write pixel out color
	vec3 colour = object_color * light;

	out_color = vec4(colour, 1.f);
}