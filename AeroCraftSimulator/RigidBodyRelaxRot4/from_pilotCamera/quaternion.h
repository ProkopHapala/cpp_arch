

// read also:
// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/

//template <class TYPE, class VEC, class MAT, class QUAT> 
//template <class TYPE, class VEC> 
template <class TYPE> 
class Quat4TYPE {
	using VEC  = Vec3TYPE<TYPE>; 
	using MAT  = Mat3TYPE<TYPE>; 
	using QUAT = Quat4TYPE<TYPE>; 
	public:
	union{
		struct{ TYPE x,y,z,w; };
		TYPE array[4];
	};

	inline void set   ( const  QUAT& q                           ){ x=q.x; y=q.y; z=q.z; w=q.w; };
	inline void set   ( TYPE fx, TYPE fy, TYPE fz, TYPE fw ){ x=fx;  y=fy;  z=fz;  w=fw; };
	inline void setOne( ){ x=y=z=0; w=1; };

	inline void set       ( TYPE * v )       { x=v[0]; y=v[1]; z=v[2]; w=v[3]; };
	inline void toArray   ( TYPE * v ) const { v[0]=x; v[1]=y; v[2]=z; v[3]=w; };

// ============== Basic Math

    inline QUAT operator*=( TYPE f        ) { x*=f; y*=f; z*=f; z*=f; w*=f;   return *this; };
    inline QUAT operator+=( const QUAT& q ) { x+=q.x; y+=q.y; z+=q.z; w+=q.w; return *this; };
    inline QUAT operator-=( const QUAT& q ) { x-=q.x; y-=q.y; z-=q.z; w-=q.w; return *this; };

    inline void mul ( TYPE f        ) { x*=f; y*=f; z*=f; z*=f; w*=f;   };
    inline void add ( const QUAT& q ) { x+=q.x; y+=q.y; z+=q.z; w+=q.w; };
    inline void sub ( const QUAT& q ) { x-=q.x; y-=q.y; z-=q.z; w-=q.w; };

    inline TYPE dot ( QUAT q ) {  return       w*q.w + x*q.x + y*q.y + z*q.z;   }
    inline TYPE norm2(         ) {  return       w*  w + x*  x + y*  y + z*  z;   }
	inline TYPE norm (         ) {  return sqrt( w*  w + x*  x + y*  y + z*  z ); }
    inline TYPE normalize() {
		TYPE norm  = sqrt( x*x + y*y + z*z + w*w );
		TYPE inorm = 1.0d/norm;
		x *= inorm;    y *= inorm;    z *= inorm;   w *= inorm;
		return norm;
    }

// ====== Quaternion multiplication

	inline void setQmul( const QUAT& a, const QUAT& b) {
        x =  a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x;
        y = -a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y;
        z =  a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z;
        w = -a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w;
    };

    inline void qmul( const QUAT& a) {
        TYPE x_ =  x * a.w + y * a.z - z * a.y + w * a.x;
        TYPE y_ = -x * a.z + y * a.w + z * a.x + w * a.y;
        TYPE z_ =  x * a.y - y * a.x + z * a.w + w * a.z;
                w = -x * a.x - y * a.y - z * a.z + w * a.w;
        x = x_; y = y_; z = z_;
    };

    inline void qmul2( const QUAT& a) {
        TYPE x_ =  a.x * w + a.y * z - a.z * y + a.w * x;
        TYPE y_ = -a.x * z + a.y * w + a.z * x + a.w * y;
        TYPE z_ =  a.x * y - a.y * x + a.z * w + a.w * z;
               w  = -a.x * x - a.y * y - a.z * z + a.w * w;
        x = x_; y = y_; z = z_;
    };

    inline void invertUnitary() { x=-x; y=-y; z=-z; }

    inline void invert() {
		TYPE norm = sqrt( x*x + y*y + z*z + w*w );
		if ( norm > 0.0 ) {
			TYPE invNorm = 1.0d / norm;
			x *= -invNorm; y *= -invNorm;z *= -invNorm;	w *=  invNorm;
		}
    };

// =======  pitch, yaw, roll

