


void glVertex( Vec3d v ){ glVertex3f( (float)v.x, (float)v.y, (float)v.z ); }
void glNormal( Vec3d v ){ glNormal3f( (float)v.x, (float)v.y, (float)v.z ); }
void glColor ( Vec3d v ){ glColor3f ( (float)v.x, (float)v.y, (float)v.z ); }

void glTranslate ( Vec3d v  ) { glTranslatef( (float)v.x, (float)v.y, (float)v.z ); };
void glScale     ( double f ) { glScalef    ( (float)f,   (float)f,   (float)f   );  };


/*
inline void toGLMat( const Vec3d& pos, const Mat3d& rot, float* glMat ){
	glMat[0 ] = (float)rot.xx;   glMat[1 ] = (float)rot.yx;   glMat[2 ] = (float)rot.zx;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.xy;   glMat[5 ] = (float)rot.yy;   glMat[6 ] = (float)rot.zy;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.xz;   glMat[9 ] = (float)rot.yz;   glMat[10] = (float)rot.zz;   glMat[11]  = 0;
	glMat[12] = (float)pos. x;   glMat[13] = (float)pos. y;   glMat[14] = (float)pos. z;   glMat[15]  = 1;
};
*/

inline void toGLMat( const Vec3d& pos, const Mat3d& rot, float* glMat ){
	glMat[0 ] = (float)rot.ax;   glMat[1 ] = (float)rot.ay;   glMat[2 ] = (float)rot.az;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.bx;   glMat[5 ] = (float)rot.by;   glMat[6 ] = (float)rot.bz;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.cx;   glMat[9 ] = (float)rot.cy;   glMat[10] = (float)rot.cz;   glMat[11]  = 0;
	glMat[12] = (float)pos. x;   glMat[13] = (float)pos. y;   glMat[14] = (float)pos. z;   glMat[15]  = 1;
};

inline void toGLMatCam( const Vec3d& pos, const Mat3d& rot, float* glMat ){
/*
	glMat[0 ] = (float)rot.ax;   glMat[1 ] = (float)rot.ay;   glMat[2 ] = (float)rot.az;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.bx;   glMat[5 ] = (float)rot.by;   glMat[6 ] = (float)rot.bz;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.cx;   glMat[9 ] = (float)rot.cy;   glMat[10] = (float)rot.cz;   glMat[11]  = 0;
*/
/*
	glMat[0 ] = (float)rot.ax;   glMat[1 ] = (float)rot.bx;   glMat[2 ] = (float)rot.cx;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.ay;   glMat[5 ] = (float)rot.by;   glMat[6 ] = (float)rot.cy;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.az;   glMat[9 ] = (float)rot.bz;   glMat[10] = (float)rot.cz;   glMat[11]  = 0;
*/
/*
	glMat[0 ] = (float)-rot.ax;   glMat[1 ] = (float)-rot.bx;   glMat[2 ] = (float)-rot.cx;   glMat[3 ]  = 0;
	glMat[4 ] = (float)-rot.ay;   glMat[5 ] = (float)-rot.by;   glMat[6 ] = (float)-rot.cy;   glMat[7 ]  = 0;
	glMat[8 ] = (float)-rot.az;   glMat[9 ] = (float)-rot.bz;   glMat[10] = (float)-rot.cz;   glMat[11]  = 0;
*/
	glMat[0 ] = (float)rot.ax;   glMat[1 ] = (float)rot.bx;   glMat[2 ] = (float)-rot.cx;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.ay;   glMat[5 ] = (float)rot.by;   glMat[6 ] = (float)-rot.cy;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.az;   glMat[9 ] = (float)rot.bz;   glMat[10] = (float)-rot.cz;   glMat[11]  = 0;
	glMat[12] = (float)-pos. x;   glMat[13] = (float)-pos. y;   glMat[14] = (float)-pos. z;   glMat[15]  = 1;
};



void drawPoint( const Vec3d& vec ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_POINTS);	          	     
		glVertex3d( vec.x, vec.y, vec.z );
	glEnd();
}



void drawVec( const Vec3d& vec ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glVertex3d( 0, 0, 0 ); glVertex3d( vec.x, vec.y, vec.z );
	glEnd();
}

void drawVecInPos( const Vec3d& v, const Vec3d& pos ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glVertex3d( pos.x, pos.y, pos.z ); glVertex3d( pos.x+v.x, pos.x+v.y, pos.x+v.z );
	glEnd();
}

void drawLine( const Vec3d& p1, const Vec3d& p2 ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glVertex3d( p1.x, p1.y, p1.z ); glVertex3d( p2.x, p2.y, p2.z );
	glEnd();
}

void drawTriangle( const Vec3d& p1, const Vec3d& p2, const Vec3d& p3 ){
	Vec3d d1,d2,normal;
	d1.set( p2 - p1 );
	d2.set( p3 - p1 );
	normal.set_cross(d1,d2);
	normal.normalize();
	glBegin   (GL_TRIANGLES);
		glNormal3d( normal.x, normal.y, normal.z );	          	     
		glVertex3d( p1.x, p1.y, p1.z ); 
		glVertex3d( p2.x, p2.y, p2.z );
		glVertex3d( p2.x, p2.y, p2.z );
	glEnd();
}

void drawMatInPos( const Mat3d& mat, const Vec3d& pos ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glColor3f( 1, 0, 0 ); glVertex3d( pos.x, pos.y, pos.z ); glVertex3d( pos.x+mat.xx, pos.y+mat.xy, pos.z+mat.xz );
		glColor3f( 0, 1, 0 ); glVertex3d( pos.x, pos.y, pos.z ); glVertex3d( pos.x+mat.yx, pos.y+mat.yy, pos.z+mat.yz );
		glColor3f( 0, 0, 1 ); glVertex3d( pos.x, pos.y, pos.z ); glVertex3d( pos.x+mat.zx, pos.y+mat.zy, pos.z+mat.zz );
	glEnd();
}


