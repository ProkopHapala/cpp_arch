

//template <class TYPE, class VEC, class MAT> 
//template <class TYPE, class VEC> 
template <class TYPE> 
class Mat3TYPE{
	using VEC = Vec3TYPE<TYPE>; 
	using MAT = Mat3TYPE<TYPE>; 
	public:
	union{
		struct{
			TYPE xx,xy,xz;
			TYPE yx,yy,yz;
			TYPE zx,zy,zz;
		};
		struct{
			TYPE ax,ay,az;
			TYPE bx,by,bz;
			TYPE cx,cy,cz;
		};
		struct{	VEC a,b,c; };
		TYPE array[9];
	};


// ====== initialization

	inline void setOne(        ){ ax=by=cz=1; ay=az=bx=bz=cx=cy=0; };
	inline void set   ( TYPE f ){ ax=by=cz=f; ay=az=bx=bz=cx=cy=0; };

	inline void set  ( const VEC& va, const VEC& vb, const VEC& vc ){ a.set(va); b.set(vb); c.set(vc); }
	inline void set  ( const MAT& M ){
		ax=M.ax; ay=M.ay; az=M.az;
		bx=M.bx; by=M.by; bz=M.bz;
		cx=M.cx; cy=M.cy; cz=M.cz;
	};

	inline void set    ( TYPE * v )       { ax=v[0]; ay=v[1]; az=v[2];  bx=v[3]; by=v[4]; bz=v[5];   cx=v[6]; cy=v[7]; cz=v[8]; };
	inline void toArray( TYPE * v ) const { v[0]=ax; v[1]=ay; v[2]=az;  v[3]=bx; v[4]=by; v[5]=bz;   v[6]=cx; v[7]=cy; v[8]=cz; };

	inline void set_outer  ( const VEC& a, const VEC& b ){
		ax=a.x*b.x; ay=a.x*b.y; az=a.x*b.z;
		bx=a.y*b.x; by=a.y*b.y; bz=a.y*b.z;
		cx=a.z*b.x; cy=a.z*b.y; cz=a.z*b.z;
	};; 

	inline VEC getColx(){ VEC out; out.x = ax; out.y = bx; out.z = cx; return out; };
    inline VEC getColy(){ VEC out; out.x = ay; out.y = by; out.z = cy; return out; };
    inline VEC getColz(){ VEC out; out.x = az; out.y = bz; out.z = cz; return out; };

	inline void  colx_to( VEC& out){ out.x = ax; out.y = bx; out.z = cx; };
    inline void  coly_to( VEC& out){ out.x = ay; out.y = by; out.z = cy; };
    inline void  colz_to( VEC& out){ out.x = az; out.y = bz; out.z = cz; };

	inline void  setColx( const VEC v ){ ax = v.x; bx = v.y; cx = v.z; };
	inline void  setColy( const VEC v ){ ay = v.x; by = v.y; cy = v.z; };
	inline void  setColz( const VEC v ){ az = v.x; bz = v.y; cz = v.z; };

// ====== transpose

	inline void T(){
		TYPE t1=bx; bx=ay; ay=t1;  
		TYPE t2=cx; cx=az; az=t2;
		TYPE t3=cy; cy=bz; bz=t3;
	};

	inline void setT  ( const MAT& M ){
		ax=M.ax; ay=M.ay; az=M.az;
		bx=M.bx; by=M.by; bz=M.bz;
		cx=M.cx; cy=M.cy; cz=M.cz;
	};

	inline void setT  ( const VEC& va, const VEC& vb, const VEC& vc ){
		a.set( va.x, vb.x, vc.x ); 
		b.set( va.y, vb.y, vc.y ); 
		c.set( va.z, vb.z, vc.z );
	};

// ====== dot product with vector

	inline VEC dot( const VEC&  v ){
		VEC vout;
		vout.x = ax*v.x + ay*v.y + az*v.z;  
		vout.y = bx*v.x + by*v.y + bz*v.z;  
		vout.z = cx*v.x + cy*v.y + cz*v.z;  
		return vout;
	}

	inline void dot_to( const VEC&  v, VEC&  vout ){
		vout.x = ax*v.x + bx*v.y + cx*v.z;  
		vout.y = ay*v.x + by*v.y + cy*v.z;  
		vout.z = az*v.x + bz*v.y + cz*v.z;   
	};

	inline void dot_to_T( const VEC&  v, VEC&  vout ){
		vout.x = ax*v.x + ay*v.y + az*v.z;  
		vout.y = bx*v.x + by*v.y + bz*v.z;  
		vout.z = cx*v.x + cy*v.y + cz*v.z; 
	};

// ====== matrix multiplication

	inline void set_mmul( const MAT& A, const MAT& B ){
		ax = A.ax*B.ax + A.ay*B.bx + A.az*B.cx;  
		ay = A.ax*B.ay + A.ay*B.by + A.az*B.cy;  
		az = A.ax*B.az + A.ay*B.bz + A.az*B.cz;
		bx = A.bx*B.ax + A.by*B.bx + A.bz*B.cx;  
		by = A.bx*B.ay + A.by*B.by + A.bz*B.cy;  
		bz = A.bx*B.az + A.by*B.bz + A.bz*B.cz; 
		cx = A.cx*B.ax + A.cy*B.bx + A.cz*B.cx;  
		cy = A.cx*B.ay + A.cy*B.by + A.cz*B.cy;  
		cz = A.cx*B.az + A.cy*B.bz + A.cz*B.cz; 
	};

