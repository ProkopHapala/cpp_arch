

class TrinagleMesh{
	public:
	int nvert;
	int ntri;
	Vec3d  * verts;
	Vec3d  * normals;
	double * areas;
	int    * tris;

	int viewlist=0;

	TrinagleMesh( int nvert_, int ntri_ ){
		nvert    = nvert_;
		ntri     = ntri_;
		verts    = new Vec3d [     nvert ];
		tris     = new int   [ 3 * ntri  ]; 
		normals  = new Vec3d [ 3 * ntri  ]; 
		areas    = new double[ 3 * ntri  ]; 
	}

	void evalNormals(){
		int it = 0;
		for( int i=0; i<ntri; i++){
			Vec3d a,b,c,ab,ac,normal;
			a.set( verts[ tris[ it ] ] ); it++;
			b.set( verts[ tris[ it ] ] ); it++;
			c.set( verts[ tris[ it ] ] ); it++;
			ab.set_sub( b, a );
			ac.set_sub( c, a );
			normal.set_cross( ab, ac );
			double area = normal.norm();
			areas[i]    = area;
			normal.mul( 1/area );
			normals[i]  = normal;
		}
	}

	void make_render( ){
		if( viewlist > 0 ) {	glDeleteLists( viewlist, 1 );	}
		viewlist = glGenLists(1);
		glNewList( viewlist , GL_COMPILE );
		glShadeModel ( GL_FLAT );
		glBegin( GL_TRIANGLES );
			int it = 0;
			for( int i=0; i<ntri; i++){
				Vec3f tmp;
				convert( normals[ i ],        tmp );       glNormal3f( tmp.x, tmp.y, tmp.z );
				convert( verts[ tris[ it ] ], tmp ); it++; glVertex3f( tmp.x, tmp.y, tmp.z );
				convert( verts[ tris[ it ] ], tmp ); it++; glVertex3f( tmp.x, tmp.y, tmp.z );
				convert( verts[ tris[ it ] ], tmp ); it++; glVertex3f( tmp.x, tmp.y, tmp.z );
			}
		glEnd( );
		glEndList();
	}

	double ray( 	const Vec3d &ray0, const Vec3d &hRay, Vec3d& p ){
		double tmin = 1e+300;
		int it = 0;
		for( int i=0; i<ntri; i++){
			Vec3d a,b,c,pt;
			a.set( verts[ tris[ it ] ] ); it++;
			b.set( verts[ tris[ it ] ] ); it++;
			c.set( verts[ tris[ it ] ] ); it++;
			bool inside;
			double t = rayTriangle(  ray0, hRay,  a,b,c,  inside, pt );
			if( inside & ( t>0 ) & ( t<tmin ) ){
					tmin = t; 
					p.set( pt );
			}
		}
	}
	

};
