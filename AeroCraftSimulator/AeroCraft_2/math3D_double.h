

// read also:
// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/


//#include <math.h>
//#include <stdio.h>


class Vec3d;
class Mat3d;
class Quat4d;

/////////////////////////////
//  fast math functions
/////////////////////////////

inline double sin_taylor2( double a ){
	const double c3 = 1.0d/6;        
	const double c5 = 1.0d/120;
	double a2 = a*a;
	return    a * ( 1 - a2*( c3 - c5*a2 ) ); 
}

inline double cos_taylor2( double a ){
	const double c2 = 1.0d/2;        
	const double c4 = 1.0d/24;  
	double a2 = a*a;
	return    1 - a2*( c2 - c4*a2 );
}

inline void sincos_taylor2( double a, double& sa, double& ca ){
	const double c2 = 1.0d/2;       
	const double c3 = 1.0d/6;      
	const double c4 = 1.0d/24;     
	const double c5 = 1.0d/120; 
	double a2 = a*a;
	sa   = a * ( 1 - a2*( c3 - c5*a2 ) ) ; 
	ca   =       1 - a2*( c2 - c4*a2 )   ;
}

/////////////////////////
//   CLASS :   Vec3d
//////////////////////////

class Vec3d{
	public:
	union{
		struct{ double x,y,z; };
		struct{ double a,b,c; };
		double array[3];
	};

	inline void set( double  f                       ) { x=f;   y=f;   z=f;   };
    inline void set( double fx, double fy, double fz ) { x=fx;  y=fy;  z=fz;  };
    inline void set( const Vec3d& v                  ) { x=v.x; y=v.y; z=v.z; };

	// do not use assignement operator because it is not obious what is ment
	// also there is conflict with Mat3d: member ‘Vec3d Mat3d::<anonymous union>::<anonymous struct>::a’ with copy assignment operator not allowed in anonymous aggregate
	//inline Vec3d& operator =( const double f ) { x=f; y=f; z=f;       return *this; }; 
	//inline Vec3d& operator =( const Vec3d& v ) { x=v.x; y=v.y; z=v.z; return *this; }; 

    inline Vec3d& operator+=( double f ) { x+=f; y+=f; z+=f; return *this; };
    inline Vec3d& operator*=( double f ) { x*=f; y*=f; z*=f; return *this; };

    inline Vec3d& operator+=( const Vec3d&  v ) { x+=v.x; y+=v.y; z+=v.z; return *this; };
    inline Vec3d& operator-=( const Vec3d&  v ) { x-=v.x; y-=v.y; z-=v.z; return *this; };
    inline Vec3d& operator*=( const Vec3d&  v ) { x*=v.x; y*=v.y; z*=v.z; return *this; };
    inline Vec3d& operator/=( const Vec3d&  v ) { x/=v.x; y/=v.y; z/=v.z; return *this; };

	// This creates a new vectors? is it good?
    inline Vec3d operator+ ( double f   ) const { Vec3d vo; vo.x=x+f; vo.y=y+f; vo.z=z+f; return vo; };
    inline Vec3d operator* ( double f   ) const { Vec3d vo; vo.x=x*f; vo.y=y*f; vo.z=z*f; return vo; };

    inline Vec3d operator+ ( const Vec3d& vi ) const { Vec3d vo; vo.x=x+vi.x; vo.y=y+vi.y; vo.z=z+vi.z; return vo; };
    inline Vec3d operator- ( const Vec3d& vi ) const { Vec3d vo; vo.x=x-vi.x; vo.y=y-vi.y; vo.z=z-vi.z; return vo; };
    inline Vec3d operator* ( const Vec3d& vi ) const { Vec3d vo; vo.x=x*vi.x; vo.y=y*vi.y; vo.z=z*vi.z; return vo; };
    inline Vec3d operator/ ( const Vec3d& vi ) const { Vec3d vo; vo.x=x/vi.x; vo.y=y/vi.y; vo.z=z/vi.z; return vo; };

	inline void mul   ( const Vec3d& b                 ){ x*=b.x; y*=b.y; z*=b.z;          };
	inline void setMul( const Vec3d& a, const Vec3d& b ){ x=a.x*b.x; y=a.y*b.y; z=a.z*b.z; };

    inline void fma   ( double       f, const Vec3d& b ){ x+=  f*b.x; y+=  f*b.y; z+=  f*b.z; };

