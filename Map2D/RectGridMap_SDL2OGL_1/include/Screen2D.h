
class Screen2D{
	public:
	int   WIDTH;
	int   HEIGHT;
	float VIEW_DEPTH;
	float ASPECT_RATIO;
	float zoom;

	int   mouseX, mouseY;
	float mouse_begin_x ;
	float mouse_begin_y ;

	bool hasFocus;
	SDL_Window*      window;
	SDL_Renderer*    renderer;

// ============ Setup functions

	void initRenderer();
	void setupRenderer();

	void setDefaults(){
		VIEW_DEPTH   = VIEW_DEPTH_DEFAULT;
		ASPECT_RATIO = WIDTH/(float)HEIGHT;
		zoom = VIEW_ZOOM_DEFAULT;
		printf(" %f %f %f \n", zoom, ASPECT_RATIO, VIEW_DEPTH  );
		mouse_begin_x  = 0;
		mouse_begin_y  = 0;
	}

	Screen2D( int& id, int WIDTH_, int HEIGHT_ ){
		WIDTH  = WIDTH_;
		HEIGHT = HEIGHT_;
		setDefaults();
		SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);
		id = SDL_GetWindowID(window); printf( " win id %i \n", id );
		char str[40];  sprintf(str, " Window id = %d", id );
		SDL_SetWindowTitle( window, str );

		setupRenderer();
		printf( " ASPECT_RATIO %f \n", ASPECT_RATIO );
	}

// =========== control Handling

	float mouseUp     ( float mY ){ return 2*zoom*( 0.5 -mY/float(HEIGHT)                    ); };
	float mouseRight  ( float mX ){ return 2*zoom*(      mX/float(HEIGHT) - 0.5*ASPECT_RATIO ); };

// ============ per frame functions

	void camera();

	void draw();
	void projectMouse();
	void inputHanding();
	void getCameraDirections();

	void update( ){
		//SDL_RenderPresent(renderer);
		camera();
		glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		draw();
		SDL_RenderPresent(renderer);
	};

};


// ============ Setup ops

void Screen2D::setupRenderer(){
	float ambient  [] = { 0.1f, 0.15f, 0.25f, 1.0f };
	float diffuse  [] = { 0.9f, 0.8f,  0.7f,  1.0f };
	float specular [] = { 1.0f, 1.0f,  1.0f,  1.0f };
	float shininess[] = { 80.0f                    };
	float lightPos [] = { 1.0f, 1.0f, +1.0f, 0.0f  };
	glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glEnable     ( GL_COLOR_MATERIAL    );
	glLightfv    ( GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv    ( GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable     ( GL_LIGHTING         );
	glEnable     ( GL_LIGHT0           );
	glEnable     ( GL_NORMALIZE        );
	glEnable     ( GL_DEPTH_TEST       );
	glHint       ( GL_LINE_SMOOTH_HINT, GL_NICEST );	
	glShadeModel ( GL_SMOOTH           );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


// ================ PER FRAME OPS

void Screen2D::draw(){
	glEnable (GL_LIGHTING);
	glShadeModel(GL_FLAT);
	glColor3f( 0.2f, 0.8f, 0.2f );

/*
	printf( " %i \n", frameCount );
	Vec2f p00,p01,p10,p11;
	p00.set( -1.0,-1.0 );
	p01.set( +1.0,-1.0 );
	p10.set( -1.0,+1.0 );
	p11.set( +1.0,+1.0 );
	glBegin( GL_TRIANGLES );
		//render_2( p00, p01, p10, p11 );
		//render_block( p00, p01, p10, p11, frameCount&0xF );
	glEnd();
*/	

	if ( tile->render_list > 0 ){
		//printf( "drawing render_list %i \n", tile->render_list );
		glCallList( tile->render_list );
	}
	glDisable  (GL_LIGHTING);
	drawAxis( 10 );
}


void Screen2D::camera(){
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho ( -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -zoom, zoom, -VIEW_DEPTH, +VIEW_DEPTH );
	//glOrtho ( -zoom, zoom, -zoom, zoom, -VIEW_DEPTH, +VIEW_DEPTH );
	//glOrtho ( -zoom, zoom, -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -VIEW_DEPTH, +VIEW_DEPTH );
	glMatrixMode (GL_MODELVIEW);
	
}





