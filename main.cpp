#include <utility>
#include <cstddef>
#include "player.h"

VAO *triangle, *rectangle;

vector<Cuboid*> Cuboids;
Cuboid* curCuboid;

Cuboid base;
Player player;

struct Mesh{
	float xpos,xneg;
	float ypos,yneg;
	float zup;
};
typedef struct Mesh Mesh;

Mesh Meshes[10][10];

void makeBase(){
	base.setFillMode(GL_FILL);
	base.callCreate3DTexturedObject(textureID1);
	base.scaleCuboid(40.0f,40.0f,1.0f);
	base.translateCuboid(0.0f,0.0f,-1.0f);
}

void makeHundredCuboids(){
	unsigned int p=0;
	unsigned int xs = 0,ys = 0;
	for(int i = -10; i < 10; i+=2){
		for(int j = -10; j < 10; j+=2){
			  curCuboid = new Cuboid();
			  Cuboids.push_back(curCuboid);
			  if(rand()%3 == 1){
			  	Cuboids[p]->active = false;
			  }
			  Cuboids[p]->setFillMode(GL_FILL);
			  //Cuboids[0]->callCreate3DObject();
			  Cuboids[p]->callCreate3DTexturedObject(textureID2);
			  
			  
			  //call in correct order
			  Cuboids[p]->scaleCuboid(1,1,1);
			  //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);
			  Cuboids[p]->translateCuboid(float(i),float(j),2.0f);
			  //Repeatedly call the above for a set of transformations

			  
			  Meshes[xs][ys].xneg = i-1;
			  Meshes[xs][ys].xpos = i+1;
			  Meshes[xs][ys].yneg = j-1;
			  Meshes[xs][ys].ypos = j+1;
			  Meshes[xs][ys].zup = 3.5;
			  
			  //cout<<xs<<" "<<ys<<" "<<Meshes[xs][ys].xneg<<" "<<Meshes[xs][ys].xpos<<" "<<Meshes[xs][ys].yneg<<" "<<Meshes[xs][ys].ypos<<endl;
			  //fflush(stdout);

			  ys++;
			  p++;
		}
		xs++;
		ys = 0;
	}
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  glm::mat4 MVP;


  // Eye - Location of camera. Don't change unless you are sure!!
  //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  glm::vec3 eye(0+topviewcamlateral, towerview+topviewcamupdown, 20+topviewcamval);//-100
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (0, 0, 0);//50 and 0
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

  /*FOr top view
  // Eye - Location of camera. Don't change unless you are sure!!
  //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  glm::vec3 eye(0+topviewcamlateral, 0+topviewcamupdown, 20+topviewcamval);//-100
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (0, 0, 0);//50 and 0
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);
  */

  /*
  // Compute Camera matrix (view)
  Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
  //  Don't change unless you are sure!!
  //Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

  // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
  //  Don't change unless you are sure!!
  glm::mat4 VP = Matrices.projection * Matrices.view;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  //  Don't change unless you are sure!!
  glm::mat4 MVP;	// MVP = Projection * View * Model

  // Load identity to model matrix
  Matrices.model = glm::mat4(1.0f);

  // Render your scene

  glm::mat4 translateTriangle = glm::translate (glm::vec3(-2.0f, 0.0f, 0.0f)); // glTranslatef
  glm::mat4 rotateTriangle = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
  glm::mat4 triangleTransform = translateTriangle * rotateTriangle;
  Matrices.model *= triangleTransform; 
  MVP = VP * Matrices.model; // MVP = p * V * M

  //  Don't change unless you are sure!!
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix
  draw3DObject(triangle);
  */
  //Cuboid
  
  glUseProgram(textureProgramID);

  Matrices.view = glm::lookAt(eye, target, up );
  
  for(unsigned int k=0 ; k < 100; k++){
  	//Cuboids[k]->setViewMatrix(eye,target,up);
  	if(Cuboids[k]->active == false)
  		continue;
  	if(Cuboids[k]->getCollapse() && !Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 1){
  		Cuboids[k]->translateCuboid(0.0f,0.0f,-1.0f);
  		Meshes[int(k/10)][int(k%10)].zup -= (2*1);
  		Cuboids[k]->setRestore(true);
  		Cuboids[k]->setDepth(-1);
  	}
  	else if(Cuboids[k]->getCollapse() && !Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 0){
  		Cuboids[k]->translateCuboid(0.0f,0.0f,1.0f);
  		Meshes[int(k/10)][int(k%10)].zup += (2*1);
  		Cuboids[k]->setRestore(true);
  		Cuboids[k]->setDepth(1);
  	}

  	Cuboids[k]->setModelMatrixIdentity();
  	Cuboids[k]->transformCuboidAndSetModel();
  	Cuboids[k]->setMVPMatrix();

  	if(Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 1){
  		if(Cuboids[k]->getInc() != 1000){
  			Cuboids[k]->translateCuboid(0.0f,0.0f,0.001f);
  			Meshes[int(k/10)][int(k%10)].zup += (2*0.001);
  			Cuboids[k]->setInc(Cuboids[k]->getInc()+1);
  			Cuboids[k]->setDepth(Cuboids[k]->getDepth()+0.001f);
  		}
  		else{
  			Cuboids[k]->setCollapse(false);
  			Cuboids[k]->setRestore(false);
  			Cuboids[k]->setInc(0);
  			Cuboids[k]->setUp(2);
  		}
  	}
  	else if(Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 0){
  		if(Cuboids[k]->getInc() != 1000){
  			Cuboids[k]->translateCuboid(0.0f,0.0f,-0.001f);
  			Meshes[int(k/10)][int(k%10)].zup -= (2*0.001);
  			Cuboids[k]->setInc(Cuboids[k]->getInc()+1);
  			Cuboids[k]->setDepth(Cuboids[k]->getDepth()-0.001f);
  		}
  		else{
  			Cuboids[k]->setCollapse(false);
  			Cuboids[k]->setRestore(false);
  			Cuboids[k]->setInc(0);
  			Cuboids[k]->setUp(2);
  		}
  	}
	
  	MVP = Cuboids[k]->getMVPMatrix();

  	//glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

  	// Set the texture sampler to access Texture0 memory
  	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

  	draw3DTexturedObject(Cuboids[k]->getCuboidVAO());
  }
  
  	base.setModelMatrixIdentity();
  	base.transformCuboidAndSetModel();
  	base.setMVPMatrix();

  	MVP = base.getMVPMatrix();

  	//glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

  	// Set the texture sampler to access Texture0 memory
  	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

  	draw3DTexturedObject(base.getCuboidVAO());

  	//Player
  	glUseProgram (programID);
  	if(ups){
  		if(player.y + 0.25 < player.ypos){
  			player.translateCuboid(0.0f,0.25f,0.0f);
  			player.y+=0.25;
  		}
  		ups = false;
  	}
  	else if(downs){
  		if(player.y - 0.25 > player.yneg){
  			player.translateCuboid(0.0f,-0.25f,0.0f);
  			player.y-=0.25;
  		}
  		downs = false;
  	}
  	else if(lefts){
  		if(player.x - 0.25 > player.xneg){
  			player.translateCuboid(-0.25f,0.0f,0.0f);
  			player.x-=0.25;
  		}
  		lefts = false;
  	}
  	else if(rights){
  		if(player.x + 0.25 < player.xpos){
  			player.translateCuboid(+0.25f,0.0f,0.0f);
  			player.x+=0.25;
  		}
  		rights = false;
  	}

  	player.translateCuboid(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);

 	  player.setModelMatrixIdentity();
  	player.transformCuboidAndSetModel();
  	player.setMVPMatrix();

  	MVP = player.getMVPMatrix();
  	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  	draw3DObject(player.getCuboidVAO());

  	player.translateCuboid(0.0f,0.0f,-(Meshes[player.getboxx()][player.getboxy()].zup));
  	//Player done
 
  // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
  // glPopMatrix ();
  /*
  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(2, 0, 0));        // glTranslatef
  glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateRectangle * rotateRectangle);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix
  draw3DObject(rectangle);

  // Increment angles
  float increments = 1;

  //camera_rotation_angle++; // Simulating camera rotation
  triangle_rotation = triangle_rotation + increments*triangle_rot_dir*triangle_rot_status;
  rectangle_rotation = rectangle_rotation + increments*rectangle_rot_dir*rectangle_rot_status;
  */
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
	// Load Textures
	// Enable Texture0 as current texture memory
	glActiveTexture(GL_TEXTURE0);

	textureID1 = createTexture("water.png");
	textureID2 = createTexture("cubes.png");
	textureID3 = createTexture("grass.png");

	if(textureID1 == 0 || textureID2 == 0 || textureID3 == 0 )
		cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

	// Create and compile our GLSL program from the texture shaders
	textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.TexMatrixID = glGetUniformLocation(textureProgramID, "MVP");

    /* Objects should be created before any other gl function and shaders */
	// Create the models
	triangle = createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
	rectangle = createRectangle ();
	
	makeBase();
	makeHundredCuboids();
	player.makePlayer();

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

	
	//reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
	srand(time(NULL));
	int width = 1024;
	int height = 768;

    GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // OpenGL Draw commands
        draw();

        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();

        reshapeWindow(window,width,height);
        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 2) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
            //camera_rotation_angle+=5;
   
            int temp;
            if(!Cuboids[temp=rand()%100]->getCollapse() && Cuboids[temp]->getUp() == 2){
            	Cuboids[temp]->setCollapse(true);
            	Cuboids[temp]->setUp(rand()%2);
            }
            last_update_time = current_time;
        }
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}