	inline double dot  ( const Vec3d& a ) const { return        x*a.x + y*a.y + z*a.z  ; };
	inline double norm2( ) const { return        x*x + y*y + z*z  ; };
	inline double norm ( ) const { return  sqrt( x*x + y*y + z*z ); };
    inline double normalize() {
		double norm  = sqrt( x*x + y*y + z*z );
		double inorm = 1.0d/norm;
		x *= inorm;    y *= inorm;    z *= inorm;
		return norm;
    }


	inline Vec3d cross    ( const Vec3d& a                 ){ Vec3d vo; vo.x=y*a.z-z*a.y; vo.y=z*a.x-x*a.z; vo.z=x*a.y-y*a.x; return vo; };
    inline void  set_cross( const Vec3d& a, const Vec3d& b ){ x =a.y*b.z-a.z*b.y; y =a.z*b.x-a.x*b.z; z =a.x*b.y-a.y*b.x; };
	inline void  add_cross( const Vec3d& a, const Vec3d& b ){ x+=a.y*b.z-a.z*b.y; y+=a.z*b.x-a.x*b.z; z+=a.x*b.y-a.y*b.x;   };


	// Rodrigues rotation formula: v' = cosa*v + sina*(uaxis X v) + (1-cosa)*(uaxis . v)*uaxis
	inline void rotate( double angle, const Vec3d& axis  ){
		Vec3d uaxis;
		uaxis.set( axis * axis.norm() );
		double ca   = cos(angle);
		double sa   = sin(angle);
 		rotate_csa( ca, sa, uaxis );
	};

	inline void rotate_csa( double ca, double sa, const Vec3d& uaxis ){

		double cu = (1-ca)*dot(uaxis);
		double utx  = uaxis.y*z - uaxis.z*y; 
		double uty  = uaxis.z*x - uaxis.x*z; 
		double utz  = uaxis.x*y - uaxis.y*x;
		double x_ = ca*x + sa*utx + cu*uaxis.x;
		double y_ = ca*y + sa*uty + cu*uaxis.y;
		       z  = ca*z + sa*utz + cu*uaxis.z;
		x = x_; y = y_; 
	};

};

/*
inline double dot  ( const Vec3d& a, const Vect& b ){ return       a.x*b.x + a.y*b.y + a.z*b.z  ; }
inline double norm2( const Vec3d& a                ){ return       a.x*a.x + a.y*a.y + a.z*a.z  ; }
inline double norm ( const Vec3d& a                ){ return sqrt( a.x*a.x + a.y*a.y + a.z*a.z ); }

inline Vec3d cross( const Vec3d& a, const Vec3d& a             ){ Vec3d vo; vo.x=a.y*b.z-a.z*b.y; vo.y=a,z*b.x-a.x*b.z; vo.z=a.x*b.y-a.y*b.x; return vo; };
inline void  cross( const Vec3d& a, const Vec3d& a, Vec3d& vo  ){           vo.x=a.y*b.z-a.z*b.y; vo.y=a,z*b.x-a.x*b.z; vo.z=a.x*b.y-a.y*b.x; return vo; };
*/

/////////////////////////
//   CLASS :   Mat3d
//////////////////////////

class Mat3d{
	public:
	union{
		struct{
			double xx,xy,xz;
			double yx,yy,yz;
			double zx,zy,zz;
		};
		struct{
			double ax,ay,az;
			double bx,by,bz;
			double cx,cy,cz;
		};
		struct{	Vec3d a,b,c; };
		double array[9];
	};


// ====== initialization

	inline void setOne(          ){ ax=by=cz=1; ay=az=bx=bz=cx=cy=0; };
	inline void set   ( double f ){ ax=by=cz=f; ay=az=bx=bz=cx=cy=0; };

	inline void set  ( const Vec3d& va, const Vec3d& vb, const Vec3d& vc ){ a.set(va); b.set(vb); c.set(vc); }
	inline void set  ( const Mat3d& M ){
		ax=M.ax; ay=M.ay; az=M.az;
		bx=M.bx; by=M.by; bz=M.bz;
		cx=M.cx; cy=M.cy; cz=M.cz;
	};

	inline void set_outer  ( const Vec3d& a, const Vec3d& b ){
		ax=a.x*b.x; ay=a.x*b.y; az=a.x*b.z;
		bx=a.y*b.x; by=a.y*b.y; bz=a.y*b.z;
		cx=a.z*b.x; cy=a.z*b.y; cz=a.z*b.z;
	};; 

