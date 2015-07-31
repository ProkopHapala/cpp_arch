
# ========== initialize SDLapp

def makeclean( ):
	import os
	[ os.remove(f) for f in os.listdir(".") if f.endswith(".so") ]
	[ os.remove(f) for f in os.listdir(".") if f.endswith(".o") ]
	[ os.remove(f) for f in os.listdir(".") if f.endswith(".pyc") ]

makeclean( )

import SDLapp

# ========== run SDLapp

import numpy as np



SDLapp.lib._setup (     )


'''
points = ( np.random.random( (10,3) ) - 0.5 ) * 3
links  = np.random.random( (30,2) ) * 10.0
links  = links.astype(np.int32).copy()
#print " points : ", points 
#print " links  : ", links
#i = SDLapp.putBox (     )
i = SDLapp.putLines ( links, points )
SDLapp.lib._loop  ( 200 )
SDLapp.lib.erase  ( i   )
SDLapp.lib._loop  ( 200 )
'''

radii  = np.array([ 1.4,      1.8,      1.7             ])
#colors = np.array([ int("FFF0F0F0",16), int("FF101010",16), int("FFFF0000",16)  ]).astype(np.uint32).copy()
colors = np.array([ int("FFF0F0F0",16), int("FF101010",16), int("FFFF0000",16)  ]).astype(np.uint32).copy()
vhex = np.vectorize(hex)
print "radii",   radii
print "colors",  vhex(colors)

SDLapp.setAtomTypes( radii, colors )

atoms=np.array([
 [ 2,    0.0, -1.0, 0.0 ],
 [ 1,    0.0,  0.0, 0.0 ],
 [ 0,   -0.8,  0.5, 0.0 ],
 [ 0,   +0.8,  0.5, 0.0 ]
])

xyzs  = atoms[:,1:].copy()
types = atoms[:,0 ].astype( np.int32 ).copy()

print "xyzs",  xyzs
print "types", types

SDLapp.addMolecule( types, xyzs )
SDLapp.lib._loop  ( 10000 )


SDLapp.lib._quit  (     )

