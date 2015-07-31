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
float zoom = 0.3;
float beginx,beginy;
int moving = 0;int scaling = 0;int spinning = 0;
TVec3D shift = { 0.0, 0.0, 0.0}; //contains molecule shift in scene
TVec3D oshift = { 0.0, 0.0, 0.0}; //contains old molecule shift in scene

//calibration scene-windows
float winx=1.66,winy=1.66; //depending on the perspective and windows aspect ratio
float mx,my; //Actual mouse position in natural coordinate
float mwx,mwy; //Actual mouse position in scene coordinate

//materialy
float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};
float diffuse[] = {  0.7f, 0.7f, 0.7f, 1.0f};
float specular[] = {   1.0f, 1.0f, 1.0f, 1.0f};
float shininess[] = {   80.0};
float position[] = {   0.0, 0.0, -2.0, 1.0};

void* font=(void*)GLUT_BITMAP_8_BY_13; // font to use
char s[30]; // use to common text output
int MOLECULE,GUI; //indentifiers of render lists
int natoms; int nsamples; //size of dynamic data structures
GLUquadric *Sphere;

float maxvalue; float maxdens; float sumTOT;

int firstsamp,lastsamp; //index of begin and end of projected DOS

int viewmode=1;int colorscheme=0;
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
int nbonds=0;
    for(int j=0;j<natoms;j++){
            for(int i=0;i<j;i++){
    dx=Atoms[j].x-Atoms[i].x;
    dy=Atoms[j].y-Atoms[i].y;
    dz=Atoms[j].z-Atoms[i].z;
    r=sqrt(dx*dx+dy*dy+dz*dz);
    if (r<1.6){
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

////////////////
//--coloring--//
////////////////
void colorize(float color){
colorscheme %= 4;
if(colorscheme==0)glColor3f(color,color,color);
if(colorscheme==1)glColor3f(color,color,1.0);
if(colorscheme==2)glColor3f(1.0,color,color);
if(colorscheme==3)glColor3f(0.5,color,color);
/*
//glColor3f(0.5,color,color); //from red to green-blue
//glColor3f(1.0,color,color); //from red to white
//glColor3f(color,color,1.0); //from blue to white
glColor3f(color,color,color); //grayscale
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
 glBegin(GL_LINES);
    glColor3f(1.0f,1.0f,1.0f);
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
	gluSphere( Sphere , sqrt(Atoms[i].element)/5.0 , 15.0 , 15.0  ); //velikost podle atomoveho cisla
glPopMatrix();
}}

//AtomSize(PDOS) , AtomColor(Element) 
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
    glColor3f(0.0,1.0,0.0);
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

int PaintGUI(){
/*
    int list;
    float x;
    glDeleteLists(GUI,1);
    glNewList(list=glGenLists(1),GL_COMPILE);
    //paint graph of total density
     glLineWidth(1);
     glBegin(GL_LINES);
      for (int i=1;i<nsamples;i++){
       x=(1.0/nsamples)*i*2*winx;
       glColor3f(1.0,1.0,1.0);
       if ((i<lastsamp)&&(i>firstsamp))glColor3f(0.0,1.0,0.0);
       glVertex3f(x-winx,-winy+0.1,0.0);
       glVertex3f(x-winx,(DensTOT[i]/maxdens)-winy+0.1,0.0);
     }glEnd(); 
    //text to DOS
    glColor3f(1.0,1.0,1.0);
    x=(1.0/nsamples)*firstsamp*2*winx;
    sprintf(s,"%3.3f",Xaxis[firstsamp]);
    renderBitmapString(x-winx,-winy+0.05,0.0,(void *)font,s);
    x=(1.0/nsamples)*lastsamp*2*winx;
    sprintf(s,"%3.3f",Xaxis[lastsamp]);
    renderBitmapString(x-winx-0.1,-winy+0.05,0.0,(void *)font,s);
    
    
    //paint color scale
    if (viewmode>0){
    glLineWidth(5);
    glBegin(GL_LINES);
      for (int i=1;i<50;i++){
       float value=1.0/50*i;
       colorize(value);
	   glVertex3f(winx,value*winy-0.1,0.0);
       glVertex3f(winx*0.9,value*winy-0.1,0.0);
     }
     glColor3f(1.0,1.0,1.0);
     glVertex3f(winx,-0.1,0.0);
     glVertex3f(winx*0.9,-0.1,0.0);
     glEnd();
    };
    //text to colorscale
    glColor3f(1.0,1.0,1.0);
    sprintf(s,"%.3f",maxvalue);
    renderBitmapString(winx*0.8,winy-0.1,0.0,(void *)font,s);
    sprintf(s,"0.0");
    renderBitmapString(winx*0.8,-0.1,0.0,(void *)font,s);
    

//paint CALIBRATING LINES
glBegin(GL_LINES);
glColor3f(1.0,1.0,1.0);
//up
glVertex3f(+10,winy,0.0);
glVertex3f(-10,winy,0.0);
//down
glVertex3f(+10,-winy,0.0);
glVertex3f(-10,-winy,0.0);
//left
glVertex3f(-winx,+10,0.0);
glVertex3f(-winx,+10,0.0);
//right
glVertex3f(+winx,-10,0.0);
glVertex3f(+winx,+10,0.0);
glEnd();

    glEndList();
    return(list); 
*/
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
    GUI=PaintGUI(); //vykresli GUI
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

   glViewport (0, 0, width, height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   if (ratio > 1)
	  glFrustum (-ratio, ratio, -1, 1, 3.0, 200.0);
   else
	  glFrustum (-1, 1, -1 / ratio, 1 / ratio, 3.0, 200.0);
   gluLookAt (x, 0, 0, 0, 0, -depth, 0, 1, 0);

   glMatrixMode (GL_MODELVIEW);
   build_rotmatrix (matrix, curquat);
   glTranslatef (0, 0, -depth);
   glMultMatrixf (&matrix[0][0]);

}

void PassiveMotion(int x, int y) {
/*
//mouse in fullscreen coordinates screen-> <0,1>
int w = glutGet (GLUT_SCREEN_WIDTH);
int h = glutGet (GLUT_SCREEN_HEIGHT);
int wx = glutGet (GLUT_WINDOW_X);
int wy = glutGet (GLUT_WINDOW_Y);
float x=((mx+wx)/w);
float y=1-((my+wy)/h);
*/
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

//   float w = glutGet (GLUT_SCREEN_WIDTH) / 2;
//   float h = glutGet (GLUT_SCREEN_HEIGHT) / 2;

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

/*
if (my<0.15){
   //Selekce densTOT
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)firstsamp=floor(mx*nsamples);
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)lastsamp=floor(mx*nsamples);
   if(firstsamp>firstsamp){int temp=firstsamp; firstsamp=lastsamp; lastsamp=temp;};
   CompValues();
   MOLECULE=MakeMol();
   GUI=PaintGUI();
   };
*/
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
GUI=PaintGUI();
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

//Debuging file 
debug=fopen("debug.log","w"); 
//Loading Data
   if (argc>1) LoadBas(argv[1]);
   else LoadBas("answer.bas");
fclose(debug);

   glClearColor (0, 0, 0, 0);
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
  
   glutMainLoop ();
   return 0;
}