	inline Vec3d getColx(){ Vec3d out; out.x = ax; out.y = bx; out.z = cx; return out; };
    inline Vec3d getColy(){ Vec3d out; out.x = ay; out.y = by; out.z = cy; return out; };
    inline Vec3d getColz(){ Vec3d out; out.x = az; out.y = bz; out.z = cz; return out; };

	inline void  colx_to( Vec3d& out){ out.x = ax; out.y = bx; out.z = cx; };
    inline void  coly_to( Vec3d& out){ out.x = ay; out.y = by; out.z = cy; };
    inline void  colz_to( Vec3d& out){ out.x = az; out.y = bz; out.z = cz; };

	inline void  setColx( const Vec3d v ){ ax = v.x; bx = v.y; cx = v.z; };
	inline void  setColy( const Vec3d v ){ ay = v.x; by = v.y; cy = v.z; };
	inline void  setColz( const Vec3d v ){ az = v.x; bz = v.y; cz = v.z; };

	// Don't need this, because we use union: use representation a,b,c
	//inline Vec3d getRowx(){ Vec3d out; out.x = ax; out.y = ay; out.z = az; return out; };
	//inline Vec3d getRowy(){ Vec3d out; out.x = bx; out.y = by; out.z = bz; return out; };
	//inline Vec3d getRowz(){ Vec3d out; out.x = cx; out.y = cy; out.z = cz; return out; };
	//inline void rowx_to( Vec3d& out ){ out.x = ax; out.y = ay; out.z = az; };
	//inline void rowy_to( Vec3d& out ){ out.x = bx; out.y = by; out.z = bz; };
	//inline void rowz_to( Vec3d& out ){ out.x = cx; out.y = cy; out.z = cz; };
	//inline void  setRowx( const Vec3d& v ){ ax = v.x; ay = v.y; az = v.z; };
	//inline void  setRowy( const Vec3d& v ){ bx = v.x; by = v.y; bz = v.z; };
	//inline void  setRowz( const Vec3d& v ){ cx = v.x; cy = v.y; cz = v.z; };

// ====== transpose

	inline void T(){
		double t1=bx; bx=ay; ay=t1;  
		double t2=cx; cx=az; az=t2;
		double t3=cy; cy=bz; bz=t3;
	};

	inline void setT  ( const Mat3d& M ){
		ax=M.ax; ay=M.ay; az=M.az;
		bx=M.bx; by=M.by; bz=M.bz;
		cx=M.cx; cy=M.cy; cz=M.cz;
	};

	inline void setT  ( const Vec3d& va, const Vec3d& vb, const Vec3d& vc ){
		a.set( va.x, vb.x, vc.x ); 
		b.set( va.y, vb.y, vc.y ); 
		c.set( va.z, vb.z, vc.z );
	};

// ====== dot product with vector

	inline Vec3d dot( const Vec3d&  v ){
		Vec3d vout;
		vout.x = ax*v.x + ay*v.y + az*v.z;  
		vout.y = bx*v.x + by*v.y + bz*v.z;  
		vout.z = cx*v.x + cy*v.y + cz*v.z;  
		return vout;
	}

	inline void dot_to( const Vec3d&  v, Vec3d&  vout ){
		vout.x = ax*v.x + bx*v.y + cx*v.z;  
		vout.y = ay*v.x + by*v.y + cy*v.z;  
		vout.z = az*v.x + bz*v.y + cz*v.z;   
	};

	inline void dot_to_T( const Vec3d&  v, Vec3d&  vout ){
		vout.x = ax*v.x + ay*v.y + az*v.z;  
		vout.y = bx*v.x + by*v.y + bz*v.z;  
		vout.z = cx*v.x + cy*v.y + cz*v.z; 
	};

// ====== matrix multiplication

