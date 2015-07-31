



class Molecule{
	int natoms,nbonds;
	Vec3d  * atoms;
	int    * bonds;
	Uint32 * colors;
	double * radii;

	int viewlist=0;


	Molecule( int natoms_, Vec3d * atoms_, double * radii_, int * colors_ ){
		natoms = natoms_; atoms  = atoms_; radii  = radii_; colors = colors_;
	}

	Molecule( int natoms_ ){
		natoms = natoms_;
		atoms  = new Vec3d [ natoms ];
		radii  = new double[ natoms ];
		colors = new Uint32[ natoms ];
	}

	int findBonds( double sc ){
		int nbonds = 0;
		int * bonds_ = new int[ 2*natoms*natoms ];
		for (int i=0; i<natoms; i++ ){
			for (int j=0; j<i; j++ ){
				double rijmax = sc * ( radii[i] + radii[j] );
				Vec3d d; d.set_sub( atoms[i], atoms[j] );
				double rij2 = d.norm2();
				if( rij2 < rijmax*rijmax ){ bonds_[nbonds]=i; bonds_[nbonds]=j; nbonds+=2; }
			}
		}
		if ( bonds != NULL ){ delete bonds; }
		bonds = new  int[ nbonds ];
		for (int i=0; i<nbonds; i++ ){  bonds[i] = bonds_[i]; };
		delete bonds_;
		return nbonds;
	}

	int makeViewCPK ( int nsphere, int nstick, float atomscale, float bondwidth ){
		if( viewlist > 0 ) {	glDeleteLists( viewlist, 1 );	}
		int nvert = 0;
		int viewlist = glGenLists(1);
		glNewList( viewlist , GL_COMPILE );
/*
			for( int i=0; i<natoms; i++ ){
				setColorInt32( colors[ i ] );
				nvert += drawSphere_oct( nsphere, atomscale*radii[i], atoms[i] );
			}
*/
			glColor3f( 0.2f, 0.2f, 0.2f );
			for( int ib=0; ib<nbonds; ib+=2 ){
				Vec3f ai,aj;
				convert( atoms[bonds[ib  ]], ai ); 
				convert( atoms[bonds[ib+1]], aj );
				nvert += drawCylinderStrip( 4, bondwidth, bondwidth, ai, aj );
			}
		glEndList();
		printf( " nvert %i \n", nvert );
		return viewlist;
	}

};