	inline void dpitch( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); pitch( ca, sa );  };
	inline void pitch ( TYPE angle ){ pitch( cos(angle), sin(angle) );  };
    inline void pitch ( TYPE ca, TYPE sa ) {
        TYPE x_ =  x * ca + w * sa;
        TYPE y_ =  y * ca + z * sa;
        TYPE z_ = -y * sa + z * ca;
                w = -x * sa + w * ca;
        x = x_; y = y_; z = z_;
    };

	inline void dyaw( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); yaw( ca, sa );  };
	inline void yaw ( TYPE angle ){ yaw( cos(angle), sin(angle) );  };
    inline void yaw ( TYPE ca, TYPE sa ) {
        TYPE x_ =  x * ca - z * sa;
        TYPE y_ =  y * ca + w * sa;
        TYPE z_ =  x * sa + z * ca;
                w = -y * sa + w * ca;
        x = x_; y = y_; z = z_;
    };

	inline void droll( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); roll( ca, sa );  };
	inline void roll ( TYPE angle ){ roll( cos(angle), sin(angle) );  };
    inline void roll ( TYPE ca, TYPE sa ) {
        TYPE x_ =  x * ca + y * sa;
        TYPE y_ = -x * sa + y * ca;
        TYPE z_ =  z * ca + w * sa;
                w = -z * sa + w * ca;
        x = x_; y = y_; z = z_;
    };


	inline void dpitch2( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); pitch2( ca, sa );  };
	inline void pitch2 ( TYPE angle ){ pitch2( cos(angle), sin(angle) );  };
    inline void pitch2 ( TYPE ca, TYPE sa ) {
/*
        TYPE x_ =  ax * w + ay * z - az * y + aw * x;
        TYPE y_ = -ax * z + ay * w + az * x + aw * y;
        TYPE z_ =  ax * y - ay * x + az * w + aw * z;
               w  = -ax * x - ay * y - az * z + aw * w;
*/
        TYPE x_ =  sa * w + ca * x;
        TYPE y_ = -sa * z + ca * y;
        TYPE z_ =  sa * y + ca * z;
               w  = -sa * x + ca * w;
        x = x_; y = y_; z = z_;
    };

	inline void dyaw2( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); yaw2( ca, sa );  };
	inline void yaw2 ( TYPE angle ){ yaw2( cos(angle), sin(angle) );  };
    inline void yaw2 ( TYPE ca, TYPE sa ) {
        TYPE x_ = + sa * z  + ca * x;
        TYPE y_ = + sa * w  + ca * y;
        TYPE z_ = - sa * x  + ca * z;
               w  = - sa * y  + ca * w;
        x = x_; y = y_; z = z_;
    };

	inline void droll2( TYPE angle ){ TYPE ca,sa; sincos_taylor2(angle,ca,sa); roll2( ca, sa );  };
	inline void roll2 ( TYPE angle ){ roll2( cos(angle), sin(angle) );  };
    inline void roll2 ( TYPE ca, TYPE sa ) {
        TYPE x_ = - sa * y + ca * x;
        TYPE y_ = + sa * x + ca * y;
        TYPE z_ = + sa * w + ca * z;
               w  = - sa * z + ca * w;
        x = x_; y = y_; z = z_;
    };

// ======= Conversion : Angle & Axis

	inline void fromAngleAxis( TYPE angle, const VEC& axis ){  
		TYPE ir   = 1/axis.norm();
		VEC  hat  = axis * ir;
		TYPE a    = 0.5d * angle;
		TYPE sa   = sin(a);
		w =           cos(a);
		x = sa * hat.x;
		y = sa * hat.y;
		z = sa * hat.z;
	};


