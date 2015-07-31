
///////////////////////////////////////
//    Geometry Primitives and Shapes
///////////////////////////////////////

void drawBox( float x0, float x1, float y0, float y1, float z0, float z1, float r, float g, float b ){
	glBegin(GL_QUADS);
		glColor3f( r, g, b );		          	     
		glVertex3f( x0, y0, z0 ); glVertex3f( x1, y0, z0 ); glVertex3f( x1, y1, z0 ); glVertex3f( x0, y1, z0 ); 
		glVertex3f( x0, y0, z0 ); glVertex3f( x1, y0, z0 ); glVertex3f( x1, y0, z1 ); glVertex3f( x0, y0, z1 ); 
		glVertex3f( x0, y0, z0 ); glVertex3f( x0, y1, z0 ); glVertex3f( x0, y1, z1 ); glVertex3f( x0, y0, z1 );	
		glVertex3f( x1, y1, z1 ); glVertex3f( x0, y1, z1 ); glVertex3f( x0, y0, z1 ); glVertex3f( x1, y0, z1 ); 
		glVertex3f( x1, y1, z1 ); glVertex3f( x0, y1, z1 ); glVertex3f( x0, y1, z0 ); glVertex3f( x1, y1, z0 ); 
		glVertex3f( x1, y1, z1 ); glVertex3f( x1, y0, z1 ); glVertex3f( x1, y0, z0 ); glVertex3f( x1, y1, z0 );		
	glEnd();
};

void drawAxis( float sc ){
	glDisable (GL_LIGHTING);
	glBegin   (GL_LINES);	          	     
		glColor3f( 1, 0, 0 ); glVertex3f( 0, 0, 0 ); glVertex3f( 1*sc, 0, 0 );
		glColor3f( 0, 1, 0 ); glVertex3f( 0, 0, 0 ); glVertex3f( 0, 1*sc, 0 );
		glColor3f( 0, 0, 1 ); glVertex3f( 0, 0, 0 ); glVertex3f( 0, 0, 1*sc );	
	glEnd();
};

int makeBoxList( float x0, float x1, float y0, float y1, float z0, float z1, float r, float g, float b  ){
	int ilist=glGenLists(1);
	glNewList( ilist, GL_COMPILE );
	drawBox( x0, x1, y0, y1, z0, z1, r, g, b );
	glEndList();
	return( ilist );
	// don't forget use glDeleteLists( ilist ,1); later 
}




#include <iostream>
#include <fstream>
#include <string>

