
#version 150

uniform vec2 pos0;

in  vec2 in_Position;
in  vec3 in_Color;
 
out vec3 ex_Color;

void main(void) {
 
	gl_Position = vec4(  in_Position + pos0,  0.0,  1.0 );

    ex_Color = in_Color;
};
