/****************************************************************************

  example2.cpp

  A simple GLUT program using the GLUI_Context *User Interface Library

  -----------------------------------------------------------------------
	   
  9/9/98 Paul Rademacher (rademach@cs.unc.edu)

****************************************************************************/

#include <string.h>
#include <GL/glui.h>
#include <GL/glui/TransferFunction.h>

using namespace glui;

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
int   main_window;


/** These are the live variables passed into GLUI_Context ****/
int   wireframe = 0;
int   obj = 0;
int   segments = 8;

// Using a std::string as a live variable is safe.
std::string text = "Hello World!";

// Using a char buffer as a live var is also possible, but it is dangerous 
// because GLUI_Context *doesn't know how big your buffer is.  
// But still, it works as long as text doesn't happen to overflow.
//char  text[200] = {"Hello World!"};

GLUI_Checkbox   *checkbox;
GLUI_Spinner    *spinner;
GLUI_RadioGroup *radio;
GLUI_EditText   *edittext;

/**************************************** control_cb() *******************/
/* GLUI_Context *control callback                                                 */

void control_cb( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'  
    ********************************************************************/

  printf( "callback: %d\n", control );
  printf( "             checkbox: %d\n", checkbox->get_int_val() );
  printf( "              spinner: %d\n", spinner->get_int_val() );
  printf( "          radio group: %d\n", radio->get_int_val() );
  printf( "                 text: %s\n", edittext->get_text() );
  
}

void xf_cb( int // control
            )
{
  printf("Transfer function changed...\n");
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
    // A few keys here to test the sync_live capability.
  case 'o':
    // Cycle through object types
    ++obj %= 3;
    GLUI_Master.sync_live_all();
    break;
  case 'w':
    // Toggle wireframe mode
    wireframe = !wireframe;
    GLUI_Master.sync_live_all();
    break;
  case 27: 
  case 'q':
    exit(0);
    break;
  };
  glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
  if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
    last_x = x;
    last_y = y;
  }
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
  rotationX += (float) (y - last_y);
  rotationY += (float) (x - last_x);

  last_x = x;
  last_y = y;

  glutPostRedisplay(); 
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
  xy_aspect = (float)x / (float)y;
  glViewport( 0, 0, x, y );

  glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef( 0.0f, 0.0f, -1.6f );
  glRotatef( rotationY, 0.0, 1.0, 0.0 );
  glRotatef( rotationX, 1.0, 0.0, 0.0 );

  /*** Now we render object, using the variables 'obj', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently 
    updated by GLUI_Context ****/
  
  if ( obj == 0 ) {
    if ( wireframe )      
      glutWireSphere( .6, segments, segments );
    else                  
      glutSolidSphere( .6, segments, segments );
  }
  else if ( obj == 1 ) {
    if ( wireframe )
      glutWireTorus( .2,.5,16,segments );
    else
      glutSolidTorus( .2,.5,16,segments );
  }
  else if ( obj == 2 ) {
    if ( wireframe )
      glutWireTeapot( .5 );
    else
      glutSolidTeapot( .5 );
  }

  glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glColor3ub( 0, 0, 0 );
  glRasterPos2i( 10, 10 );

  //  printf( "text: %s\n", text );

  /*** Render the live character array 'text' ***/
  for (unsigned int i=0; i<text.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, text[i] );

  glEnable( GL_LIGHTING );

  glutSwapBuffers(); 
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( 300, 300 );
 
  main_window = glutCreateWindow( "GLUI_Context *Example 2" );
  glutDisplayFunc( myGlutDisplay );
  glutReshapeFunc( myGlutReshape );  
  glutKeyboardFunc( myGlutKeyboard );
  glutMotionFunc( myGlutMotion );
  glutMouseFunc( myGlutMouse );

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
  GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
  GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI_Context *code         */
  /****************************************/

  GLUI_Context *context = GLUI_Master.create_glui( "GLUI", 0, 400, 50 );
  new GLUI_StaticText( context, "GLUI Example 2 a" );

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
  
  const int numXfAlpha = 128;
  float xfAlpha[numXfAlpha];
  new GLUI_TransferFunction( context, "XF", xfAlpha, numXfAlpha, -1,xf_cb );
  new GLUI_StaticText( context, "GLUI Example 2 b" );
  new GLUI_Separator(context);
  checkbox = new GLUI_Checkbox( context, "Wireframe", &wireframe, 1, control_cb );
  spinner  = new GLUI_Spinner( context, "Segments:", &segments, 2, control_cb );
  spinner->set_int_limits( 3, 60 );
  edittext = new GLUI_EditText( context, "Text:", text, 3, control_cb );
  GLUI_Panel *obj_panel = new GLUI_Panel( context, "Object Type" );
  radio = new GLUI_RadioGroup( obj_panel,&obj,4,control_cb );
  new GLUI_RadioButton( radio, "Sphere" );
  new GLUI_RadioButton( radio, "Torus" );
  new GLUI_RadioButton( radio, "Teapot" );
  new GLUI_Button( context, "Quit", 0,(GLUI_Update_CB)exit );
  GLUI_Rotation *lights_rot = new GLUI_Rotation(context, "Blue Light", lights_rotation );
  lights_rot->set_spin( .82 );
 
  context->set_main_gfx_window( main_window );

  GLUI_Master.set_glutIdleFunc( NULL );

  glutMainLoop();

  return EXIT_SUCCESS;
}
