
#version 150

uniform int  natoms;
uniform vec3 atoms[6];
uniform vec3 coefs[6];

precision highp float;

out vec4 gl_FragColor;

// ========= parameters

const float lRadial =  4.0;
const float kRadial = -1.0;

const vec3 kSpring = vec3( -0.03, -0.03,  0.0 );
const vec3 rPP0    = vec3(  0,        0, -4.0 );

const float ffMax   = 10.0;
const float ffConv  = 0.0000001;

const float finc     = 1.1; 
const float fdec     = 0.5;
const float falpha   = 0.99;
const float dtMax    = 0.1; 
const float dampMax  = 0.1;

// ======== force fields

vec3 getForceRadial( vec3 dR ){
	float l   = sqrt( dot(dR,dR) );
	return dR*kRadial*( l - lRadial )/l;
}
 
vec3 getForceLJQ( vec3 dR, vec3 C ){

	float ir2 = 1/dot( dR, dR );
	float ir6 = ir2*ir2*ir2;
	//        C6         C12            k*Qi*Qj
	float f = C[0]*ir6 + C[1]*ir6*ir6 + C[2]*sqrt(ir2);
	return dR*f*ir2;

//	float f = 0.01/(0.01+dot( dR, dR ));
//	return vec3( f, f, f );
};

vec3 getSurfForces( vec3 rPP ){
	vec3 F = vec3( 0.0, 0.0, 0.0 );
	for(int i=0; i<natoms; i++){
		F += getForceLJQ( rPP - atoms[i] , coefs[i] );
	}
	return F;
}

// ======== relaxation

vec3 relax( vec3 pos, vec3 rTip ){
	vec3 vel = vec3( 0, 0, 0 );
	float dt    = dtMax;
	float damp  = dampMax;
	for( int i=0; i<1000; i++ ){
		// evaluate force
		vec3 force  = getSurfForces( pos );
		vec3 dpos = pos - rTip;
		force += getForceRadial( dpos );
		force += kSpring*( dpos - rPP0 );
		// move FIRE 
		float vf = dot( vel,   force );
		float vv = dot( vel,   vel   );
	    float ff = dot( force, force );
		if( vf < 0.0 ){
			vel   = vec3( 0.0, 0.0, 0.0 );
			dt    = dt * fdec;
		  	damp  = dampMax;
		}else{
			float cf  =       damp * sqrt( vv/ff );
			float cv  = 1.0 - damp;
			vel = cv * vel  + cf * force;
			dt     = min( dt * finc, dtMax );
			damp  *= falpha;
		}
		if( ff > ffMax ) force *= ffMax/sqrt(ff);  // 
		vel += force * dt;
		pos += vel   * dt;	
		if( ff < ffConv ) break;
	} 

	return pos;
}

// ======== main

void main(void) {
	vec3 rTip = vec3( vec2(-2.5,-2.5)+(gl_FragCoord.xy/50), 6.8 );
	vec3 rPP   = relax( rTip + rPP0, rTip );
	vec3 force = getSurfForces( rPP );

	const float dz = 0.1;
	vec3 rPP2   = relax( rPP, rTip+vec3( 0, 0, -dz ) );
	vec3 force2 = getSurfForces( rPP2 );
	//vec3 force = getSurfForces( rTip );
	float df = 0.1*16.0*(force2.z - force.z)/dz;
	gl_FragColor = vec4( df+0.5, df+0.5, df+0.5, 1.0 );

	//gl_FragColor = vec4( force, 1.0 );
//	gl_FragColor = vec4( vec3(0.5,0.5,0.5) + force.zzz*10, 1.0 );

//	gl_FragColor = vec4( vec3(0.5,0.5,0.5) + (rPP-rTip), 1.0 );

};


