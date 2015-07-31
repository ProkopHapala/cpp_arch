

// read also:
// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/


//#include <math.h>
//#include <stdio.h>

/////////////////////////
//   CLASS :   Vec3d
//////////////////////////

class Vec3d{
	public:
	union{
		struct{ double x,y,z; };
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

	inline double norm2( ) const { return        x*x + y*y + z*z  ; };
	inline double norm ( ) const { return  sqrt( x*x + y*y + z*z ); };
    inline double normalize() {
		double norm  = sqrt( x*x + y*y + z*z );
		double inorm = 1.0d/norm;
		x *= inorm;    y *= inorm;    z *= inorm;
		return norm;
    }


//	inline Vec3d cross    ( const Vec3d& a                 ){ Vec3d vo; vo.x=y*a.z-z*a.y; vo.y=z*a.x-x*a.z; vo.z=x*a.y-y*a.x; return vo; };
    inline void  set_cross( const Vec3d& a, const Vec3d& b ){ x =a.y*b.z-a.z*b.y; y =a.z*b.x-a.x*b.z; z =a.x*b.y-a.y*b.x; };
	inline void  add_cross( const Vec3d& a, const Vec3d& b ){ x+=a.y*b.z-a.z*b.y; y+=a.z*b.x-a.x*b.z; z+=a.x*b.y-a.y*b.x;   };
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

	inline void setOne(          ){ xx=yy=zz=1; xy=xz=yx=yz=zx=zy=0; };
	inline void set   ( double f ){ xx=yy=zz=f; xy=xz=yx=yz=zx=zy=0; };

	inline void set  ( const Vec3d& va, const Vec3d& vb, const Vec3d& vc ){ a.set(va); b.set(vb); c.set(vc); }
	inline void set  ( const Mat3d& M ){
		xx=M.xx; xy=M.xy; xz=M.xz;
		yx=M.yx; yy=M.yy; yz=M.yz;
		zx=M.zx; zy=M.zy; zz=M.zz;
	};


	inline void set_outer  ( const Vec3d& a, const Vec3d& b ){
		xx=a.x*b.x; xy=a.x*b.y; xz=a.x*b.z;
		yx=a.y*b.x; yy=a.y*b.y; yz=a.y*b.z;
		zx=a.z*b.x; zy=a.z*b.y; zz=a.z*b.z;
	};; 

	inline Vec3d getColx(){ Vec3d out; out.x = xx; out.y = yx; out.z = zx; return out; };
    inline Vec3d getColy(){ Vec3d out; out.x = xy; out.y = yy; out.z = zy; return out; };
    inline Vec3d getColz(){ Vec3d out; out.x = xz; out.y = yz; out.z = zz; return out; };

	inline void  colx_to( Vec3d& out){ out.x = xx; out.y = yx; out.z = zx; };
    inline void  coly_to( Vec3d& out){ out.x = xy; out.y = yy; out.z = zy; };
    inline void  colz_to( Vec3d& out){ out.x = xz; out.y = yz; out.z = zz; };

	inline void  setColx( const Vec3d v ){ xx = v.x; yx = v.y; zx = v.z; };
	inline void  setColy( const Vec3d v ){ xy = v.x; yy = v.y; zy = v.z; };
	inline void  setColz( const Vec3d v ){ xz = v.x; yz = v.y; zz = v.z; };

	// Don't need this, because we use union: use representation a,b,c
	//inline Vec3d getRowx(){ Vec3d out; out.x = xx; out.y = xy; out.z = xz; return out; };
	//inline Vec3d getRowy(){ Vec3d out; out.x = yx; out.y = yy; out.z = yz; return out; };
	//inline Vec3d getRowz(){ Vec3d out; out.x = zx; out.y = zy; out.z = zz; return out; };
	//inline void rowx_to( Vec3d& out ){ out.x = xx; out.y = xy; out.z = xz; };
	//inline void rowy_to( Vec3d& out ){ out.x = yx; out.y = yy; out.z = yz; };
	//inline void rowz_to( Vec3d& out ){ out.x = zx; out.y = zy; out.z = zz; };
	//inline void  setRowx( const Vec3d& v ){ xx = v.x; xy = v.y; xz = v.z; };
	//inline void  setRowy( const Vec3d& v ){ yx = v.x; yy = v.y; yz = v.z; };
	//inline void  setRowz( const Vec3d& v ){ zx = v.x; zy = v.y; zz = v.z; };

// ====== transpose

