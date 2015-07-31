#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
//#include "basload.c"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define DEPTH 5 //hloubka pocatku sceny v obrazu 
#define EYESEP .3

using namespace std;

float Radii [] ={0.365	,0.320	
,1.395	,0.975	,0.835	,0.735	,0.700	,0.665	,0.605	,0.690	
,1.670	,1.400	,1.215	,1.105	,1.030	,1.010	,0.995	,0.840	
,2.080	,1.770	,1.520	,1.380	,1.300	,1.335	,1.395	,1.325	,1.305	,1.280	,1.365	,1.330	,1.280	,1.235	,1.170	,1.155	,1.145	,1.100	
,2.230	,1.960	,1.710	,1.515	,1.410	,1.450	,1.455	,1.280	,1.350	,1.355	,1.565	,1.515	,1.495	,1.430	,1.415	,1.375	,1.365	,1.300	
,2.425	,2.065	,1.820	,1.850	,1.850	,1.850	,1.850	,1.850	,1.850	,1.800	,1.750	,1.750	,1.750	,1.750	,1.750	,1.750	,1.675	,1.525	,1.415	,1.405	,1.470	,1.290	,1.360	,1.315	,1.395	,1.495	,1.690	,1.635	,1.530	,1.900	};


//////////////////////
//----DATA TYPES----//

struct TVec3D {
       float x,y,z;
       };


struct TAtom {
float x,y,z;
int element;
float charge;
float refcharge;
float value;
};

struct TBond {
       int i,j;
       float r;
       };
       
//////////////////////////////////////////////       
// --------- GLOBAL VARIABLES --------------//


//variables used for rotation
int displayMode = 0; 
float matrix[4][4];
float curquat[4],lastquat[4];
int rotate = 0;
int cas;
float zoom = 0.1;
float beginx,beginy;
int moving = 0;int scaling = 0;int spinning = 0;
TVec3D shift = { 0.0, 0.0, 0.0}; //contains molecule shift in scene
TVec3D oshift = { 0.0, 0.0, 0.0}; //contains old molecule shift in scene

//calibration scene-windows
float winx=1.66,winy=1.66; //depending on the perspective and windows aspect ratio
float mx,my; //Actual mouse position in natural coordinate
float mwx,mwy; //Actual mouse position in scene coordinate

//materialy
float ambient[]   = { 0.5f, 0.5f, 0.5f, 1.0f};
float diffuse[]   = {  0.7f, 0.7f, 0.7f, 1.0f};
float specular[]  = {   1.0f, 1.0f, 1.0f, 1.0f};
float shininess[] = {   80.0};
float position[]  = {   0.0, 0.0, +2.0, 1.0};

void* font=(void*)GLUT_BITMAP_8_BY_13; // font to use
char s[30]; // use to common text output
int MOLECULE,GUI; //indentifiers of render lists
int natoms; int nsamples; //size of dynamic data structures
GLUquadric *Sphere;

float maxvalue; float maxdens; float sumTOT;

int firstsamp,lastsamp; //index of begin and end of projected DOS

int viewmode=-1;int colorscheme=0;
int writevalue=1; int writeelement=-1;int writepercent= -1;int writeshare=-1;

//debug
FILE *debug;

//------ DATA---------//
TAtom* Atoms;
vector<TBond*> Bonds;


///////////////////////////////////
///////////////////////////////////
// ---- END OF DECLARATIONS ---- //
///////////////////////////////////
///////////////////////////////////


void renderBitmapString(float x, float y, float z, void *font,char *string)
{  char *c;
  glRasterPos3f(x, y, 0.0);
  for (c=string; *c != '\0'; c++)glutBitmapCharacter(font, *c);
}

//////////////////////////////////
//----LOAD CHARGES FROM FILE----//
////////////////////////////////// 
void LoadCharge(){
string line1;string line2;
float value,sum;

//vlastni nacteni dat
ifstream fin1("CHARGES");
getline (fin1,line1);
for (int j=0;j<natoms;j++){
   getline (fin1,line1);
   istringstream in1;
   in1.str(line1);
   sum=0;
   //fprintf(debug,"\n  %d)",j);
   while( in1 >> value )sum+=value; //dokud na radku neco je scitej
   //fprintf(debug,"\n atom %d has sum %f )",j,sum);
   Atoms[j].charge = sum;
   //fprintf(debug,"\n atom %d has charge %f )",j,Atoms[j].charge);
};
fin1.close(); 

ifstream fin2("CHARGES-ref");
getline (fin2,line2);
for (int j=0;j<natoms;j++){
   getline (fin2,line2);
   istringstream in2;
   in2.str(line2);
   sum=0;
   //fprintf(debug,"\n  %d)",j);
   while( in2 >> value )sum+=value; //dokud na radku neco je scitej
   fprintf(debug,"\n atom %d has sum %f )",j,sum);
   Atoms[j].refcharge = sum;
   //fprintf(debug,"\n atom %d has charge %f )",j,Atoms[j].charge);
};
fin2.close(); 

//debug
for(int i=0;i<natoms;i++)
{//float val=Atoms[i].charge;
//fprintf(debug,"\n atom %d has charge %f )",i,Atoms[i].charge);
fprintf(debug,"\n atom %d has refcharge %f )",i,Atoms[i].refcharge);
}
};

