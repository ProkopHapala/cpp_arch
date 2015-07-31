import numpy as np
from   ctypes import c_int, c_double, c_bool
import ctypes
import os

name='SDLapp' 
ext='.so' 

def recompile( 
		FFLAGS = "-std=c++11 -Og -g -Wall",
		LFLAGS = "-I/usr/local/include/SDL2 -lSDL2"
	):
	import os
	print " COMPILATION OF : "+name
	print  os.getcwd()
	#os.system('g++ -I/usr/local/include/SDL2 -lSDL2 -c -fPIC '+name+'.cpp -o '+name+'.o')
	#os.system('g++ -I/usr/local/include/SDL2 -shared -Wl,-soname,lib'+name+ext+' -o lib'+name+ext+' '+name+'.o')
	os.system("g++ "+FFLAGS+" -c -fPIC "+name+".cpp -o "+name+".o "+LFLAGS )
	os.system("g++ "+FFLAGS+" -shared -Wl,-soname,lib"+name+ext+" -o lib"+name+ext+" "+name+".o "+LFLAGS)

if not os.path.exists("./"+name+ext):
	recompile()

libSDL = ctypes.CDLL( "/usr/local/lib/libSDL2.so",  ctypes.RTLD_GLOBAL )
libGL = ctypes.CDLL( "/usr/lib/nvidia-current/libGL.so",  ctypes.RTLD_GLOBAL )
#libGL  = ctypes.CDLL( "/usr/lib/nvidia-331/libGL.so",  ctypes.RTLD_GLOBAL )
lib    = ctypes.CDLL("./libSDLapp.so" )

array1ui = np.ctypeslib.ndpointer(dtype=np.uint32, ndim=1, flags='CONTIGUOUS')
array1i  = np.ctypeslib.ndpointer(dtype=np.int32,  ndim=1, flags='CONTIGUOUS')
array2i  = np.ctypeslib.ndpointer(dtype=np.int32,  ndim=2, flags='CONTIGUOUS')
array1d  = np.ctypeslib.ndpointer(dtype=np.double, ndim=1, flags='CONTIGUOUS')
array2d  = np.ctypeslib.ndpointer(dtype=np.double, ndim=2, flags='CONTIGUOUS')

# void _setup( )
lib._setup.argtypes  = None
lib._setup.restype   = None

# void _quit( )
lib._quit.argtypes   = None
lib._quit.restype    = None

# void _loop( int n )
lib._loop.argtypes   = [c_int]
lib._loop.restype    = None

# bool erase( int i )
lib.erase.argtypes   = [c_int]
lib.erase.restype    = c_bool

# int putBox( double * from, double * to, double * color  )
lib.putBox.argtypes   = [array1d,array1d,array1d]
lib.putBox.restype    = c_int
def putBox( pos1=(-1.0,-1.0,-1.0), pos2=(1.0,1.0,1.0), color=(0.8,0.8,0.8) ):
	return lib.putBox( np.array(pos1), np.array(pos2), np.array(color) )

# int putLines( int nlinks, int * links, double * points_ ){
lib.putLines.argtypes   = [c_int,array2i,array2d]
lib.putLines.restype    = c_int
def putLines( links, points ):
	return lib.putLines( len(links), links, points )


# void setAtomTypes( int ntypes_, double * radii_, int * colors_ )
lib.setAtomTypes.argtypes   = [c_int,array1d,array1ui]
lib.setAtomTypes.restype    = None
def setAtomTypes( radii, colors ):
	return lib.setAtomTypes( len(radii), radii, colors )

# void addMolecule( int natoms, int * types, double * xyzs ){

lib.addMolecule.argtypes   = [c_int,array1i,array2d]
lib.addMolecule.restype    = None
def addMolecule( types, xyzs ):
	return lib.addMolecule( len(types), types, xyzs )