int loadObjToList( bool loadNormals, int maxn, char* filename ){
	using namespace std;
	Vec3f* vs  = new  Vec3f[maxn];
	Vec3f* vns;
	if( loadNormals ) vns = new Vec3f[4*maxn]; 
	int ilist = glGenLists(1);
	glNewList( ilist, GL_COMPILE );
	glColor3f( 0.9f, 0.9f, 0.9f );	
	//drawBox( -1, 1, -1, 1, -1, 1, 0.5, 0.5, 0.5 );
	int  x0 = -1,x1 = +1,y0 = -1,y1 = +1,z0 = -1,z1 = +1;
	ifstream myfile (filename);
	if ( myfile.is_open() )	{
		int istart,iend;
		string line;
		int iv  = 0;
		int ivn = 0;
		while ( getline (myfile,line) )		{
			// reading faces
			if( line.compare(0,2,"v ")==0 ){
				string word;
				istart   = line.find(" ",0     ); 
				iend     = line.find(" ",istart+1);
				word     = line.substr(istart,iend-istart);
				vs[iv].x = stof( word );
				istart   = iend;
				iend     = line.find(" ",istart+1);
				word     = line.substr(istart,iend-istart);
				vs[iv].y = stof( word );
				word     = line.substr(iend+1);
				vs[iv].z = stof( word );
				//printf( " _%f_%f_%f_ ",vs[3*iv],vs[3*iv+1],vs[3*iv+2] ); printf( line.c_str() ); printf( "\n" );
				iv++;
			} else
			// reading normals
			if( loadNormals && ( line.compare(0,2,"vn")==0 ) ){
				string word;
				istart    = line.find(" ",0     ); 
				iend      = line.find(" ",istart+1);
				word      = line.substr(istart,iend-istart);
				vns[ivn].x = stof( word );
				istart    = iend;
				iend      = line.find(" ",istart+1);
				word      = line.substr(istart,iend-istart);
				vns[ivn].y = stof( word );
				word      = line.substr(iend+1);
				vns[ivn].z = stof( word );
				//printf( " _%f_%f_%f_ ",vns[ivn].x,vns[ivn].y,vns[ivn].z ); printf( line.c_str() ); printf( "\n" );
				ivn++;
			} else
			// reading faces
			if( line.compare(0,2,"f ")==0 ){
				int fciv[4];
				int fcin[4];
				int nv=0;
				istart = line.find(" ",0 );
				iend   = line.find(" ",istart+1);
				while( iend != string::npos ){
					int islash  = line.find("/",istart+1);
					string word = line.substr(istart,islash-istart);
					fciv[nv]    = stoi( word );
					if (loadNormals){
						string word   = line.substr(islash+2,iend-islash-1);
						fcin[nv]      = stoi( word );
						//cout << word << "\n";
					}
					nv++;
					istart = iend;
					iend = line.find(" ",istart+1);
				}
				//cout << line << "\n";
				if (loadNormals){
				//if (false){
					glEnable (GL_LIGHTING);
					if      ( nv == 3 ){ glBegin(GL_TRIANGLES); }
					else if ( nv == 4 ){ glBegin(GL_QUADS);     };
					for ( int j=0; j<nv; j++ ){ 
						int i  = fciv[j]-1;
						int in = fcin[j]-1;
						glNormal3f(vns[in].x,vns[in].y,vns[in].z); 
						glVertex3f( vs[i ].x, vs[i ].y, vs[i ].z );   
					}
					glEnd();
/*
					glDisable (GL_LIGHTING);
					for ( int j=0; j<nv; j++ ){ 
						int i  = fciv[j]-1;
						int in = fcin[j]-1;
						glBegin(GL_LINES);
							glVertex3f( vs[i ].x, vs[i ].y, vs[i ].z );  
							glVertex3f( vs[i ].x + vns[in].x, vs[i ].y + vns[in].y, vs[i ].z + vns[in].z );
						glEnd();
					}
*/
				}else{ // compute normals

					int   i0 = fciv[0]-1; int   i1 = fciv[1]-1; int   i2 = fciv[2]-1;
					Vec3f a       = vs[i0]-vs[i1];
					Vec3f b       = vs[i2]-vs[i1];
					Vec3f normal  = a.cross( b );
					normal       *= -1/sqrt(normal.mag2()); 
					glEnable (GL_LIGHTING);
					if      ( nv == 3 ){ glBegin(GL_TRIANGLES); }
					else if ( nv == 4 ){ glBegin(GL_QUADS); };
					for ( int j=0; j<nv; j++ ){ 
						int i = fciv[j]-1; 
						glNormal3f(normal.x,normal.y,normal.z); 
						glVertex3f( vs[i].x, vs[i].y, vs[i].z );   
					}
					glEnd();
/*
					int i = fciv[1]-1;
					glDisable (GL_LIGHTING);
					glBegin(GL_LINES);
						Vec3f center = vs[i1] + (a + b)*0.5f;
						glVertex3f( center.x           , center.y           , center.z            );
						glVertex3f( center.x + normal.x, center.y + normal.y, center.z + normal.z );
					glEnd();
*/
				}
				//printf( " %i %f %f %f %f \n", i, nx,ny,nz, nx*nx+ny*ny+nz*nz );
				//cout << nv <<"           " << line << "\n";
				//break;
			}
		}
	myfile.close();
	}else cout << "Unable to open file"; 
	delete [] vs;
	if( loadNormals ) delete [] vns; 
	//delete [] fcs;
	glEndList();
	return ilist;
}


