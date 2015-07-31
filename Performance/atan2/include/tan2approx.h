
double atan2_a1( double y, double x ){
  //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
  //Volkan SALMA
  double a, angle;
  double abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition  
  if ( x < 0 ){
    a     = ( x + abs_y ) / ( abs_y - x );
    angle = 2.35619449019d;
  }else{
    a     = ( x - abs_y ) / ( x + abs_y );
    angle = 0.78539816339d;
  }
  double aa = a * a;
  //angle += a * ( -0.9817d + 0.1963d * aa );  
  // angle +=  a * ( -1 + aa*( 0.326388646629 + aa*( -0.155559850719 + aa*0.0437730406925 ) ) );
  angle +=  a * ( -1 + aa*( 0.331768825725 + aa*( -0.184940152398 + aa*( 0.091121250024 -0.0233480867489*aa ) ) ) ); 
  return  ( y<0 )? -angle : angle ;
}


double atan2_a2( double y, double x ){
  double absx,absy;
  Uint8 kind=0;
  double a;
  if(    x > 0    ){ absx = x;   kind |= 2; }else{ absx=-x;  } 
  if(    y > 0    ){ absy = y;   kind |= 4; }else{ absy=-y;  } 
  if( absx > absy ){ a = absy / absx;       }else{ a = absx / absy; kind |= 1; }
  //double alfa = atan_poly( a ); 
  double alfa = a * ( 1 + a * ( 0.00317436203193 + a * ( -0.362955638193 + a * ( 0.0920711110177 + a*( 0.105247322934 + a*-0.0521389943934 ) ) ) ) );
  switch( kind ){
    case 0:  return -3.14159265359 + alfa; 
    case 1:  return -1.57079632679 - alfa; 
    case 2:  return                - alfa; 
    case 3:  return -1.57079632679 + alfa; 
    case 4:  return  3.14159265359 - alfa; 
    case 5:  return  1.57079632679 + alfa; 
    case 6:  return                  alfa; 
    case 7:  return +1.57079632679 - alfa;
  }
  return 0;
}

inline double atan_poly( double a ){
  return a * ( 1 + a * ( 0.00317436203193 + a * ( -0.362955638193 + a * ( 0.0920711110177 + a*( 0.105247322934 + a*-0.0521389943934 ) ) ) ) );
  /*
  final double c3 = -0.365791976855; 
  final double c4 =  0.151190140253;
  return a * ( 1 + a * a * ( c3 + a * c4 ) ); 
  */
}


double atan2_a3( double y, double x ){
  if( x > 0 ){
    if( y > 0 ){
      if( x > y ){
        double a = y / x;
        return atan_poly( a );
      }else{
        double a = x / y;
        return 1.57079632679 - atan_poly( a );
      }
    }else{
      if( x > -y ){
        double a = y / x;
        return -atan_poly( -a );
      }else{
        double a = x / y;
        return -1.57079632679 + atan_poly( -a );
      }
    }
  }else{
    if( y > 0 ){
      if( -x > y ){
        double a = y / x;
        return 3.14159265359 - atan_poly( -a );
      }else{
        double a = x / y;
        return 1.57079632679 + atan_poly( -a );
      }
    }else{
      if(  x < y ){
        double a = y / x;
        return -3.14159265359 + atan_poly( a );
      }else{
        double a = x / y;
        return -1.57079632679 - atan_poly( a );
      }
    }
  }
}


// ============= for testing

double atan2_a1_test( double phi ){	
	double x = cos( phi );	double y = sin( phi );
	return atan2_a1( y, x );
}
double atan2_a1_err( double phi ){	
	double x = cos( phi );	double y = sin( phi );
	return atan2_a1( y, x ) - atan2( y, x );;
}

double atan2_a2_test( double phi ){
	double x = cos( phi );	double y = sin( phi );
	return atan2_a2( y, x );
}
double atan2_a2_err( double phi ){	
	double x = cos( phi );	double y = sin( phi );
	return atan2_a2( y, x ) - atan2( y, x );;
}

double atan2_a3_test( double phi ){
	double x = cos( phi );	double y = sin( phi );
	return atan2_a3( y, x );
}
double atan2_a3_err( double phi ){	
	double x = cos( phi );	double y = sin( phi );
	return atan2_a3( y, x ) - atan2( y, x );;
}

double atan2_test( double phi ){
	double x = cos( phi );	double y = sin( phi );
	return atan2( y, x );
}