///////////////////////////////////////
//----Compute Atom Property Value----//
///////////////////////////////////////

void CompValues(){
maxvalue=0;
//for(int j=0;j<natoms;j++)Atoms[j].value=Atoms[j].charge;
for(int j=0;j<natoms;j++)Atoms[j].value=(Atoms[j].charge-Atoms[j].refcharge);
for(int j=0;j<natoms;j++)if (fabs(Atoms[j].value)>maxvalue)maxvalue=fabs(Atoms[j].value);
//debug
//for(int j=0;j<natoms;j++)fprintf(debug,"\n atom %d has charge %f )",j,Atoms[j].charge);
}

//////////////////////
//----Find Bonds----//
////////////////////// 

void FindBonds(){
float r;
float dx,dy,dz;
float rbond;
int nbonds=0;
    for(int j=0;j<natoms;j++){
            for(int i=0;i<j;i++){
    dx=Atoms[j].x-Atoms[i].x;
    dy=Atoms[j].y-Atoms[i].y;
    dz=Atoms[j].z-Atoms[i].z;
    r=sqrt(dx*dx+dy*dy+dz*dz);
    rbond=Radii[Atoms[i].element]+Radii[Atoms[j].element];
    rbond=rbond*1.3;
    // if (r<2.6){
    if (r<rbond){
    nbonds++;
    TBond* bond = new TBond();
    bond->i=i;bond->j=j;bond->r=r;
    Bonds.push_back(bond);
    }
    }}
    };

///////////////////////
//--Centre Molecule--//
///////////////////////
void CentreMol(){
float ax=0;float ay=0;float az=0;
for(int j=0;j<natoms;j++){ //compute average position
ax+=Atoms[j].x;ay+=Atoms[j].y;az+=Atoms[j].z;
};
ax/=natoms;ay/=natoms;az/=natoms;
fprintf(debug,"molcular centre is ( %f , %f , %f )",ax,ay,az);
for(int j=0;j<natoms;j++){ //substract average
Atoms[j].x-=ax;Atoms[j].y-=ay;Atoms[j].z-=az;
};

//debug - try if now is molecule in centre
/*
for(int j=0;j<natoms;j++){ //compute average position
ax+=Atoms[j].x;ay+=Atoms[j].y;az+=Atoms[j].z;
};
ax/=natoms;ay/=natoms;az/=natoms;
fprintf(debug,"molcular centre is ( %f , %f , %f )",ax,ay,az);
*/
};