/*
//   this version does not need class Vec3f

int loadObjToList( int maxn, char* filename ){
	using namespace std;
	float* vs = new float[3*maxn]; 
	//Vec3f* vs = new  Vec3f[maxn]; 
	int ilist = glGenLists(1);
	glNewList( ilist, GL_COMPILE );
	drawBox( -1, 1, -1, 1, -1, 1, 0.5, 0.5, 0.5 );
	int  x0 = -1,x1 = +1,y0 = -1,y1 = +1,z0 = -1,z1 = +1;
	ifstream myfile (filename);
	if ( myfile.is_open() )	{
		int istart,iend;
		string line;
		int iv = 0;
		while ( getline (myfile,line) )		{
			// reading faces
			if( line.compare(0,2,"v ")==0 ){
				string word;
				istart = line.find(" ",0     ); 
				iend   = line.find(" ",istart+1);
				word   = line.substr(istart,iend-istart);
				vs[ 3*iv     ] = stof( word );
				istart = iend;
				iend   = line.find(" ",istart+1);
				word   = line.substr(istart,iend-istart);
				vs[ 3*iv + 1 ] = stof( word );
				word   = line.substr(iend+1);
				vs[ 3*iv + 2 ] = stof( word );
				//printf( " _%f_%f_%f_ ",vs[3*iv],vs[3*iv+1],vs[3*iv+2] ); printf( line.c_str() ); printf( "\n" );
				iv++;
			} else
			// reading faces
			if( line.compare(0,2,"f ")==0 ){
				int fciv[4];
				int nv=0;
				istart = line.find(" ",0 );
				iend   = line.find(" ",istart+1);
				while( iend != string::npos ){
					int islash = line.find("/",istart+1);
					string word   = line.substr(istart,islash-istart);
					//cout << " >" << word << "< \n" ;
					fciv[nv] = stoi( word );
					nv++;
					istart = iend;
					iend = line.find(" ",istart+1);
				}
				int   i0 = fciv[0]-1; int   i1 = fciv[1]-1; int   i2 = fciv[2]-1;
				//Vec3f a; a.set( vs[3*i0  ] - vs[3*i1  ],1,1);

				float ax = vs[3*i0  ] - vs[3*i1  ];
				float ay = vs[3*i0+1] - vs[3*i1+1];
				float az = vs[3*i0+2] - vs[3*i1+2];
				float bx = vs[3*i2  ] - vs[3*i1  ];
				float by = vs[3*i2+1] - vs[3*i1+1];
				float bz = vs[3*i2+2] - vs[3*i1+2];
				float nx = ay * bz - az * by;
				float ny = az * bx - ax * bz;
				float nz = ax * by - ay * bx;
				float ir = -1/sqrt( nx*nx + ny*ny + nz*nz );
				nx*=ir; ny*=ir; nz*=ir;
				glEnable (GL_LIGHTING);
				if      ( nv == 3 ){ glBegin(GL_TRIANGLES); }
				else if ( nv == 4 ){ glBegin(GL_QUADS); };
				for ( int j=0; j<nv; j++ ){ int i = fciv[j]-1;  glNormal3f(nx,ny,nz); glVertex3f( vs[3*i], vs[3*i+1], vs[3*i+2] );   }
				glEnd();
				int i = fciv[1]-1;
				glDisable (GL_LIGHTING);
				glBegin(GL_LINES);
					glVertex3f( vs[3*i] + 0.5*(ax+bx)     , vs[3*i+1] + 0.5*(ay+by)     , vs[3*i+2] + 0.5*(az+bz)      );
					glVertex3f( vs[3*i] + 0.5*(ax+bx) + nx, vs[3*i+1] + 0.5*(ay+by) + ny, vs[3*i+2] + 0.5*(az+bz) + nz );
				glEnd();
				printf( " %i %f %f %f %f \n", i, nx,ny,nz, nx*nx+ny*ny+nz*nz );
				//cout << nv <<"           " << line << "\n";
				//break;
			}
		}
	myfile.close();
	}else cout << "Unable to open file"; 
	delete [] vs;
	//delete [] fcs;
	glEndList();
	return ilist;
}
*/