	inline void T(){
		double t1=yx; yx=xy; xy=t1;  
		double t2=zx; zx=xz; xz=t2;
		double t3=zy; zy=yz; yz=t3;
	};

	inline void setT  ( const Mat3d& M ){
		xx=M.xx; xy=M.xy; xz=M.xz;
		yx=M.yx; yy=M.yy; yz=M.yz;
		zx=M.zx; zy=M.zy; zz=M.zz;
	};

	inline void setT  ( const Vec3d& va, const Vec3d& vb, const Vec3d& vc ){
		a.set( va.x, vb.x, vc.x ); 
		b.set( va.y, vb.y, vc.y ); 
		c.set( va.z, vb.z, vc.z );
	};

// ====== dot product with vector

	inline Vec3d dot( const Vec3d&  v ){
		Vec3d vout;
		vout.x = xx*v.x + xy*v.y + xz*v.z;  
		vout.y = yx*v.x + yy*v.y + yz*v.z;  
		vout.z = zx*v.x + zy*v.y + zz*v.z;  
		return vout;
	}

	inline void dot_to( const Vec3d&  v, Vec3d&  vout ){
		vout.x = xx*v.x + xy*v.y + xz*v.z;  
		vout.y = yx*v.x + yy*v.y + yz*v.z;  
		vout.z = zx*v.x + zy*v.y + zz*v.z;  
	};

	inline void dot_to_T( const Vec3d&  v, Vec3d&  vout ){
		vout.x = xx*v.x + yx*v.y + zx*v.z;  
		vout.y = xy*v.x + yy*v.y + zy*v.z;  
		vout.z = xz*v.x + yz*v.y + zz*v.z;  
	};

// ====== matrix multiplication

	inline void set_mmul( const Mat3d& A, const Mat3d& B ){
		xx = A.xx*B.xx + A.xy*B.yx + A.xz*B.zx;  
		xy = A.xx*B.xy + A.xy*B.yy + A.xz*B.zy;  
		xz = A.xx*B.xz + A.xy*B.yz + A.xz*B.zz;
		yx = A.yx*B.xx + A.yy*B.yx + A.yz*B.zx;  
		yy = A.yx*B.xy + A.yy*B.yy + A.yz*B.zy;  
		yz = A.yx*B.xz + A.yy*B.yz + A.yz*B.zz; 
		zx = A.zx*B.xx + A.zy*B.yx + A.zz*B.zx;  
		zy = A.zx*B.xy + A.zy*B.yy + A.zz*B.zy;  
		zz = A.zx*B.xz + A.zy*B.yz + A.zz*B.zz; 
	};

	inline void set_mmul_NT( const Mat3d& A, const Mat3d& B ){
		xx = A.xx*B.xx + A.xy*B.xy + A.xz*B.xz;  
		xy = A.xx*B.yx + A.xy*B.yy + A.xz*B.yz;  
		xz = A.xx*B.zx + A.xy*B.zy + A.xz*B.zz;
		yx = A.yx*B.xx + A.yy*B.xy + A.yz*B.xz;  
		yy = A.yx*B.yx + A.yy*B.yy + A.yz*B.yz;  
		yz = A.yx*B.zx + A.yy*B.zy + A.yz*B.zz; 
		zx = A.zx*B.xx + A.zy*B.xy + A.zz*B.xz;  
		zy = A.zx*B.yx + A.zy*B.yy + A.zz*B.yz;  
		zz = A.zx*B.zx + A.zy*B.zy + A.zz*B.zz; 
	};

