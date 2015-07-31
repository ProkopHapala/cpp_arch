

This is program for visualisation of PDOS files form FIREBALL (dens_*.dat) in real space graphicly


Instalation
-----------
You need
1)OpenGL
2)GLUT binaries (http://www.opengl.org/resources/libraries/glut/)

INPUT
-----

To run program you need inputt files
answer.bas
dens_map.dat

if you don't have density file in format of dens_map.dat you may use sciprt dosmerge.sh to generate it from files
dens_0XY.dat (only dens_000.dat...dens_099.dat will be loaded) and dens_TOT.dat.

to generate dens_map.dat copy dosmerge.sh into directory containing dens*.dat files and run it in the directory.
the script need bash and awk.

to run DOSview copy answer.bas and dens_map.dat into the same directory of DOSview and run DOSview in the directory
(means - if you run it on linux from desktop it usually don't run in the directory and crush)

GUI
---

In the lower part you se total DOS of the molecule, green parts are selected for displaying PDOS on molecule
use left mouse button for seting start of the energy interval and right mouse button for end 
(just click on the lower part of the graph). The value shown is SUM of values in the energy interval for a given atom.
It's normalized such that most populated atom has sphere of radius 0.5A or white color.



The numbers under of graph are energy value of start/end in eV.
the numbers on the molecule shown by default are 

To rotate molecule click (left) and drag in the upper part of sreen.

please do not resize or maximize the window, this case is not well handled yet.

keyboard
--------
"+" , "-"   - zoom
"/" , "*"   - move molecule in z-direction (in and out) - similar to zoom
"c" - change colorscheme
"m" - change view mode: a] DOS as sphere ragdius  b] DOS as sphere color

"v" - write absolute value on atom
"e" - write element (proton number) of atom
"p" - write % value of current atom in comparison to atom with max value
"s" - write share (%) of the total DOS in the energy interval is localized on the atom

Mouse
-----
left button drag in upper part : Rotate molecule
right button drag in upper part : Move molecule in x,y
left button click under graph - set start of energy interval
right button click under graph - set end of energy interval


Compilation
-----------

Linux - run make on the makefile, you need GLUT-devel package installed and also something like g++ :)

Windows - there are project files for DevCpp. The makefile.win could probably work with any mingw, but not tested.
http://www.bloodshed.net/devcpp.html

In each case you need GLUT devel, GLUT bins and OpenGL.