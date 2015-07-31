
//#include <math.h>
//#include <stdio.h>

/////////////////////////
//   CLASS :   Vec3f
//////////////////////////

class Vec3f{
	public:
	union{
		struct{ float x,y,z; };
		float array[3];
	};

    inline void set( const Vec3f&  v        ) { x=v.x; y=v.y; z=v.z; };
	inline void set( const float  f        ) { x=f; y=f; z=f; };
    inline void set( const float fx, const float fy, const float fz ) { x=fx; y=fy; z=fz; };


	inline Vec3f& operator =( const float f ) { x=f; y=f; z=f; return *this; };
    inline Vec3f& operator+=( const float f ) { x+=f; y+=f; z+=f; return *this; };
    inline Vec3f& operator*=( const float f ) { x*=f; y*=f; z*=f; return *this; };

    inline Vec3f& operator+=( const Vec3f&  v ) { x+=v.x; y+=v.y; z+=v.z; return *this; };
    inline Vec3f& operator-=( const Vec3f&  v ) { x-=v.x; y-=v.y; z-=v.z; return *this; };
    inline Vec3f& operator*=( const Vec3f&  v ) { x*=v.x; y*=v.y; z*=v.z; return *this; };
    inline Vec3f& operator/=( const Vec3f&  v ) { x/=v.x; y/=v.y; z/=v.z; return *this; };

    inline Vec3f operator+ ( const float f ) const { Vec3f vo; vo.x=x+f; vo.y=y+f; vo.z=z+f; return vo; };
    inline Vec3f operator* ( const float f ) const { Vec3f vo; vo.x=x*f; vo.y=y*f; vo.z=z*f; return vo; };

    inline Vec3f operator+ ( const Vec3f& vi ) const { Vec3f vo; vo.x=x+vi.x; vo.y=y+vi.y; vo.z=z+vi.z; return vo; };
    inline Vec3f operator- ( const Vec3f& vi ) const { Vec3f vo; vo.x=x-vi.x; vo.y=y-vi.y; vo.z=z-vi.z; return vo; };
    inline Vec3f operator* ( const Vec3f& vi ) const { Vec3f vo; vo.x=x*vi.x; vo.y=y*vi.y; vo.z=z*vi.z; return vo; };
    inline Vec3f operator/ ( const Vec3f& vi ) const { Vec3f vo; vo.x=x/vi.x; vo.y=y/vi.y; vo.z=z/vi.z; return vo; };

    inline void fma( const float f, const Vec3f& b ){ x+=f*b.x; y+=f*b.y; z+=f*b.z;       };

    inline void fma( const Vec3f& a, const Vec3f& b ){ x+=a.x*b.x; y+=a.y*b.y; z+=a.z*b.z; };
    inline void fms( const Vec3f& a, const Vec3f& b ){ x-=a.x*b.x; y-=a.y*b.y; z-=a.z*b.z; };
    inline void fda( const Vec3f& a, const Vec3f& b ){ x+=a.x/b.x; y+=a.y/b.y; z+=a.z/b.z; };
    inline void fds( const Vec3f& a, const Vec3f& b ){ x-=a.x/b.x; y-=a.y/b.y; z-=a.z/b.z; };

    inline Vec3f fma_func( const float f, const Vec3f& b ){ Vec3f vo; vo.x=x+f*b.x;   vo.y=y+f*b.y;   vo.x=z+f*b.z;   return vo; };
    inline Vec3f fma_func( const Vec3f& a, const Vec3f& b ){ Vec3f vo; vo.x=x+a.x*b.x; vo.y=y+a.y*b.y; vo.z=z+a.z*b.z; return vo; };
    inline Vec3f fms_func( const Vec3f& a, const Vec3f& b ){ Vec3f vo; vo.x=x-a.x*b.x; vo.y=y-a.y*b.y; vo.z=z-a.z*b.z; return vo; };
    inline Vec3f fda_func( const Vec3f& a, const Vec3f& b ){ Vec3f vo; vo.x=x+a.x/b.x; vo.y=y+a.y/b.y; vo.z=z+a.z/b.z; return vo; };
    inline Vec3f fds_func( const Vec3f& a, const Vec3f& b ){ Vec3f vo; vo.x=x-a.x/b.x; vo.y=y-a.y/b.y; vo.z=z-a.z/b.z; return vo; };

