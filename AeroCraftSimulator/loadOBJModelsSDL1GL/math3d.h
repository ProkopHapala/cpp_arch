
// I want to move this into Vec3d.h

#include <math.h>
#include <stdio.h>

/////////////////////////
//   CLASS :   Vec3d
//////////////////////////

class Vec3d{
	public:
	union{
		struct{ double x,y,z; };
		double array[3];
	};

    inline void set( const Vec3d&  v        ) { x=v.x; y=v.y; z=v.z; };
	inline void set( const double  f        ) { x=f; y=f; z=f; };
    inline void set( const double fx, const double fy, const double fz ) { x=fx; y=fy; z=fz; };

	inline Vec3d& operator =( const double f ) { x=f; y=f; z=f; return *this; };
    inline Vec3d& operator+=( const double f ) { x+=f; y+=f; z+=f; return *this; };
    inline Vec3d& operator*=( const double f ) { x*=f; y*=f; z*=f; return *this; };

    inline Vec3d& operator+=( const Vec3d&  v ) { x+=v.x; y+=v.y; z+=v.z; return *this; };
    inline Vec3d& operator-=( const Vec3d&  v ) { x-=v.x; y-=v.y; z-=v.z; return *this; };
    inline Vec3d& operator*=( const Vec3d&  v ) { x*=v.x; y*=v.y; z*=v.z; return *this; };
    inline Vec3d& operator/=( const Vec3d&  v ) { x/=v.x; y/=v.y; z/=v.z; return *this; };

    inline Vec3d operator+ ( const double f ) const { Vec3d vo; vo.x=x+f; vo.y=y+f; vo.z=z+f; return vo; };
    inline Vec3d operator* ( const double f ) const { Vec3d vo; vo.x=x*f; vo.y=y*f; vo.z=z*f; return vo; };

    inline Vec3d operator+ ( const Vec3d& vi ) const { Vec3d vo; vo.x=x+vi.x; vo.y=y+vi.y; vo.z=z+vi.z; return vo; };
    inline Vec3d operator- ( const Vec3d& vi ) const { Vec3d vo; vo.x=x-vi.x; vo.y=y-vi.y; vo.z=z-vi.z; return vo; };
    inline Vec3d operator* ( const Vec3d& vi ) const { Vec3d vo; vo.x=x*vi.x; vo.y=y*vi.y; vo.z=z*vi.z; return vo; };
    inline Vec3d operator/ ( const Vec3d& vi ) const { Vec3d vo; vo.x=x/vi.x; vo.y=y/vi.y; vo.z=z/vi.z; return vo; };

    inline void fma( const double f, const Vec3d& b ){ x+=f*b.x; y+=f*b.y; z+=f*b.z;       };

    inline void fma( const Vec3d& a, const Vec3d& b ){ x+=a.x*b.x; y+=a.y*b.y; z+=a.z*b.z; };
    inline void fms( const Vec3d& a, const Vec3d& b ){ x-=a.x*b.x; y-=a.y*b.y; z-=a.z*b.z; };
    inline void fda( const Vec3d& a, const Vec3d& b ){ x+=a.x/b.x; y+=a.y/b.y; z+=a.z/b.z; };
    inline void fds( const Vec3d& a, const Vec3d& b ){ x-=a.x/b.x; y-=a.y/b.y; z-=a.z/b.z; };

    inline Vec3d fma_func( const double f, const Vec3d& b ){ Vec3d vo; vo.x=x+f*b.x;   vo.y=y+f*b.y;   vo.x=z+f*b.z;   return vo; };
    inline Vec3d fma_func( const Vec3d& a, const Vec3d& b ){ Vec3d vo; vo.x=x+a.x*b.x; vo.y=y+a.y*b.y; vo.z=z+a.z*b.z; return vo; };
    inline Vec3d fms_func( const Vec3d& a, const Vec3d& b ){ Vec3d vo; vo.x=x-a.x*b.x; vo.y=y-a.y*b.y; vo.z=z-a.z*b.z; return vo; };
    inline Vec3d fda_func( const Vec3d& a, const Vec3d& b ){ Vec3d vo; vo.x=x+a.x/b.x; vo.y=y+a.y/b.y; vo.z=z+a.z/b.z; return vo; };
    inline Vec3d fds_func( const Vec3d& a, const Vec3d& b ){ Vec3d vo; vo.x=x-a.x/b.x; vo.y=y-a.y/b.y; vo.z=z-a.z/b.z; return vo; };

    inline double dot ( const Vec3d& a ){ return x*a.x + y*a.y + z*a.z;  };
    inline double mag2(                ){ return x*x+y*y+z*z;            };