// ====== Differential rotation

	inline void dRot_exact ( TYPE dt, const VEC& omega ) {
		TYPE hx   = omega.x;
		TYPE hy   = omega.y;
		TYPE hz   = omega.z;
		TYPE r2   = hx*hx + hy*hy + hz*hz;
		if(r2>0){
			TYPE norm = sqrt( r2 );
			TYPE a    = dt * norm * 0.5d;
			TYPE sa   = sin( a )/norm;  // we normalize it here to save multiplications
			TYPE ca   = cos( a );
			hx*=sa; hy*=sa; hz*=sa;            // hat * sin(a)
			TYPE x_ = x, y_ = y, z_ = z, w_ = w;
			x =  hx*w_ + hy*z_ - hz*y_ + ca*x_;
			y = -hx*z_ + hy*w_ + hz*x_ + ca*y_;
			z =  hx*y_ - hy*x_ + hz*w_ + ca*z_;
			w = -hx*x_ - hy*y_ - hz*z_ + ca*w_;
		}
	};


	inline void dRot_taylor2 ( TYPE dt, VEC& omega ) {
		TYPE hx   = omega.x;
		TYPE hy   = omega.y;
		TYPE hz   = omega.z;
		TYPE r2   = hx*hx + hy*hy + hz*hz;
		TYPE b2   = dt*dt*r2;
		const TYPE c2 = 1.0d/8;    // 4  *2       
		const TYPE c3 = 1.0d/48;   // 8  *2*3     
		const TYPE c4 = 1.0d/384;  // 16 *2*3*4   
		const TYPE c5 = 1.0d/3840; // 32 *2*3*4*5 
		TYPE sa   = dt * ( 0.5d - b2*( c3 - c5*b2 ) ); 
		TYPE ca   =      ( 1    - b2*( c2 - c4*b2 ) );
		hx*=sa; hy*=sa; hz*=sa;  // hat * sin(a)
		TYPE x_ = x, y_ = y, z_ = z, w_ = w;
		x =  hx*w_ + hy*z_ - hz*y_ + ca*x_;
		y = -hx*z_ + hy*w_ + hz*x_ + ca*y_;
		z =  hx*y_ - hy*x_ + hz*w_ + ca*z_;
		w = -hx*x_ - hy*y_ - hz*z_ + ca*w_;
	};


	inline void toMatrix( MAT& result) {
	    TYPE r2 = w*w + x*x + y*y + z*z;
	    //TYPE s  = (r2 > 0) ? 2d / r2 : 0;
		TYPE s  = 2 / r2;
	    // compute xs/ys/zs first to save 6 multiplications, since xs/ys/zs
	    // will be used 2-4 times each.
	    TYPE xs = x * s;  TYPE ys = y * s;  TYPE zs = z * s;
	    TYPE xx = x * xs; TYPE xy = x * ys; TYPE xz = x * zs;
	    TYPE xw = w * xs; TYPE yy = y * ys; TYPE yz = y * zs;
	    TYPE yw = w * ys; TYPE zz = z * zs; TYPE zw = w * zs;
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


	inline void toMatrix_unitary( MAT& result) {
		TYPE xx = 2*x * x;
		TYPE xy = 2*y * y;
		TYPE xz = 2*x * z;
		TYPE xw = 2*x * w;
		TYPE yy = 2*y * y;
		TYPE yz = 2*y * z;
		TYPE yw = 2*y * w;
		TYPE zz = 2*z * z;
		TYPE zw = 2*z * w;
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



	inline void fromMatrix( const VEC& a, const VEC& b, const VEC& c ) { fromMatrix( a.x,  a.y,  a.z,  b.x,  b.y,  b.z,  c.x,  c.y,  c.z  );  }
	inline void fromMatrix( const MAT& M                                 ) { fromMatrix( M.ax, M.ay, M.az, M.bx, M.by, M.bz, M.cx, M.cy, M.cz );  }

    inline void fromMatrix( TYPE max, TYPE may, TYPE maz, TYPE mbx, TYPE mby, TYPE mbz, TYPE mcx, TYPE mcy, TYPE mcz) {
		TYPE t = max + mby + mcz;
		// we protect the division by s by ensuring that s>=1
		if (t >= 0) { // |w| >= .5
			TYPE s = sqrt(t + 1); // |s|>=1 ...
			w = 0.5f * s;
			s = 0.5f / s;                 // so this division isn't bad
			x = (mcy - mbz) * s;
			y = (maz - mcx) * s;
			z = (mbx - may) * s;
		} else if ((max > mby) && (max > mcz)) {
			TYPE s = sqrt(1.0f + max - mby - mcz); // |s|>=1
			x = s * 0.5f; // |x| >= .5
			s = 0.5f / s;
			y = (mbx + may) * s;
			z = (maz + mcx) * s;
			w = (mcy - mbz) * s;
		} else if (mby > mcz) {
			TYPE s = sqrt(1.0f + mby - max - mcz); // |s|>=1
			y = s * 0.5f; // |y| >= .5
			s = 0.5f / s;
			x = (mbx + may) * s;
			z = (mcy + mbz) * s;
			w = (maz - mcx) * s;
		} else {
			TYPE s = sqrt(1.0f + mcz - max - mby); // |s|>=1
			z = s * 0.5f; // |z| >= .5
			s = 0.5f / s;
			x = (maz + mcx) * s;
			y = (mcy + mbz) * s;
			w = (mbx - may) * s;
		}
	};

};

/*
class Quat4i : public Quat4TYPE< int,    Vec3i, Mat3i, Quat4i >{  };
class Quat4f : public Quat4TYPE< float,  Vec3f, Mat3f, Quat4f >{  };
class Quat4d : public Quat4TYPE< double, Vec3d, Mat3d, Quat4d >{  };
*/

using Quat4i = Quat4TYPE< int>;
using Quat4f = Quat4TYPE< float>;
using Quat4d = Quat4TYPE< double >;


