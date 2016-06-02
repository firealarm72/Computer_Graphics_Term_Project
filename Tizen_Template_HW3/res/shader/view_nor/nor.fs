precision mediump float;

uniform sampler2D s_tex0;
uniform sampler2D s_texNor;

uniform vec3 materialSpec, materialAmbi, materialEmit;
uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;

varying vec2 v_texCoord;
varying vec3 v_lightWS, v_viewWS;
varying mat3 worldToTangentSpace;
varying mat3 tangentToWorldSpace;

struct Material{
	float sh;	//shiness
	vec3 diff, spec, ambi, emit;	//material colors : diffuse, specular, ambient, emission
};

struct Light{
	vec3 dir, diff, spec, ambi;	//light and colors : diffuse, specular, smbient
};

vec3 phongLight(vec3 view, vec3 normal, Material M, Light S){
	float diff = max(dot(normal, S.dir), 0.0);
	vec3 refl = 2.0 * normal * dot(normal, S.dir) - S.dir;
	float spec = (diff > 0.0)? pow(max(dot(refl, view), 0.0), M.sh) : 0.0;
	//diffuse reflection
	vec3 sum = vec3(0.0);
	sum += diff * S.diff * M.diff;
	sum += spec * S.spec * M.spec;
	sum += S.ambi * M.ambi + M.emit;
	return sum;
}



void main() {
   // you should fill in this function
   vec3 materialDiff = texture2D(s_tex0, v_texCoord).xyz;
   vec3 norTS =tangentToWorldSpace * normalize(texture2D(s_texNor, v_texCoord).xyz * 2.0 - vec3(1.0));//convert normal vector in world space into tangent space
   
   Material material = Material(materialSh, materialDiff, materialSpec, materialAmbi, materialEmit);
   Light source = Light(normalize(v_lightWS), sourceDiff,sourceSpec, sourceAmbi);
   
   vec3 color =  phongLight(normalize(v_viewWS), norTS, material, source);//phong light equation
   gl_FragColor = vec4(color, 1.0);
}