	inline void set_mmul_TN( const Mat3d& A, const Mat3d& B ){
		xx = A.xx*B.xx + A.yx*B.yx + A.zx*B.zx;  
		xy = A.xx*B.xy + A.yx*B.yy + A.zx*B.zy;  
		xz = A.xx*B.xz + A.yx*B.yz + A.zx*B.zz;
		yx = A.xy*B.xx + A.yy*B.yx + A.zy*B.zx;  
		yy = A.xy*B.xy + A.yy*B.yy + A.zy*B.zy;  
		yz = A.xy*B.xz + A.yy*B.yz + A.zy*B.zz; 
		zx = A.xz*B.xx + A.yz*B.yx + A.zz*B.zx;  
		zy = A.xz*B.xy + A.yz*B.yy + A.zz*B.zy;  
		zz = A.xz*B.xz + A.yz*B.yz + A.zz*B.zz; 
	};

	inline void set_mmul_TT( const Mat3d& A, const Mat3d& B ){
		xx = A.xx*B.xx + A.yx*B.xy + A.zx*B.xz;  
		xy = A.xx*B.yx + A.yx*B.yy + A.zx*B.yz;  
		xz = A.xx*B.zx + A.yx*B.zy + A.zx*B.zz;
		yx = A.xy*B.xx + A.yy*B.xy + A.zy*B.xz;  
		yy = A.xy*B.yx + A.yy*B.yy + A.zy*B.yz;  
		yz = A.xy*B.zx + A.yy*B.zy + A.zy*B.zz; 
		zx = A.xz*B.xx + A.yz*B.xy + A.zz*B.xz;  
		zy = A.xz*B.yx + A.yz*B.yy + A.zz*B.yz;  
		zz = A.xz*B.zx + A.yz*B.zy + A.zz*B.zz; 
	};

// ====== matrix solver

   inline double determinant() {
        double fCoxx = yy * zz - yz * zy;
        double fCoyx = yz * zx - yx * zz;
        double fCozx = yx * zy - yy * zx;
        double fDet = xx * fCoxx + xy * fCoyx + xz * fCozx;
        return fDet;
    };

	inline void invert_to( Mat3d& Mout ) {
        double idet = 1/determinant(); // we dont check det|M|=0
        Mout.xx = ( yy * zz - yz * zy ) * idet;
        Mout.xy = ( xz * zy - xy * zz ) * idet;
        Mout.xz = ( xy * yz - xz * yy ) * idet;
        Mout.yx = ( yz * zx - yx * zz ) * idet;
        Mout.yy = ( xx * zz - xz * zx ) * idet;
        Mout.yz = ( xz * yx - xx * yz ) * idet;
        Mout.zx = ( yx * zy - yy * zx ) * idet;
        Mout.zy = ( xy * zx - xx * zy ) * idet;
        Mout.zz = ( xx * yy - xy * yx ) * idet;
    };

    inline void adjoint_to( Mat3d& Mout ) {
        Mout.xx = yy * zz - yz * zy;
        Mout.xy = xz * zy - xy * zz;
        Mout.xz = xy * yz - xz * yy;
        Mout.yx = yz * zx - yx * zz;
        Mout.yy = xx * zz - xz * zx;
        Mout.yz = xz * yx - xx * yz;
        Mout.zx = yx * zy - yy * zx;
        Mout.zy = xy * zx - xx * zy;
        Mout.zz = xx * yy - xy * yx;
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

// ====== basic aritmetic

// ============== Basic Math

    inline Quat4d operator*=( double f        ) { x*=f; y*=f; z*=f; z*=f; w*=f;   return *this; };
    inline Quat4d operator+=( const Quat4d& q ) { x+=q.x; y+=q.y; z+=q.z; w+=q.w; return *this; };
    inline Quat4d operator-=( const Quat4d& q ) { x-=q.x; y-=q.y; z-=q.z; w-=q.w; return *this; };

    inline double dot ( Quat4d q ) {  return       w*q.w + x*q.x + y*q.y + z*q.z;   }
    inline double mag2(          ) {  return       w*  w + x*  x + y*  y + z*  z;   }
	inline double mag (          ) {  return sqrt( w*  w + x*  x + y*  y + z*  z ); }
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
        double aw = a.w, ax = a.x, ay = a.y, az = a.z;
        double x_ =  x * aw + y * az - z * ay + w * ax;
        double y_ = -x * az + y * aw + z * ax + w * ay;
        double z_ =  x * ay - y * ax + z * aw + w * az;
                w = -x * ax - y * ay - z * az + w * aw;
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
		    result.xx = 1 - (yy + zz);
		    result.xy =     (xy - zw);
		    result.xz =     (xz + yw);
		    result.yx =     (xy + zw);
		    result.yy = 1 - (xx + zz);
		    result.yz =     (yz - xw);
		    result.zx =     (xz - yw);
		    result.zy =     (yz + xw);
		    result.zz = 1 - (xx + yy);
	};


