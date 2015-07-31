
#version 150

uniform vec2 blobs[5];

precision highp float;
 
in  vec3 ex_Color;
out vec4 gl_FragColor;
 

float blobFunc( vec2 p, vec2 p0 ){
	vec2 dp = p - p0;
	float r2 = dot(dp,dp);
	return 1/( 1 + 0.01*r2 );
};


void main(void) {

	vec2 p = gl_FragCoord.xy;

	float f = 0;
	f += blobFunc( p, blobs[0] );
	f += blobFunc( p, blobs[1] );
	f += blobFunc( p, blobs[2] );
	f += blobFunc( p, blobs[3] );
	f += blobFunc( p, blobs[4] );

	gl_FragColor = vec4( f,f,f,1.0 );;

    //gl_FragColor = vec4(ex_Color,1.0);
	//gl_FragColor = vec4( sin(gl_FragCoord.x*0.1), sin(gl_FragCoord.y*0.1), sin(gl_FragCoord.z*0.1),1.0); 

};