//////////////////////////////////////////
//----Render Molecule to Render list----//
//////////////////////////////////////////
int MakeMol(){ //paint molecule into render list
    int list;
    int maxbond = Bonds.size();

glDeleteLists(MOLECULE,1); //because of memory friendly :-)    
glNewList(list=glGenLists(1),GL_COMPILE);
    Sphere=gluNewQuadric();
    
    //Bonds As Lines
glDisable (GL_LIGHTING);
 glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    for (int ib=0;ib<maxbond;ib++) {
    int i=Bonds[ib]->i;int j=Bonds[ib]->j;
    glVertex3f(Atoms[i].x,Atoms[i].y,Atoms[i].z);
    glVertex3f(Atoms[j].x,Atoms[j].y,Atoms[j].z);
    };
 glEnd();
      

//AtomSize(Element) , AtomColor(PDOS) 
if (viewmode==1){
glDisable (GL_LIGHTING);
for (int i=0;i<natoms;i++){
glPushMatrix();
	glTranslated(Atoms[i].x,Atoms[i].y,Atoms[i].z);
	float value;
	if (Atoms[i].value>0) {value = Atoms[i].value/maxvalue;glColor3f(0.0,0.0,value);} 
        else {value = -Atoms[i].value/maxvalue;glColor3f(value,0.0,0.0);}
//colorize(value);
	gluSphere( Sphere , Radii[Atoms[i].element] , 15.0 , 15.0  ); //velikost podle atomoveho cisla
glPopMatrix();
}}

if (viewmode==-1){
for (int i=0;i<natoms;i++){
glEnable (GL_LIGHTING);
glPushMatrix();
	glTranslated(Atoms[i].x,Atoms[i].y,Atoms[i].z);
	float value;
	if (Atoms[i].value>0) {value = Atoms[i].value/maxvalue;glColor3f(0.0,0.0,value);} 
        else {value = -Atoms[i].value/maxvalue;glColor3f(value,0.0,0.0);}
	gluSphere( Sphere , value *0.7 , 25.0 , 25.0  ); //velikost podle vlastnosti (napr. hustoty) 
glPopMatrix();
}}
//NOTE: gluSphere make sometimes problems by compiling if there are other errors, but don't cause it
   
    //atomy jako tecky 
    glBegin(GL_POINTS);
    for (int i=0;i<natoms;i++) glVertex3f(Atoms[i].x,Atoms[i].y,Atoms[i].z);
    glEnd();

    //write element
    if(writeelement==1){
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_DEPTH);
    glDisable(GL_LIGHTING);
    glColor3f(1.0,1.0,1.0);
    for (int i=0;i<natoms;i++){
    sprintf(s,"%d",Atoms[i].element);
    glPushMatrix();
    glTranslated(Atoms[i].x,Atoms[i].y,Atoms[i].z);
    renderBitmapString(0,0,0,(void *)font,s);
    glPopMatrix();
    }
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_DEPTH);
    glEnable (GL_LIGHTING);
    }

    
    if(writevalue==1){
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_DEPTH);
    glDisable(GL_LIGHTING);
    glColor3f(0.0,0.5,0.0);
    for (int i=0;i<natoms;i++){
    sprintf(s,"%.2f",Atoms[i].value);
    glPushMatrix();
    glTranslated(Atoms[i].x,Atoms[i].y,Atoms[i].z);
    renderBitmapString(0,0,0,(void *)font,s);
    glPopMatrix();
    }
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_DEPTH);
    glEnable (GL_LIGHTING);
    }

glEndList();
return(list);
};

/////////////////////////
//----LOAD GEOMETRY----//
/////////////////////////
void LoadBas(char * fn){
    int list;
    FILE* f;   
    f=fopen(fn,"r");  
    fscanf(f,"%d\n",&natoms);
    Atoms=(TAtom*)malloc(sizeof(TAtom)*natoms);
    for  (int i=0; i<natoms; i++) {
    fscanf(f,"%d %f %f %f\n",&Atoms[i].element,&Atoms[i].x,&Atoms[i].y,&Atoms[i].z);     
    };
   
    CentreMol(); //centre molecular geometry arround (0,0,0)
    FindBonds(); //najde vazby
    LoadCharge();  //nacte naboje
    CompValues(); //spocita co se ma z dat zobrazit
    MOLECULE=MakeMol(); //vykresli vsechno do render listu       
    fclose(f); 
}

//// ------ Definitions from tracbal.h -------- ////
void trackball (float q[4], float p1x, float p1y, float p2x, float p2y);
void add_quats (float q1[4], float q2[4], float dest[4]);
void build_rotmatrix (float m[4][4], float q[4]);

////////////////////////
//------Camera-------///
////////////////////////
void camera (float depth, float x)
{


   int width = glutGet (GLUT_WINDOW_WIDTH);
   int height = glutGet (GLUT_WINDOW_HEIGHT);
   float ratio = (float)width / (float)height;

	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(-1,1,   -1,1,   -4,4);			// Create Ortho 640x480 View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();

    glTranslatef (0.0, 0.0, -zoom*10);    //move centre of rotation in molecule
    gluLookAt (0, 0, 0,    0, 0, -depth,     0, 1, 0);

  glMatrixMode (GL_MODELVIEW);
  build_rotmatrix (matrix, curquat);
  glTranslatef (shift.x, shift.y, 0);
  glMultMatrixf (&matrix[0][0]);

}

void PassiveMotion(int x, int y) {

//mouse in window coordinates Wondow-> <0,1>
int ww = glutGet (GLUT_WINDOW_WIDTH);
int wh = glutGet (GLUT_WINDOW_HEIGHT);
mx=(x/(float)ww);
my=1-(y/(float)wh);

//mouse in scene coordinates
mwx=(mx-0.5)*2*winx; //map <0,1> -> widow-scene;
mwy=(my-0.5)*2*winy;
};

void motion (int x, int y){
PassiveMotion(x, y);
   float w = glutGet (GLUT_WINDOW_WIDTH) / 2;
   float h = glutGet (GLUT_WINDOW_HEIGHT) / 2;

   if (spinning){
	  trackball (lastquat,(2.0*beginx-w)/w,(h-2.0*beginy)/h,(2.0*x-w)/w,(h-2.0*y)/h);
	  beginx = x; beginy = y;
	  add_quats (lastquat, curquat, curquat);
	  glutPostRedisplay ();   }
   
   if (moving){ 
   shift.x=oshift.x+(mwx-beginx);
   shift.y=oshift.y+(mwy-beginy); 
  }  

}

