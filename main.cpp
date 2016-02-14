#include <utility>
#include <cstddef>
#include "player.h"

#include <SFML/Audio.hpp>
sf::Sound collision,gold,fall;

bool inits = true,lifeminus = false,isDangerSet = false;
VAO *triangle, *rectangle;

vector<Cuboid*> Cuboids;
Cuboid* curCuboid;

Cuboid base;
Player player;

vector<Obstacle*> obstacles;
Obstacle* curObstacle;

vector<Obstacle*> golds;

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

void makeObstacles(){
  int p = 0;
  for(int i=0;i<(rand()%2+2);i++){
    curObstacle = new Obstacle(false);
    obstacles.push_back(curObstacle);

    obstacles[p]->makeObstacle(2,2,3.5);
    p++;
  }
}

void makeGolds(){
  int p = 0;
  for(int i=0;i<(rand()%2+6);i++){
    curObstacle = new Obstacle(true);
    golds.push_back(curObstacle);

    golds[p]->makeObstacle(2,2,3.5);
    p++;
  }
}

void makeHundredCuboids(){
	unsigned int p=0;
	unsigned int xs = 0,ys = 0;
	for(int i = -10; i < 10; i+=2){
		for(int j = -10; j < 10; j+=2){
			  curCuboid = new Cuboid();
			  Cuboids.push_back(curCuboid);

        if(i == 0 && j == 0)
          Cuboids[p]->isGoal = true;

			  if(rand()%4 == 1 && !Cuboids[p]->isGoal){
			  	Cuboids[p]->active = false;
			  }
			  Cuboids[p]->setFillMode(GL_FILL);
			  //Cuboids[0]->callCreate3DObject();

        if(!Cuboids[p]->isGoal)
          Cuboids[p]->callCreate3DTexturedObject(textureID2);
        else
          Cuboids[p]->callCreate3DTexturedObject(textureID9);
			  
			  //call in correct order
			  Cuboids[p]->scaleCuboid(1,1,1);
			  //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);
			  Cuboids[p]->translateCuboid(float(i),float(j),2.0f);
			  //Repeatedly call the above for a set of transformations
        Cuboids[p]->x = i;
        Cuboids[p]->y = j;

			  
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

  glm::vec3 eye(eyedefaultx+eyex, eyedefaulty+eyey, eyedefaultz+eyez);//-100  

  //glm::vec3 eye(0+topviewcamlateral, towerview+topviewcamupdown, 20+topviewcamval);//-100
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target(targetdefaultx+targetx, targetdefaulty+targety, targetdefaultz+targetz);

  //glm::vec3 target (0, 0, 0);//50 and 0
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

  
  //Cuboid
  
  glUseProgram(textureProgramID);

  Matrices.view = glm::lookAt(eye, target, up );
  
  for(unsigned int k=0 ; k < 100; k++){
  	//Cuboids[k]->setViewMatrix(eye,target,up);

  	if(Cuboids[k]->active == false)
  		continue;
  	if(Cuboids[k]->getCollapse() && !Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 1){


          curCuboid = Cuboids[k];
          int tempx = curCuboid->x, tempy = curCuboid->y;
          delete curCuboid;

          curCuboid = new Cuboid();
          Cuboids[k] = curCuboid;

          Cuboids[k]->isDanger = true;
          Cuboids[k]->active = true;

          Cuboids[k]->setFillMode(GL_FILL);
          //Cuboids[0]->callCreate3DObject();
          Cuboids[k]->callCreate3DTexturedObject(textureID8);
          //call in correct order
          Cuboids[k]->scaleCuboid(1,1,1);
          //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);
          Cuboids[k]->translateCuboid(tempx,tempy,2);
          Cuboids[k]->x = tempx;Cuboids[k]->y = tempy;
          Cuboids[k]->setUp(1);
          Cuboids[k]->setCollapse(true);
          Cuboids[k]->isDanger = true;

      Cuboids[k]->translateCuboid(0.0f,0.0f,-1.0f);
  		Meshes[int(k/10)][int(k%10)].zup -= (/*2**/1);
  		Cuboids[k]->setRestore(true);
  		Cuboids[k]->setDepth(-1);
  	}
  	else if(Cuboids[k]->getCollapse() && !Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 0){

        if(rand()%2){
          curCuboid = Cuboids[k];
          int tempx = curCuboid->x, tempy = curCuboid->y;
          delete curCuboid;

          curCuboid = new Cuboid();
          Cuboids[k] = curCuboid;

          Cuboids[k]->isDanger = true;
          Cuboids[k]->active = true;
          Cuboids[k]->isYellow = true;

          Cuboids[k]->setFillMode(GL_FILL);
          //Cuboids[0]->callCreate3DObject();
          Cuboids[k]->callCreate3DTexturedObject(textureID6);
          //call in correct order
          Cuboids[k]->scaleCuboid(1,1,1);
          //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);
          Cuboids[k]->translateCuboid(tempx,tempy,2);
          Cuboids[k]->x = tempx;Cuboids[k]->y = tempy;
          Cuboids[k]->setUp(0);
          Cuboids[k]->setCollapse(true);
        }

  		Cuboids[k]->translateCuboid(0.0f,0.0f,1.0f);
  		Meshes[int(k/10)][int(k%10)].zup += (/*2**/1);
  		Cuboids[k]->setRestore(true);
  		Cuboids[k]->setDepth(1);
  	}

  	Cuboids[k]->setModelMatrixIdentity();
  	Cuboids[k]->transformCuboidAndSetModel();
  	Cuboids[k]->setMVPMatrix();

  	if(Cuboids[k]->getRestore() && Cuboids[k]->getUp() == 1){
  		if(Cuboids[k]->getInc() != 1000){

  			Cuboids[k]->translateCuboid(0.0f,0.0f,0.001f);
  			Meshes[int(k/10)][int(k%10)].zup += (/*2**/0.001);
  			Cuboids[k]->setInc(Cuboids[k]->getInc()+1);
  			Cuboids[k]->setDepth(Cuboids[k]->getDepth()+0.001f);

        int tempdepth = Cuboids[k]->getDepth(),tempInc = Cuboids[k]->getInc();

        
        if(Cuboids[k]->isDanger && Cuboids[k]->getInc() == 500){
          curCuboid = Cuboids[k];
          int tempx = curCuboid->x, tempy = curCuboid->y;
          delete curCuboid;

          curCuboid = new Cuboid();
          Cuboids[k] = curCuboid;

          Cuboids[k]->active = true;

          Cuboids[k]->setDepth(tempdepth);
          Cuboids[k]->setInc(tempInc);

          Cuboids[k]->setFillMode(GL_FILL);
          //Cuboids[0]->callCreate3DObject();
          Cuboids[k]->callCreate3DTexturedObject(textureID2);
          //call in correct order
          Cuboids[k]->scaleCuboid(1,1,1);
          //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);

          Cuboids[k]->translateCuboid(tempx,tempy,1.5);
          Cuboids[k]->x = tempx;
          Cuboids[k]->y = tempy;
          Cuboids[k]->setUp(1);
          Cuboids[k]->setRestore(true);
          Cuboids[k]->setCollapse(true);
        }
        

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
  			Meshes[int(k/10)][int(k%10)].zup -= (/*2**/0.001);
  			Cuboids[k]->setInc(Cuboids[k]->getInc()+1);
  			Cuboids[k]->setDepth(Cuboids[k]->getDepth()-0.001f);
  		}
  		else{
        
        if(Cuboids[k]->isDanger){
          curCuboid = Cuboids[k];
          int tempx = curCuboid->x, tempy = curCuboid->y;
          delete curCuboid;

          curCuboid = new Cuboid();
          Cuboids[k] = curCuboid;

          Cuboids[k]->active = true;

          Cuboids[k]->setFillMode(GL_FILL);
          //Cuboids[0]->callCreate3DObject();
          Cuboids[k]->callCreate3DTexturedObject(textureID2);
          //call in correct order
          Cuboids[k]->scaleCuboid(1,1,1);
          //Cuboids[0]->rotateCuboid(90.f,0.0f,0.0f,1.0f);
          Cuboids[k]->translateCuboid(tempx,tempy,2);
          Cuboids[k]->x = tempx;
          Cuboids[k]->y = tempy;
          Cuboids[k]->setUp(0);
        }
        
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
  
    /*
    for(int i=9;i>-1;i--){
      for(int j=0;j<10;j++){
        cout<<Meshes[j][i].zup<<" ";
      }
      cout<<endl;
    }
    cout<<endl;
    */

  	base.setModelMatrixIdentity();
  	base.transformCuboidAndSetModel();
  	base.setMVPMatrix();

  	MVP = base.getMVPMatrix();

  	//glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

  	// Set the texture sampler to access Texture0 memory
  	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

  	draw3DTexturedObject(base.getCuboidVAO());

    //Obstacles

    for(unsigned int i=0; i<obstacles.size(); i++){
      MVP = obstacles[i]->handleObstacle();
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DTexturedObject(obstacles[i]->cube.getCuboidVAO());

      if(LessThan(abs(player.x - obstacles[i]->x), 0.2) && LessThan(abs(player.y - obstacles[i]->y), 0.2)){
        if(!lifeminus){
          player.lives--;
          player.score -= 10;
          lifeminus = true;
          collision.play();
        }
      }
        
    }

    //Obstacles done

    //Obstacles

    for(unsigned int i=0; i<golds.size(); i++){
      MVP = golds[i]->handleObstacle();
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DTexturedObject(golds[i]->cube.getCuboidVAO());

      if(LessThan(abs(player.x - golds[i]->x), 0.25) && LessThan(abs(player.y - golds[i]->y), 0.25)){
        player.score += 20;
        gold.play();
      }
    }

    cout<<player.score<<" "<<player.lives<<endl;

    //Obstacles done

  	//Player

    player.z += Meshes[player.getboxx()][player.getboxy()].zup;
    int oldx = player.getboxx(), oldy = player.getboxy();

    //cout<<player.z<<endl;

  	glUseProgram (textureProgramID);

    if(inits){
      int tempx = -5,tempy = -6;
      while(1){
        player.handleKeyboard(tempx,tempy,0);
        tempx --;tempy --;
        if(Cuboids[player.getboxx()*10 + player.getboxy()]->active)
          break;
      }
      player.score = 0;
      player.lives = 3;
      inits = false;
    }

  	if(ups){
  		if(player.y + steplengths < player.ypos){
        if(!Cuboids[player.getboxx()*10 + player.getboxy()+1]->active){
          player.handleKeyboard(0.0f,steplengths,0.0f);
        }
        else if(Meshes[player.getboxx()][player.getboxy()+1].zup <= player.z+0.25)        
  			   player.handleKeyboard(0.0f,steplengths,0.0f);
        else{
          if(Cuboids[player.getboxx()*10 + player.getboxy()+1]->isYellow){
            player.score-=10;
            player.lives--;
            fall.play();
          }
          //cout<<Meshes[player.getboxx()][player.getboxy()].zup<<" "<<player.z<<endl;
          //cout<<"ups "<<Meshes[player.getboxx()][player.getboxy()+1].zup<<endl;
        }
  		}
      else{
        player.handleKeyboard(0.0f,steplengths,0.0f);
        if(player.y > player.ypos){
          cout<<"You fell over!"<<endl;
          exit(0);
        }
      }
  		ups = false;
  	}
  	else if(downs){
  		if(player.y - steplengths > player.yneg){
        if(!Cuboids[player.getboxx()*10 + player.getboxy()-1]->active){
          player.handleKeyboard(0.0f,-steplengths,0.0f);
        }
        else if(Meshes[player.getboxx()][player.getboxy()-1].zup <= player.z+0.25)
  			   player.handleKeyboard(0.0f,-steplengths,0.0f);
        else{
          if(Cuboids[player.getboxx()*10 + player.getboxy()-1]->isYellow){
            player.score-=10;
            player.lives--;
            fall.play();
          }
          //cout<<Meshes[player.getboxx()][player.getboxy()].zup<<" "<<player.z<<endl;
          //cout<<"downs "<<Meshes[player.getboxx()][player.getboxy()-1].zup<<endl;
        }
  		}
      else{
        player.handleKeyboard(0.0f,-steplengths,0.0f);
        if(player.y < player.yneg){
          cout<<"You fell over!"<<endl;
          exit(0);
        }
      }
  		downs = false;
  	}
  	else if(lefts){
  		if(player.x - steplengths > player.xneg){
        if(!Cuboids[(player.getboxx()-1)*10 + player.getboxy()]->active){
          player.handleKeyboard(-steplengths,0.0f,0.0f);
        }
        else if(Meshes[player.getboxx()-1][player.getboxy()].zup <= player.z+0.25)
            player.handleKeyboard(-steplengths,0.0f,0.0f);
        else{
          if(Cuboids[(player.getboxx()-1)*10 + player.getboxy()]->isYellow){
            player.score-=10;
            player.lives--;
            fall.play();
          }
          //cout<<Meshes[player.getboxx()][player.getboxy()].zup<<" "<<player.z<<endl;
          //cout<<"lefts "<<Meshes[player.getboxx()-1][player.getboxy()].zup<<endl;
        }
  		}
      else{
        player.handleKeyboard(-steplengths,0,0.0f);
        if(player.x < player.xneg){
          cout<<"You fell over!"<<endl;
          exit(0);
        }
      }
  		lefts = false;
  	}
  	else if(rights){
  		if(player.x + steplengths < player.xpos){
        if(!Cuboids[(player.getboxx()+1)*10 + player.getboxy()]->active){
          player.handleKeyboard(steplengths,0.0f,0.0f);
        }
        else if(Meshes[player.getboxx()+1][player.getboxy()].zup <= player.z+0.25)
      			player.handleKeyboard(steplengths,0.0f,0.0f);
        else{
          if(Cuboids[(player.getboxx()+1)*10 + player.getboxy()]->isYellow){
            player.score-=10;
            player.lives--;
            fall.play();
          }
          //cout<<Meshes[player.getboxx()][player.getboxy()].zup<<" "<<player.z<<endl;
          //cout<<"rights "<<Meshes[player.getboxx()+1][player.getboxy()].zup<<endl;
        }
  		}
      else{
        player.handleKeyboard(steplengths,0,0.0f);
        if(player.x > player.xpos){
          cout<<"You fell over!"<<endl;
          exit(0);
        }
      }
  		rights = false;
  	}

    if(jump){
      player.handleJump();
    }

    if(!Cuboids[player.getboxx()*10 + player.getboxy()]->isDanger)
      isDangerSet = false;

    if(Cuboids[player.getboxx()*10 + player.getboxy()]->isDanger && !isDangerSet){
      player.score -= 10;
      player.lives--;
      isDangerSet = true;
      fall.play();
    }

    if(!Cuboids[player.getboxx()*10 + player.getboxy()]->active){
      cout<<"You fell into an abyss!"<<endl;
      exit(0);
    }

    player.rotate();

    //cout<<player.x<<" "<<player.y<<" "<<player.z<<endl;

    if(currentCamMode == 2){
      eyedefaultx = player.x;
      eyedefaulty = player.y;
      eyedefaultz = player.z*2;

      //cout<<"eye"<<eyedefaultx+eyex<<" "<<eyedefaulty+eyey<<" "<<eyedefaultz+eyez<<endl;

      targetdefaultx = eyedefaultx+eyex;
      targetdefaulty = eyedefaulty+eyey+10;
      targetdefaultz = 0;

      //cout<<"target"<<targetdefaultx+targetx<<" "<<targetdefaulty+targety<<" "<<targetdefaultz+targetz<<endl;
    }

    if(currentCamMode == 3){
      eyedefaultx = player.x;
      eyedefaulty = player.y - 2;
      eyedefaultz = player.z*2.5;

      //cout<<"eye"<<eyedefaultx+eyex<<" "<<eyedefaulty+eyey<<" "<<eyedefaultz+eyez<<endl;

      targetdefaultx = eyedefaultx+eyex;
      targetdefaulty = eyedefaulty+eyey+10;
      targetdefaultz = 0;

      //cout<<"target"<<targetdefaultx+targetx<<" "<<targetdefaulty+targety<<" "<<targetdefaultz+targetz<<endl;
    }

    MVP = player.handleTorso(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.torso.getCuboidVAO());
    MVP = player.handleHead(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.head.getCuboidVAO());
    MVP = player.handleLeftLeg(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.leftleg.getCuboidVAO());
    MVP = player.handleRightLeg(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.rightleg.getCuboidVAO());
    MVP = player.handleLeftHand(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.lefthand.getCuboidVAO());
    MVP = player.handleRightHand(0.0f,0.0f,Meshes[player.getboxx()][player.getboxy()].zup);
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DTexturedObject(player.righthand.getCuboidVAO());

    player.z -= Meshes[oldx][oldy].zup;
  	//Player done

    if(Cuboids[player.getboxx()*10+player.getboxy()]->isGoal){
      cout<<"Game Over :) Your score is "<<player.score<<endl;
      exit(0);
    }

    if(player.lives == -1){
      cout<<"Game Over :) Your score is "<<player.score<<endl;
      exit(0);
    }
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
    glfwSetScrollCallback(window, scrollButton);

    glfwSetCursorPosCallback(window, cursorPosn); 

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
	textureID3 = createTexture("pant.png");
  textureID4 = createTexture("shirt.png");  
  textureID5 = createTexture("skin.png");
  textureID6 = createTexture("flame.png");
  textureID7 = createTexture("gold.png");
  textureID8 = createTexture("danger.png");
  textureID9 = createTexture("goal.png");

	if(textureID1 == 0 || textureID2 == 0 || textureID3 == 0 || textureID4 == 0 || textureID5 == 0 || textureID6 == 0
   || textureID7 == 0 || textureID8 == 0 || textureID9 == 0)
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

  makeObstacles();
  makeGolds();

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

  sf::SoundBuffer buffer1;
  if(!buffer1.loadFromFile("../collision.wav"))
      return -1;
  collision.setBuffer(buffer1);

  sf::SoundBuffer buffer2;
  if(!buffer2.loadFromFile("../gold.wav"))
      return -1;
  gold.setBuffer(buffer2);

  sf::SoundBuffer buffer3;
  if(!buffer3.loadFromFile("../fall.wav"))
      return -1;
  fall.setBuffer(buffer3);

    GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;
    glfwSetCursorPos(window, 512, 384);
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
   
            if(lifeminus)
              lifeminus = false;

            if(currentCamMode != 4)
              glfwSetCursorPos(window, 512, 384);

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