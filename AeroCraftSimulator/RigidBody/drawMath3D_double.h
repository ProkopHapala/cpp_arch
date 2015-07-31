

/*
inline void toGLMat( const Vec3d& pos, const Mat3d& rot, float* glMat ){
	glMat[0 ] = (float)rot.xx;   glMat[1 ] = (float)rot.xy;   glMat[2 ] = (float)rot.xz;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.yx;   glMat[5 ] = (float)rot.yy;   glMat[6 ] = (float)rot.yz;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.zx;   glMat[9 ] = (float)rot.zy;   glMat[10] = (float)rot.zz;   glMat[11]  = 0;
	glMat[12] = (float)pos. x;   glMat[13] = (float)pos. y;   glMat[14] = (float)pos. z;   glMat[15]  = 1;
};

*/

inline void toGLMat( const Vec3d& pos, const Mat3d& rot, float* glMat ){
	glMat[0 ] = (float)rot.xx;   glMat[1 ] = (float)rot.yx;   glMat[2 ] = (float)rot.zx;   glMat[3 ]  = 0;
	glMat[4 ] = (float)rot.xy;   glMat[5 ] = (float)rot.yy;   glMat[6 ] = (float)rot.zy;   glMat[7 ]  = 0;
	glMat[8 ] = (float)rot.xz;   glMat[9 ] = (float)rot.yz;   glMat[10] = (float)rot.zz;   glMat[11]  = 0;
	glMat[12] = (float)pos. x;   glMat[13] = (float)pos. y;   glMat[14] = (float)pos. z;   glMat[15]  = 1;
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

void drawMat( const Mat3d& mat ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( mat.xx, mat.xy, mat.xz );
		glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( mat.yx, mat.yy, mat.yz );
		glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( mat.zx, mat.zy, mat.zz );
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