	inline void set_mmul( const Mat3d& A, const Mat3d& B ){
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

	inline void set_mmul_NT( const Mat3d& A, const Mat3d& B ){
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

	inline void set_mmul_TN( const Mat3d& A, const Mat3d& B ){
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

	inline void set_mmul_TT( const Mat3d& A, const Mat3d& B ){
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

   inline double determinant() {
        double fCoax = by * cz - bz * cy;
        double fCobx = bz * cx - bx * cz;
        double fCocx = bx * cy - by * cx;
        double fDet = ax * fCoax + ay * fCobx + az * fCocx;
        return fDet;
    };

	inline void invert_to( Mat3d& Mout ) {
        double idet = 1/determinant(); // we dont check det|M|=0
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

    inline void adjoint_to( Mat3d& Mout ) {
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

	inline void rotate( double angle, const Vec3d& axis  ){
		Vec3d uaxis;
		uaxis.set( axis * axis.norm() );
		double ca   = cos(angle);
		double sa   = sin(angle);
 		rotate_csa( ca, sa, uaxis );
	};

	inline void rotate_csa( double ca, double sa, const Vec3d& uaxis ){
		a.rotate_csa( ca, sa, uaxis );
		b.rotate_csa( ca, sa, uaxis );
		c.rotate_csa( ca, sa, uaxis );
		//a.set(1);
		//b.set(2);
		//c.set(3);
	};


};




///////////////////////////
//   CLASS :   Quaternion
///////////////////////////


class Quat4d {
	public:
	union{
		struct{ double x,y,z,w; };
		double array[4];
	};

	inline void set   ( const  Quat4d& q                           ){ x=q.x; y=q.y; z=q.z; w=q.w; };
	inline void set   ( double fx, double fy, double fz, double fw ){ x=fx;  y=fy;  z=fz;  w=fw; };
	inline void setOne( ){ x=y=z=0; w=1; };

// ============== Basic Math

    inline Quat4d operator*=( double f        ) { x*=f; y*=f; z*=f; z*=f; w*=f;   return *this; };
    inline Quat4d operator+=( const Quat4d& q ) { x+=q.x; y+=q.y; z+=q.z; w+=q.w; return *this; };
    inline Quat4d operator-=( const Quat4d& q ) { x-=q.x; y-=q.y; z-=q.z; w-=q.w; return *this; };

    inline double dot ( Quat4d q ) {  return       w*q.w + x*q.x + y*q.y + z*q.z;   }
    inline double norm2(         ) {  return       w*  w + x*  x + y*  y + z*  z;   }
	inline double norm (         ) {  return sqrt( w*  w + x*  x + y*  y + z*  z ); }
    inline double normalize() {
		double norm  = sqrt( x*x + y*y + z*z + w*w );
		double inorm = 1.0d/norm;
		x *= inorm;    y *= inorm;    z *= inorm;   w *= inorm;
		return norm;
    }

// ====== Quaternion multiplication

	inline void setQmul( const Quat4d& a, const Quat4d& b) {
        x =  a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x;
        y = -a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y;
        z =  a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z;
        w = -a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w;
    };

    inline void qmul( const Quat4d& a) {
        double x_ =  x * a.w + y * a.z - z * a.y + w * a.x;
        double y_ = -x * a.z + y * a.w + z * a.x + w * a.y;
        double z_ =  x * a.y - y * a.x + z * a.w + w * a.z;
                w = -x * a.x - y * a.y - z * a.z + w * a.w;
        x = x_; y = y_; z = z_;
    };

    inline void qmul2( const Quat4d& a) {
        double x_ =  a.x * w + a.y * z - a.z * y + a.w * x;
        double y_ = -a.x * z + a.y * w + a.z * x + a.w * y;
        double z_ =  a.x * y - a.y * x + a.z * w + a.w * z;
               w  = -a.x * x - a.y * y - a.z * z + a.w * w;
        x = x_; y = y_; z = z_;
    };

    inline void invertUnitary() { x=-x; y=-y; z=-z; }

    inline void invert() {
		double norm = sqrt( x*x + y*y + z*z + w*w );
		if ( norm > 0.0 ) {
			double invNorm = 1.0d / norm;
			x *= -invNorm; y *= -invNorm;z *= -invNorm;	w *=  invNorm;
		}
    };

// =======  pitch, yaw, roll

	inline void dpitch( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); pitch( ca, sa );  };
	inline void pitch ( double angle ){ pitch( cos(angle), sin(angle) );  };
    inline void pitch ( double ca, double sa ) {
        double x_ =  x * ca + w * sa;
        double y_ =  y * ca + z * sa;
        double z_ = -y * sa + z * ca;
                w = -x * sa + w * ca;
        x = x_; y = y_; z = z_;
    };

	inline void dyaw( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); yaw( ca, sa );  };
	inline void yaw ( double angle ){ yaw( cos(angle), sin(angle) );  };
    inline void yaw ( double ca, double sa ) {
        double x_ =  x * ca - z * sa;
        double y_ =  y * ca + w * sa;
        double z_ =  x * sa + z * ca;
                w = -y * sa + w * ca;
        x = x_; y = y_; z = z_;
    };

	inline void droll( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); roll( ca, sa );  };
	inline void roll ( double angle ){ roll( cos(angle), sin(angle) );  };
    inline void roll ( double ca, double sa ) {
        double x_ =  x * ca + y * sa;
        double y_ = -x * sa + y * ca;
        double z_ =  z * ca + w * sa;
                w = -z * sa + w * ca;
        x = x_; y = y_; z = z_;
    };


	inline void dpitch2( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); pitch2( ca, sa );  };
	inline void pitch2 ( double angle ){ pitch2( cos(angle), sin(angle) );  };
    inline void pitch2 ( double ca, double sa ) {
/*
        double x_ =  ax * w + ay * z - az * y + aw * x;
        double y_ = -ax * z + ay * w + az * x + aw * y;
        double z_ =  ax * y - ay * x + az * w + aw * z;
               w  = -ax * x - ay * y - az * z + aw * w;
*/
        double x_ =  sa * w + ca * x;
        double y_ = -sa * z + ca * y;
        double z_ =  sa * y + ca * z;
               w  = -sa * x + ca * w;
        x = x_; y = y_; z = z_;
    };

	inline void dyaw2( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); yaw2( ca, sa );  };
	inline void yaw2 ( double angle ){ yaw2( cos(angle), sin(angle) );  };
    inline void yaw2 ( double ca, double sa ) {
        double x_ = + sa * z  + ca * x;
        double y_ = + sa * w  + ca * y;
        double z_ = - sa * x  + ca * z;
               w  = - sa * y  + ca * w;
        x = x_; y = y_; z = z_;
    };

	inline void droll2( double angle ){ double ca,sa; sincos_taylor2(angle,ca,sa); roll2( ca, sa );  };
	inline void roll2 ( double angle ){ roll2( cos(angle), sin(angle) );  };
    inline void roll2 ( double ca, double sa ) {
        double x_ = - sa * y + ca * x;
        double y_ = + sa * x + ca * y;
        double z_ = + sa * w + ca * z;
               w  = - sa * z + ca * w;
        x = x_; y = y_; z = z_;
    };

// ======= Conversion : Angle & Axis

	inline void fromAngleAxis( double angle, const Vec3d& axis ){  
		double ir   = 1/axis.norm();
		Vec3d  hat  = axis * ir;
		double a    = 0.5d * angle;
		double sa   = sin(a);
		w =           cos(a);
		x = sa * hat.x;
		y = sa * hat.y;
		z = sa * hat.z;
	};


// ====== Differential rotation

	inline void dRot_exact ( double dt, const Vec3d& omega ) {
		double hx   = omega.x;
		double hy   = omega.y;
		double hz   = omega.z;
		double r2   = hx*hx + hy*hy + hz*hz;
		if(r2>0){
			double norm = sqrt( r2 );
			double a    = dt * norm * 0.5d;
			double sa   = sin( a )/norm;  // we normalize it here to save multiplications
			double ca   = cos( a );
			hx*=sa; hy*=sa; hz*=sa;            // hat * sin(a)
			double x_ = x, y_ = y, z_ = z, w_ = w;
			x =  hx*w_ + hy*z_ - hz*y_ + ca*x_;
			y = -hx*z_ + hy*w_ + hz*x_ + ca*y_;
			z =  hx*y_ - hy*x_ + hz*w_ + ca*z_;
			w = -hx*x_ - hy*y_ - hz*z_ + ca*w_;
		}
	};


	inline void dRot_taylor2 ( double dt, Vec3d& omega ) {
		double hx   = omega.x;
		double hy   = omega.y;
		double hz   = omega.z;
		double r2   = hx*hx + hy*hy + hz*hz;
		double b2   = dt*dt*r2;
		const double c2 = 1.0d/8;    // 4  *2       
		const double c3 = 1.0d/48;   // 8  *2*3     
		const double c4 = 1.0d/384;  // 16 *2*3*4   
		const double c5 = 1.0d/3840; // 32 *2*3*4*5 
		double sa   = dt * ( 0.5d - b2*( c3 - c5*b2 ) ); 
		double ca   =      ( 1    - b2*( c2 - c4*b2 ) );
		hx*=sa; hy*=sa; hz*=sa;  // hat * sin(a)
		double x_ = x, y_ = y, z_ = z, w_ = w;
		x =  hx*w_ + hy*z_ - hz*y_ + ca*x_;
		y = -hx*z_ + hy*w_ + hz*x_ + ca*y_;
		z =  hx*y_ - hy*x_ + hz*w_ + ca*z_;
		w = -hx*x_ - hy*y_ - hz*z_ + ca*w_;
	};


	inline void toMatrix( Mat3d& result) {
	    double r2 = w*w + x*x + y*y + z*z;
	    //double s  = (r2 > 0) ? 2d / r2 : 0;
		double s  = 2 / r2;
	    // compute xs/ys/zs first to save 6 multiplications, since xs/ys/zs
	    // will be used 2-4 times each.
	    double xs = x * s;  double ys = y * s;  double zs = z * s;
	    double xx = x * xs; double xy = x * ys; double xz = x * zs;
	    double xw = w * xs; double yy = y * ys; double yz = y * zs;
	    double yw = w * ys; double zz = z * zs; double zw = w * zs;
	    // using s=2/norm (instead of 1/norm) saves 9 multiplications by 2 here
	    result.ax = 1 - (yy + zz);
	    result.bx =     (xy - zw);
	    result.cx =     (xz + yw);
	    result.ay =     (xy + zw);
	    result.by = 1 - (xx + zz);
	    result.cy =     (yz - xw);
	    result.az =     (xz - yw);
	    result.bz =     (yz + xw);
	    result.cz = 1 - (xx + yy);
	};


	inline void toMatrix_unitary( Mat3d& result) {
		double xx = 2*x * x;
		double xy = 2*y * y;
		double xz = 2*x * z;
		double xw = 2*x * w;
		double yy = 2*y * y;
		double yz = 2*y * z;
		double yw = 2*y * w;
		double zz = 2*z * z;
		double zw = 2*z * w;
		result.ax = 1 - (yy + zz);
		result.bx =     (xy - zw);
		result.cx =     (xz + yw);
		result.ay =     (xy + zw);
		result.by = 1 - (xx + zz);
		result.cy =     (yz - xw);
		result.az =     (xz - yw);
		result.bz =     (yz + xw);
		result.cz = 1 - (xx + yy);
	};



	inline void fromMatrix( const Vec3d& a, const Vec3d& b, const Vec3d& c ) { fromMatrix( a.x,  a.y,  a.z,  b.x,  b.y,  b.z,  c.x,  c.y,  c.z  );  }
	inline void fromMatrix( const Mat3d& M                                 ) { fromMatrix( M.ax, M.ay, M.az, M.bx, M.by, M.bz, M.cx, M.cy, M.cz );  }

    inline void fromMatrix( double max, double may, double maz, double mbx, double mby, double mbz, double mcx, double mcy, double mcz) {
		double t = max + mby + mcz;
		// we protect the division by s by ensuring that s>=1
		if (t >= 0) { // |w| >= .5
			double s = sqrt(t + 1); // |s|>=1 ...
			w = 0.5f * s;
			s = 0.5f / s;                 // so this division isn't bad
			x = (mcy - mbz) * s;
			y = (maz - mcx) * s;
			z = (mbx - may) * s;
		} else if ((max > mby) && (max > mcz)) {
			double s = sqrt(1.0f + max - mby - mcz); // |s|>=1
			x = s * 0.5f; // |x| >= .5
			s = 0.5f / s;
			y = (mbx + may) * s;
			z = (maz + mcx) * s;
			w = (mcy - mbz) * s;
		} else if (mby > mcz) {
			double s = sqrt(1.0f + mby - max - mcz); // |s|>=1
			y = s * 0.5f; // |y| >= .5
			s = 0.5f / s;
			x = (mbx + may) * s;
			z = (mcy + mbz) * s;
			w = (maz - mcx) * s;
		} else {
			double s = sqrt(1.0f + mcz - max - mby); // |s|>=1
			z = s * 0.5f; // |z| >= .5
			s = 0.5f / s;
			x = (maz + mcx) * s;
			y = (mcy + mbz) * s;
			w = (mbx - may) * s;
		}
	};

};



