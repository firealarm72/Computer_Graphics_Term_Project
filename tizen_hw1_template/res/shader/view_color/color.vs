uniform mat4 worldMat, viewMat, projMat;

// 'attribute' and 'varying' are qualifiers which is compatible with OpenGL ES 2.0
// These do the same for 'in' and 'out' keywords.
// You don't need to modify them.
attribute vec3 position;
attribute  vec3 normal;

varying vec3 v_normal;

void main() {
	//you should fill in this function
    gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);
    v_normal = normal;
}