    inline void   cross    ( const Vec3d& a ){ double _x=y*a.z-z*a.y; 	double _y=z*a.x-x*a.z; z=x*a.y-y*a.x;     x=_x; y=_y;  };
    inline Vec3d cross_func( const Vec3d& a ){ Vec3d vo; vo.x=y*a.z-z*a.y; vo.y=z*a.x-x*a.z; vo.z=x*a.y-y*a.x;   return vo;   };

	void print(){ printf( " % % % \n",x,y,z); };
};


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
		struct{	Vec3d a,b,c; };
		double array[9];
	};

	inline Vec3d getRowx(){ Vec3d out; out.x = xx; out.y = xy; out.z = xz; return out; }
	inline Vec3d getRowy(){ Vec3d out; out.x = yx; out.y = yy; out.z = yz; return out; }
	inline Vec3d getRowz(){ Vec3d out; out.x = zx; out.y = zy; out.z = zz; return out; }

	inline void  setRowx( Vec3d v ){ xx = v.x; xy = v.y; xz = v.z; };
	inline void  setRowy( Vec3d v ){ yx = v.x; yy = v.y; yz = v.z; };
	inline void  setRowz( Vec3d v ){ zx = v.x; zy = v.y; zz = v.z; };

	inline Vec3d getColx(){ Vec3d out; out.x = xx; out.y = yx; out.z = zx; return out; }
    inline Vec3d getColy(){ Vec3d out; out.x = xy; out.y = yy; out.z = zy; return out; }
    inline Vec3d getColz(){ Vec3d out; out.x = xz; out.y = yz; out.z = zz; return out; }

	inline void  setColx( Vec3d v ){ xx = v.x; yx = v.y; zx = v.z; };
	inline void  setColy( Vec3d v ){ xy = v.x; yy = v.y; zy = v.z; };
	inline void  setColz( Vec3d v ){ xz = v.x; yz = v.y; zz = v.z; };

	inline Vec3d dot( const Vec3d&  v ){
		Vec3d vout;
		vout.x = xx*v.x + xy*v.y + xz*v.z;  
		vout.y = yx*v.x + yy*v.y + yz*v.z;  
		vout.z = zx*v.x + zy*v.y + zz*v.z;  
		return vout;
	}


	inline Mat3d dot( const Mat3d&  M ){
		Mat3d Mout;
		Mout.xx = xx*M.xx + xy*M.yx + xz*M.zx;  
		Mout.xy = xx*M.xy + yy*M.yy + yz*M.zy;  
		Mout.xz = xx*M.xz + zy*M.yz + zz*M.zz;
		Mout.yx = yx*M.xx + xy*M.yx + xz*M.zx;  
		Mout.yy = yx*M.xy + yy*M.yy + yz*M.zy;  
		Mout.yz = yx*M.xz + zy*M.yz + zz*M.zz; 
		Mout.zx = zx*M.xx + xy*M.yx + xz*M.zx;  
		Mout.zy = zx*M.xy + yy*M.yy + yz*M.zy;  
		Mout.zz = zx*M.xz + zy*M.yz + zz*M.zz; 
		return Mout;
	}

	inline Mat3d transpose(){
		Mat3d Mout;
		Mout.xy=yx; Mout.yx=xy;
		Mout.xz=zx; Mout.zx=xz;
		Mout.yz=zy; Mout.zy=yz;
		return Mout;
	}


   inline double determinant() {
        double fCo00 = yy * zz - yz * zy;
        double fCo10 = yz * zx - yx * zz;
        double fCo20 = yx * zy - yy * zx;
        double fDet = xx * fCo00 + xy * fCo10 + xz * fCo20;
        return fDet;
    }

	inline Mat3d invert() {
		Mat3d Mout;
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
        return Mout;
    }

    inline Mat3d adjoint() {
		Mat3d Mout;
        Mout.xx = yy * zz - yz * zy;
        Mout.xy = xz * zy - xy * zz;
        Mout.xz = xy * yz - xz * yy;
        Mout.yx = yz * zx - yx * zz;
        Mout.yy = xx * zz - xz * zx;
        Mout.yz = xz * yx - xx * yz;
        Mout.zx = yx * zy - yy * zx;
        Mout.zy = xy * zx - xx * zy;
        Mout.zz = xx * yy - xy * yx;
        return Mout;
    }

	void print(){
		printf( " % % % \n",xx,xy,xz);
		printf( " % % % \n",yx,yy,yz);
		printf( " % % % \n",zx,zy,zz);
	}

};