    inline float dot ( const Vec3f& a ){ return x*a.x + y*a.y + z*a.z;  };
    inline float mag2(                ){ return x*x+y*y+z*z;            };

    inline void  crossLocal( const Vec3f& a ){ float _x=y*a.z-z*a.y; 	float _y=z*a.x-x*a.z; z=x*a.y-y*a.x;     x=_x; y=_y;  };
    inline Vec3f cross     ( const Vec3f& a ){ Vec3f vo; vo.x=y*a.z-z*a.y; vo.y=z*a.x-x*a.z; vo.z=x*a.y-y*a.x;   return vo;   };

	void print(){ printf( " % % % \n",x,y,z); };

};


/////////////////////////
//   CLASS :   Mat3f
//////////////////////////


class Mat3f{
	public:
	union{
		struct{
			float xx,xy,xz;
			float yx,yy,yz;
			float zx,zy,zz;
		};
		struct{	Vec3f a,b,c; };
		float array[9];
	};

	inline Vec3f getRowx(){ Vec3f out; out.x = xx; out.y = xy; out.z = xz; return out; }
	inline Vec3f getRowy(){ Vec3f out; out.x = yx; out.y = yy; out.z = yz; return out; }
	inline Vec3f getRowz(){ Vec3f out; out.x = zx; out.y = zy; out.z = zz; return out; }

	inline void  setRowx( Vec3f v ){ xx = v.x; xy = v.y; xz = v.z; };
	inline void  setRowy( Vec3f v ){ yx = v.x; yy = v.y; yz = v.z; };
	inline void  setRowz( Vec3f v ){ zx = v.x; zy = v.y; zz = v.z; };

	inline Vec3f getColx(){ Vec3f out; out.x = xx; out.y = yx; out.z = zx; return out; }
    inline Vec3f getColy(){ Vec3f out; out.x = xy; out.y = yy; out.z = zy; return out; }
    inline Vec3f getColz(){ Vec3f out; out.x = xz; out.y = yz; out.z = zz; return out; }

	inline void  setColx( Vec3f v ){ xx = v.x; yx = v.y; zx = v.z; };
	inline void  setColy( Vec3f v ){ xy = v.x; yy = v.y; zy = v.z; };
	inline void  setColz( Vec3f v ){ xz = v.x; yz = v.y; zz = v.z; };

	inline Vec3f dot( const Vec3f&  v ){
		Vec3f vout;
		vout.x = xx*v.x + xy*v.y + xz*v.z;  
		vout.y = yx*v.x + yy*v.y + yz*v.z;  
		vout.z = zx*v.x + zy*v.y + zz*v.z;  
		return vout;
	}


	inline Mat3f dot( const Mat3f&  M ){
		Mat3f Mout;
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

	inline Mat3f transpose(){
		Mat3f Mout;
		Mout.xy=yx; Mout.yx=xy;
		Mout.xz=zx; Mout.zx=xz;
		Mout.yz=zy; Mout.zy=yz;
		return Mout;
	}


   inline float determinant() {
        float fCo00 = yy * zz - yz * zy;
        float fCo10 = yz * zx - yx * zz;
        float fCo20 = yx * zy - yy * zx;
        float fDet = xx * fCo00 + xy * fCo10 + xz * fCo20;
        return fDet;
    }

	inline Mat3f invert() {
		Mat3f Mout;
        float idet = 1/determinant(); // we dont check det|M|=0
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

    inline Mat3f adjoint() {
		Mat3f Mout;
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


