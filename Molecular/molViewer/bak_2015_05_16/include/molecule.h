
const  int ntypes_def       = 3; 
static Uint32 colors_def[ntypes_def] ={ 0xFFF0F0F0, 0xFF101010, 0xFFFF0000 }; 
static double radii_def [ntypes_def] ={ 1.4,        1.8,        1.7        };

int ntypes=ntypes_def;
Uint32 * colors; 
double * radii;


class Molecule{
	public:
	int natoms,nbonds=0;
	int    * types = NULL;
	Vec3d  * xyzs  = NULL;
	int    * bonds = NULL;
	int viewlist=0;

	Molecule( ){
		bonds = NULL; viewlist=0; nbonds=0;
	};

	Molecule( int natoms_, int * types_, Vec3d * xyzs_ ){
		natoms = natoms_; types = types_; xyzs  = xyzs_;
		bonds = NULL; viewlist=0; nbonds=0;
	}

	Molecule( int natoms_ ){
		natoms = natoms_;
		types  = new   int[ natoms ];
		xyzs   = new Vec3d[ natoms ];
		bonds = NULL; viewlist=0; nbonds=0;
	}

	int findBonds( double sc ){
		//printf( " 0 \n" );
		nbonds = 0;
		int * bonds_ = new int[ 2*natoms*natoms ];
		//printf( " 1 \n" );
		for (int i=0; i<natoms; i++ ){
			for (int j=i+1; j<natoms; j++ ){
				//printf( " i %i j %i \n", i, j );
				double rijmax = sc * ( radii[types[i]] + radii[types[j]] );
				Vec3d d; d.set_sub( xyzs[i], xyzs[j] );
				double rij2 = d.norm2();
				//printf( " %i %i %i %i %f %f   %f %f \n", i, j, types[i], types[j], rij2, ( rijmax*rijmax ), radii[types[i]], radii[types[j]] );
				if( rij2 < ( rijmax*rijmax ) ){ 
					bonds_[nbonds]=i; bonds_[nbonds+1]=j; 
					//printf( " %i %i %i \n",  nbonds,  bonds_[nbonds  ], bonds_[nbonds+1] );
					nbonds+=2; 
				}
			}
		}
		printf( " %i \n", bonds );
		if ( bonds != NULL ){ delete bonds; printf( " delete bonds \n"); }
		bonds = new  int[ nbonds ];
		for (int i=0; i<nbonds; i++ ){  bonds[i] = bonds_[i]; };
		delete bonds_;
		return nbonds;
	}

	int makeViewCPK ( int nsphere, int nstick, float atomscale, float bondwidth ){
		if( viewlist > 0 ) {	glDeleteLists( viewlist, 1 );	}
		int nvert = 0;
		viewlist = glGenLists(1);
		glNewList( viewlist , GL_COMPILE );
			glShadeModel ( GL_SMOOTH );
			//Vec3d pos; pos.set(0,0,0);
			//nvert += drawSphere_oct( nsphere, 2.0, pos );
			for( int i=0; i<natoms; i++ ){
				setColorInt32( colors[ types[i] ] );
				//glColor3f( 0.8f, 0.8f, 0.8f );
				nvert += drawSphere_oct( nsphere, atomscale*radii[types[i]], xyzs[i] );
			}

			glColor3f( 0.2f, 0.2f, 0.2f );
			for( int ib=0; ib<nbonds; ib+=2 ){
				Vec3f ai,aj;
				convert( xyzs[bonds[ib  ]], ai ); 
				convert( xyzs[bonds[ib+1]], aj );
				//printf( " %i %i   %i %i    %f %f %f    %f %f %f \n", ib, nbonds, bonds[ib  ], bonds[ib+1], ai.x, ai.y, ai.z, aj.x, aj.y, aj.z );
				nvert += drawCylinderStrip( nstick, bondwidth, bondwidth, ai, aj );
			}
		glEndList();
		printf( " nvert %i \n", nvert );
		return viewlist;
	}

	bool loadFromFile_bas( char const* filename ){
		printf(" filename: %s \n", filename );
		FILE * pFile;
		pFile = fopen (filename,"r");
  		fscanf (pFile, "%i", &natoms);
		printf("natoms %i \n", natoms );
		types  = new   int[ natoms ];
		xyzs   = new Vec3d[ natoms ];
		for (int i=0; i<natoms; i++){
  			fscanf (pFile, "%i %lf %lf %lf", &types[i], &xyzs[i].x, &xyzs[i].y, &xyzs[i].z );
			types[i]--;
			//printf(" %i %f %f %f \n", types[i], xyzs[i].x, xyzs[i].y, xyzs[i].z );
		}
  		fclose (pFile);
  		return 0;
	}

	void center_minmax(){
		Vec3d pmin,pmax; pmin.set( 100000,100000,100000 );  pmax.set( -100000,-100000,-100000 );
		for (int i=0; i<natoms; i++){
			pmin.x = fmin( pmin.x, xyzs[i].x  ); pmin.y = fmin( pmin.y, xyzs[i].y ); pmin.z = fmin( pmin.z, xyzs[i].z );
			pmax.x = fmax( pmax.x, xyzs[i].x  ); pmax.y = fmax( pmax.y, xyzs[i].y ); pmax.z = fmax( pmax.z, xyzs[i].z );
		}
		pmin.add(pmax); pmin.mul(0.5);
		for (int i=0; i<natoms; i++){
			xyzs[i].sub( pmin ); 
		}
	}







/*
	bool loadFromFile_bas( int maxn, char* filename ){
		ifstream myfile (filename);
		if ( myfile.is_open() )	{
			int istart,iend;
			string line;
			getline (myfile,line);
			natoms = stoi(line);
			types  = new   int[ natoms ];
			xyzs   = new Vec3d[ natoms ];
			int iv  = 0;
			int ivn = 0;
			for ( int i=0;  i++ )		{
				getline (myfile,line);
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
			}
			myfile.close();
			return true;
		}else{
			 printf( "Unable to open file \n" );
			return false;
		}
	}
*/


};
