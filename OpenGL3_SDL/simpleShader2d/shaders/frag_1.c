
#version 150

precision highp float;
 
in  vec3 ex_Color;
out vec4 gl_FragColor;
 
void main(void) {

    gl_FragColor = vec4(ex_Color,1.0);
	//gl_FragColor = vec4( sin(gl_FragCoord.x*0.1), sin(gl_FragCoord.y*0.1), sin(gl_FragCoord.z*0.1),1.0); 
};