	inline void toMatrix_unitary( Mat3d& result) {
		double xx = x * x;
		double xy = y * y;
		double xz = x * z;
		double xw = x * w;
		double yy = y * y;
		double yz = y * z;
		double yw = y * w;
		double zz = z * z;
		double zw = z * w;
		result.xx = 1 - 2 * ( yy + zz );
		result.xy =     2 * ( xy - zw );
		result.xz =     2 * ( xz + yw );
		result.yx =     2 * ( xy + zw );
		result.yy = 1 - 2 * ( xx + zz );
		result.yz =     2 * ( yz - xw );
		result.zx =     2 * ( xz - yw );
		result.zy =     2 * ( yz + xw );
		result.zz = 1 - 2 * ( xx + yy );
	};


	inline void toMatrix_unitary2( Mat3d& result) {
		double x2 = 2*x;
		double y2 = 2*x;
		double z2 = 2*x;
		double xx = x2 * x;
		double xy = y2 * y;
		double xz = x2 * z;
		double xw = x2 * w;
		double yy = y2 * y;
		double yz = y2 * z;
		double yw = y2 * w;
		double zz = z2 * z;
		double zw = z2 * w;
		result.xx = 1 - ( yy + zz );
		result.xy =     ( xy - zw );
		result.xz =     ( xz + yw );
		result.yx =     ( xy + zw );
		result.yy = 1 - ( xx + zz );
		result.yz =     ( yz - xw );
		result.zx =     ( xz - yw );
		result.zy =     ( yz + xw );
		result.zz = 1 - ( xx + yy );
	};

	inline void fromMatrix( const Vec3d& a, const Vec3d& b, const Vec3d& c ) { fromMatrix( a.x,  a.y,  a.z,  b.x,  b.y,  b.z,  c.x,  c.y,  c.z  );  }
	inline void fromMatrix( const Mat3d& M                                 ) { fromMatrix( M.xx, M.xy, M.xz, M.yx, M.yy, M.yz, M.zx, M.zy, M.zz );  }
    inline void fromMatrix( double mxx, double mxy, double mxz, double myx, double myy, double myz, double mzx, double mzy, double mzz) {
		double t = mxx + myy + mzz;
		// we protect the division by s by ensuring that s>=1
		if (t >= 0) { // |w| >= .5
			double s = sqrt(t + 1); // |s|>=1 ...
			w = 0.5f * s;
			s = 0.5f / s;                 // so this division isn't bad
			x = (mzy - myz) * s;
			y = (mxz - mzx) * s;
			z = (myx - mxy) * s;
		} else if ((mxx > myy) && (mxx > mzz)) {
			double s = sqrt(1.0f + mxx - myy - mzz); // |s|>=1
			x = s * 0.5f; // |x| >= .5
			s = 0.5f / s;
			y = (myx + mxy) * s;
			z = (mxz + mzx) * s;
			w = (mzy - myz) * s;
		} else if (myy > mzz) {
			double s = sqrt(1.0f + myy - mxx - mzz); // |s|>=1
			y = s * 0.5f; // |y| >= .5
			s = 0.5f / s;
			x = (myx + mxy) * s;
			z = (mzy + myz) * s;
			w = (mxz - mzx) * s;
		} else {
			double s = sqrt(1.0f + mzz - mxx - myy); // |s|>=1
			z = s * 0.5f; // |z| >= .5
			s = 0.5f / s;
			x = (mxz + mzx) * s;
			y = (mzy + myz) * s;
			w = (myx - mxy) * s;
		}
	};

};