void mouse (int button, int state, int x, int y){

//if (my>0.15){ 
   //ovladani rotace
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	  glutIdleFunc (NULL); spinning = 1;
	  beginx = x; beginy = y; }
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)spinning = 0;
   
   //ovladani posunu
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { 
              oshift=shift; beginx=mwx; beginy=mwy;  moving = 1;}
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) moving = 0;
//}


}

void resize (int width, int height)
{
   camera (DEPTH, 0);
}

void display ()
{
//---- Setting Part ----//
 glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
 glShadeModel(GL_SMOOTH);
//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //for wireframe
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 //glEnable(GL_BLEND);					// Enable Blending
 //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Type Of Blending To Use

//-----RENDERING PART-----------//
glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//This Part will be affected by mouse rotation
   glPushMatrix();
    glTranslatef (shift.x, shift.y, shift.z);     //move molecule
    camera (DEPTH, 0);                //apply mouse camera rotation 
    glScalef (zoom, zoom, zoom);      //apply zoom
    //glTranslatef (10.0, 0.0, 0.0);    //move centre of rotation in molecule
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

 glShadeModel(GL_SMOOTH);
 //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
 glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
 glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
 glHint (GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
 glEnable(GL_POINT_SMOOTH);
 glEnable(GL_LINE_SMOOTH);
 glEnable (GL_POLYGON_SMOOTH);

    glCallList(MOLECULE);
   glPopMatrix();
  
//This Part will-NOT be affected by mouse rotation
//glDisable(GL_LINE_SMOOTH);
//glDisable(GL_LIGHTING);
//glCallList(GUI);
glutSwapBuffers ();
}

void key (unsigned char key, int x, int y)
{
if (key=='m'){viewmode*= -1;} //change view mode
if (key=='c'){colorscheme++;} //change color scheme
if (key=='r'){firstsamp=0;lastsamp=nsamples;} //reset
if (key=='v'){writevalue*= -1;} //write value on/off
if (key=='e'){writeelement*= -1;} //write element on/off
//if (key=='p'){writepercent*= -1;} //write percent of maxvalue on/off
//if (key=='s'){writeshare*= -1;} //write percent of total DOS on/off
//zoom in / ou
if (key=='+'){zoom*=1.61803399;}
if (key=='-'){zoom*=0.61803399;}
//move in / out
if (key=='/'){shift.z+=1.0;}
if (key=='*'){shift.z-=1.0;}
MOLECULE=MakeMol();

}

void special (int key, int x, int y)
{
}

void timer (int value)
{
   glutTimerFunc (value, timer, value);
   glutPostRedisplay ();
}

int main (int argc, char **argv){
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH/* | GLUT_STEREO*/);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Okno");
   glutDisplayFunc (display);
   glutReshapeFunc (resize);
   glutKeyboardFunc (key);
   glutMotionFunc (motion);
   glutPassiveMotionFunc(PassiveMotion);
   glutMouseFunc (mouse);
   glutSpecialFunc (special);
   glutTimerFunc (50, timer, 50);



 glShadeModel(GL_SMOOTH);
 //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
 glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
 glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
 glHint (GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
 glEnable(GL_POINT_SMOOTH);
 glEnable(GL_LINE_SMOOTH);
 glEnable (GL_POLYGON_SMOOTH);




//Debuging file 
debug=fopen("debug.log","w"); 
//Loading Data
   if (argc>1) LoadBas(argv[1]);
   else LoadBas("answer.bas");
fclose(debug);

   glClearColor (1.0, 1.0, 1.0, 1.0);
   glClearDepth (1.0f);
 
   glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
   glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glEnable (GL_COLOR_MATERIAL);
   glLightfv (GL_LIGHT0, GL_POSITION, position);
   glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable (GL_NORMALIZE);
   glEnable (GL_DEPTH_TEST);

   trackball (curquat, 0, 0, 0, 0);

   glEnable(GL_DEPTH);


//  ======== FOG ========
GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 0.75f};		// Fog Color
glFogfv(GL_FOG_COLOR, fogColor);
glFogi(GL_FOG_MODE, GL_LINEAR);		    // Fog Mode
glFogf(GL_FOG_DENSITY, 1.00f);			// How Dense Will The Fog Be
glHint(GL_FOG_HINT, GL_NICEST);			// Fog Hint Value
glEnable(GL_FOG);							// Enables GL_FOG
  
   glutMainLoop ();
   return 0;
}