	inline void set_mmul_NT( const MAT& A, const MAT& B ){
		ax = A.ax*B.ax + A.ay*B.ay + A.az*B.az;  
		ay = A.ax*B.bx + A.ay*B.by + A.az*B.bz;  
		az = A.ax*B.cx + A.ay*B.cy + A.az*B.cz;
		bx = A.bx*B.ax + A.by*B.ay + A.bz*B.az;  
		by = A.bx*B.bx + A.by*B.by + A.bz*B.bz;  
		bz = A.bx*B.cx + A.by*B.cy + A.bz*B.cz; 
		cx = A.cx*B.ax + A.cy*B.ay + A.cz*B.az;  
		cy = A.cx*B.bx + A.cy*B.by + A.cz*B.bz;  
		cz = A.cx*B.cx + A.cy*B.cy + A.cz*B.cz; 
	};

	inline void set_mmul_TN( const MAT& A, const MAT& B ){
		ax = A.ax*B.ax + A.bx*B.bx + A.cx*B.cx;  
		ay = A.ax*B.ay + A.bx*B.by + A.cx*B.cy;  
		az = A.ax*B.az + A.bx*B.bz + A.cx*B.cz;
		bx = A.ay*B.ax + A.by*B.bx + A.cy*B.cx;  
		by = A.ay*B.ay + A.by*B.by + A.cy*B.cy;  
		bz = A.ay*B.az + A.by*B.bz + A.cy*B.cz; 
		cx = A.az*B.ax + A.bz*B.bx + A.cz*B.cx;  
		cy = A.az*B.ay + A.bz*B.by + A.cz*B.cy;  
		cz = A.az*B.az + A.bz*B.bz + A.cz*B.cz; 
	};

	inline void set_mmul_TT( const MAT& A, const MAT& B ){
		ax = A.ax*B.ax + A.bx*B.ay + A.cx*B.az;  
		ay = A.ax*B.bx + A.bx*B.by + A.cx*B.bz;  
		az = A.ax*B.cx + A.bx*B.cy + A.cx*B.cz;
		bx = A.ay*B.ax + A.by*B.ay + A.cy*B.az;  
		by = A.ay*B.bx + A.by*B.by + A.cy*B.bz;  
		bz = A.ay*B.cx + A.by*B.cy + A.cy*B.cz; 
		cx = A.az*B.ax + A.bz*B.ay + A.cz*B.az;  
		cy = A.az*B.bx + A.bz*B.by + A.cz*B.bz;  
		cz = A.az*B.cx + A.bz*B.cy + A.cz*B.cz; 
	};

// ====== matrix solver

   inline TYPE determinant() {
        TYPE fCoax = by * cz - bz * cy;
        TYPE fCobx = bz * cx - bx * cz;
        TYPE fCocx = bx * cy - by * cx;
        TYPE fDet = ax * fCoax + ay * fCobx + az * fCocx;
        return fDet;
    };

	inline void invert_to( MAT& Mout ) {
        TYPE idet = 1/determinant(); // we dont check det|M|=0
        Mout.ax = ( by * cz - bz * cy ) * idet;
        Mout.ay = ( az * cy - ay * cz ) * idet;
        Mout.az = ( ay * bz - az * by ) * idet;
        Mout.bx = ( bz * cx - bx * cz ) * idet;
        Mout.by = ( ax * cz - az * cx ) * idet;
        Mout.bz = ( az * bx - ax * bz ) * idet;
        Mout.cx = ( bx * cy - by * cx ) * idet;
        Mout.cy = ( ay * cx - ax * cy ) * idet;
        Mout.cz = ( ax * by - ay * bx ) * idet;
    };

    inline void adjoint_to( MAT& Mout ) {
        Mout.ax = by * cz - bz * cy;
        Mout.ay = az * cy - ay * cz;
        Mout.az = ay * bz - az * by;
        Mout.bx = bz * cx - bx * cz;
        Mout.by = ax * cz - az * cx;
        Mout.bz = az * bx - ax * bz;
        Mout.cx = bx * cy - by * cx;
        Mout.cy = ay * cx - ax * cy;
        Mout.cz = ax * by - ay * bx;
    };

// ======= Rotation

	inline void rotate( TYPE angle, const VEC& axis  ){
		VEC uaxis;
		uaxis.set( axis * axis.norm() );
		TYPE ca   = cos(angle);
		TYPE sa   = sin(angle);
 		rotate_csa( ca, sa, uaxis );
	};

	inline void rotate_csa( TYPE ca, TYPE sa, const VEC& uaxis ){
		a.rotate_csa( ca, sa, uaxis );
		b.rotate_csa( ca, sa, uaxis );
		c.rotate_csa( ca, sa, uaxis );
		//a.set(1);
		//b.set(2);
		//c.set(3);
	};
};

/*
class Mat3i : public Mat3TYPE< int   , Vec3i, Mat3i >{};
class Mat3f : public Mat3TYPE< float , Vec3f, Mat3f >{};
class Mat3d : public Mat3TYPE< double, Vec3d, Mat3d >{};
*/

using Mat3i = Mat3TYPE< int   >;
using Mat3f = Mat3TYPE< float >;
using Mat3d = Mat3TYPE< double>;



