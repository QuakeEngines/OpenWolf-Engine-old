/*[Vertex]*/
attribute vec3 attr_Position;
attribute vec4 attr_TexCoord0;

uniform mat4   u_ModelViewProjectionMatrix;

uniform vec4	u_Local0; // vibrancy, 0, 0, 0

varying vec2   var_TexCoords;
varying vec4   var_Local0; // vibrancy, 0, 0, 0

void main()
{
	gl_Position = u_ModelViewProjectionMatrix * vec4(attr_Position, 1.0);
	var_TexCoords = attr_TexCoord0.st;
	var_Local0 = u_Local0;
}

/*[Fragment]*/
uniform sampler2D	u_TextureMap;

varying vec2		var_TexCoords;
varying vec4		var_Local0; // vibrancy, 0, 0, 0

vec2	texCoord = var_TexCoords;
float	Vibrance = var_Local0.x;

vec4 PS_ProcessVibrance()
{
	vec4	res;
	vec3	origcolor = texture2D(u_TextureMap, texCoord.xy).rgb;
	vec3	lumCoeff = vec3(0.212656, 0.715158, 0.072186);  				//Calculate luma with these values
	
	float	max_color = max(origcolor.r, max(origcolor.g,origcolor.b)); 	//Find the strongest color
	float	min_color = min(origcolor.r, min(origcolor.g,origcolor.b)); 	//Find the weakest color

	float	color_saturation = max_color - min_color; 						//Saturation is the difference between min and max

	float	luma = dot(lumCoeff, origcolor.rgb); 							//Calculate luma (grey)

	res.rgb = mix(vec3(luma), origcolor.rgb, (1.0 + (Vibrance * (1.0 - (sign(Vibrance) * color_saturation))))); 	//Extrapolate between luma and original by 1 + (1-saturation) - current
	
	res.w=1.0;
	return res;
}

void main()
{
	gl_FragColor.rgb = PS_ProcessVibrance().rgb;
	gl_FragColor.a = 1.0;
}