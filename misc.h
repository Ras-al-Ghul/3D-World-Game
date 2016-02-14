#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <FTGL/ftgl.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#define EPSILON 0.01

using namespace std;

bool AEqual(float a, float b)
{
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool GreaterThan(float a, float b)
{
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool LessThan(float a, float b)
{
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

GLuint textureID1,textureID2,textureID3,textureID4,textureID5,textureID6,textureID7,
        textureID8,textureID9;//1water//2wall//3pant//4shirt//5skin//6flame//7gold//8danger//9goal

int currentCamMode = 0; //0TowerView//1TopView//2AdventurerCam//3FollowCam//4HelicopterCam
float helicamxoff,helicamyoff;
//eye
float eyedefaultx = 0,eyedefaulty = -20,eyedefaultz = 20;
float eyex = 0,eyey = 0,eyez = 0;
//target
float targetdefaultx = 0,targetdefaulty = 0,targetdefaultz = 0;
float targetx = 0,targety = 0,targetz = 0;

float steplengths = 0.20;//steplengths will be 0.20,0.25,0.30

bool isMoving = false;
bool jump = false;

float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;

float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;
    GLuint TextureBuffer;
	GLuint TextureID;

    GLenum PrimitiveMode;// GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY and GL_TRIANGLES_ADJACENCY
    GLenum FillMode;// GL_FILL, GL_LINE
    int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
	GLuint TexMatrixID;
} Matrices;

struct FTGLFont {
	FTFont* font;
	GLuint fontMatrixID;
	GLuint fontColorID;
} GL3Font;

int topviewcamval = 0,topviewcamlateral = 0,topviewcamupdown = 0;
int towerview;
bool ups = false,downs = false,lefts = false,rights = false;

GLuint programID, fontProgramID, textureProgramID;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

glm::vec3 getRGBfromHue (int hue)
{
	float intp;
	float fracp = modff(hue/60.0, &intp);
	float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

	if (hue < 60)
		return glm::vec3(1,x,0);
	else if (hue < 120)
		return glm::vec3(x,1,0);
	else if (hue < 180)
		return glm::vec3(0,1,x);
	else if (hue < 240)
		return glm::vec3(0,x,1);
	else if (hue < 300)
		return glm::vec3(x,0,1);
	else
		return glm::vec3(1,0,x);
}

/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

struct VAO* create3DTexturedObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* texture_buffer_data, GLuint textureID, GLenum fill_mode=GL_FILL)
{
	struct VAO* vao = new struct VAO;
	vao->PrimitiveMode = primitive_mode;
	vao->NumVertices = numVertices;
	vao->FillMode = fill_mode;
	vao->TextureID = textureID;

	// Create Vertex Array Object
	// Should be done after CreateWindow and before any other GL calls
	glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
	glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
	glGenBuffers (1, &(vao->TextureBuffer));  // VBO - textures

	glBindVertexArray (vao->VertexArrayID); // Bind the VAO
	glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
	glVertexAttribPointer(
						  0,                  // attribute 0. Vertices
						  3,                  // size (x,y,z)
						  GL_FLOAT,           // type
						  GL_FALSE,           // normalized?
						  0,                  // stride
						  (void*)0            // array buffer offset
						  );

	glBindBuffer (GL_ARRAY_BUFFER, vao->TextureBuffer); // Bind the VBO textures
	glBufferData (GL_ARRAY_BUFFER, 2*numVertices*sizeof(GLfloat), texture_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
	glVertexAttribPointer(
						  2,                  // attribute 2. Textures
						  2,                  // size (s,t)
						  GL_FLOAT,           // type
						  GL_FALSE,           // normalized?
						  0,                  // stride
						  (void*)0            // array buffer offset
						  );

	return vao;
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

void draw3DTexturedObject (struct VAO* vao)
{
	// Change the Fill Mode for this object
	glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

	// Bind the VAO to use
	glBindVertexArray (vao->VertexArrayID);

	// Enable Vertex Attribute 0 - 3d Vertices
	glEnableVertexAttribArray(0);
	// Bind the VBO to use
	glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

	// Bind Textures using texture units
	glBindTexture(GL_TEXTURE_2D, vao->TextureID);

	// Enable Vertex Attribute 2 - Texture
	glEnableVertexAttribArray(2);
	// Bind the VBO to use
	glBindBuffer(GL_ARRAY_BUFFER, vao->TextureBuffer);

	// Draw the geometry !
	glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Unbind Textures to be safe
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* Create an OpenGL Texture from an image */
GLuint createTexture (const char* filename)
{
	GLuint TextureID;
	// Generate Texture Buffer
	glGenTextures(1, &TextureID);
	// All upcoming GL_TEXTURE_2D operations now have effect on our texture buffer
	glBindTexture(GL_TEXTURE_2D, TextureID);
	// Set our texture parameters
	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering (interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load image and create OpenGL texture
	int twidth, theight;
	unsigned char* image = SOIL_load_image(filename, &twidth, &theight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMaps to use
	SOIL_free_image_data(image); // Free the data read from file after creating opengl texture
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess it up

	return TextureID;
}

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_O:
                eyex++;
                break;
            case GLFW_KEY_P:
                eyex--;
                break;
            case GLFW_KEY_K:
                eyey++;
                break;
            case GLFW_KEY_L:
            	eyey--;
            	break;
            case GLFW_KEY_N:
                eyez++;
                break;
            case GLFW_KEY_M:
            	eyez--;
            	break;


            case GLFW_KEY_U:
                targetx++;
                break;
            case GLFW_KEY_I:
                targetx--;
                break;
            case GLFW_KEY_H:
                targety++;
                break;
            case GLFW_KEY_J:
                targety--;
                break;
            case GLFW_KEY_V:
                targetz++;
                break;
            case GLFW_KEY_B:
                targetz--;
                break;


            case GLFW_KEY_0://towerview
                glfwSetCursorPos(window, 512, 384);
                currentCamMode = 0;
                eyedefaultx = 0;
                eyedefaulty = -20;
                eyedefaultz = 20;

                targetdefaultx = 0;
                targetdefaulty = 0;
                targetdefaultz = 0;

                eyex = 0,eyey = 0,eyez = 0;
                targetx = 0,targety = 0, targetz = 0;
                break;
            case GLFW_KEY_1://topview
                glfwSetCursorPos(window, 512, 384);
                currentCamMode = 1;
                eyedefaultx = 0;
                eyedefaulty = 0;
                eyedefaultz = 20;

                targetdefaultx = 0;
                targetdefaulty = 0;
                targetdefaultz = 0;

                eyex = 0,eyey = 0,eyez = 0;
                targetx = 0,targety = 0, targetz = 0;
            	break;
            case GLFW_KEY_2://adventurercam
                glfwSetCursorPos(window, 512, 384);
                currentCamMode = 2;
                eyedefaultx = 0;
                eyedefaulty = -20;
                eyedefaultz = 20;

                targetdefaultx = 0;
                targetdefaulty = 0;
                targetdefaultz = 0;

                eyex = 0,eyey = 0,eyez = 0;
                targetx = 0,targety = 0, targetz = 0;
                break;
            case GLFW_KEY_3://followcam
                glfwSetCursorPos(window, 512, 384);
                currentCamMode = 3;
                eyedefaultx = 0;
                eyedefaulty = -20;
                eyedefaultz = 20;

                targetdefaultx = 0;
                targetdefaulty = 0;
                targetdefaultz = 0;

                eyex = 0,eyey = 0,eyez = 0;
                targetx = 0,targety = 0, targetz = 0;
                break;
            case GLFW_KEY_4://helicoptercam
                currentCamMode = 4;
                glfwSetCursorPos(window, 512, 384);
                
                helicamxoff = 0,helicamyoff = 0;

                eyedefaultx = 0;
                eyedefaulty = 0;
                eyedefaultz = 30;

                targetdefaultx = 0;
                targetdefaulty = 0;
                targetdefaultz = 0;

                eyex = 0,eyey = 0,eyez = 0;
                targetx = 0,targety = 0, targetz = 0;
                break;


            case GLFW_KEY_F:
                if(steplengths + 0.05 <= 0.3)
                    steplengths += 0.05;
                break;
            case GLFW_KEY_S:
                if(steplengths - 0.05 >= 0.2)
                    steplengths -= 0.05;
                break;


            case GLFW_KEY_UP:
                ups = true;
                isMoving = false;
                break;
            case GLFW_KEY_DOWN:
                downs = true;
                isMoving = false;
                break;
            case GLFW_KEY_LEFT:
                lefts = true;
                isMoving = false;
                break;
            case GLFW_KEY_RIGHT:
                rights = true;
                isMoving = false;
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            case GLFW_KEY_UP:
            	ups = true;
                isMoving = true;
            	break;
            case GLFW_KEY_DOWN:
            	downs = true;
                isMoving = true;
            	break;
            case GLFW_KEY_LEFT:
            	lefts = true;
                isMoving = true;
            	break;
            case GLFW_KEY_RIGHT:
            	rights = true;
                isMoving = true;
            	break;
            case GLFW_KEY_SPACE:
                jump = true;
                break;
            default:
                break;
        }
    }
    else if( action == GLFW_REPEAT){
    	switch (key) {
            case GLFW_KEY_UP:
            	ups = true;
            	break;
            case GLFW_KEY_DOWN:
            	downs = true;
            	break;
            case GLFW_KEY_LEFT:
            	lefts = true;
            	break;
            case GLFW_KEY_RIGHT:
            	rights = true;
            	break;
            

            case GLFW_KEY_O:
                eyex++;
                break;
            case GLFW_KEY_P:
                eyex--;
                break;
            case GLFW_KEY_K:
                eyey++;
                break;
            case GLFW_KEY_L:
                eyey--;
                break;
            case GLFW_KEY_N:
                eyez++;
                break;
            case GLFW_KEY_M:
                eyez--;
                break;


            case GLFW_KEY_U:
                targetx++;
                break;
            case GLFW_KEY_I:
                targetx--;
                break;
            case GLFW_KEY_H:
                targety++;
                break;
            case GLFW_KEY_J:
                targety--;
                break;
            case GLFW_KEY_V:
                targetz++;
                break;
            case GLFW_KEY_B:
                targetz--;
                break;
            default:
                break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
            quit(window);
            break;
		default:
			break;
	}
}

bool isPressedLeft = false;
bool isPressedRight = false;

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if(action == GLFW_PRESS)
                isPressedLeft = true;
            if(action == GLFW_RELEASE)
                isPressedLeft = false;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if(action == GLFW_PRESS)
                isPressedRight = true;
            if(action == GLFW_RELEASE)
                isPressedRight = false;
            break;
        default:
            break;
    }
}

void scrollButton (GLFWwindow* window, double xoffset, double yoffset)
{
    if(currentCamMode == 4){
        eyez-=yoffset;
    }
}

void cursorPosn(GLFWwindow* window, double xcur, double ycur){
    //cout<<xoffset<<" cursor "<<yoffset<<endl;
    if(currentCamMode == 4){
        helicamxoff = xcur - 512;
        helicamyoff = 384 - ycur;

        if(isPressedLeft){
            targetx += (helicamxoff/300);
            targety += (helicamyoff/300);
        }
        if(isPressedRight){
            eyex += (helicamxoff/400);
            eyey += (helicamyoff/400);
        }
    }
    else{
        helicamxoff = xcur - 512;
        helicamyoff = 384 - ycur;

        if(helicamxoff>100)
            rights = true;
        else if(helicamxoff<-100)
            lefts = true;

        if(helicamyoff>100)
            ups = true;
        else if(helicamyoff<-100)
            downs = true;
    }

}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 33.0f;// + topviewcamval;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    //Matrices.projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f);
}

// Creates the triangle object used in this sample code
VAO* createTriangle ()
{
  /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

  /* Define vertex array as used in glBegin (GL_TRIANGLES) */
  static const GLfloat vertex_buffer_data [] = {
    0, 1,0, // vertex 0
    -1,-1,0, // vertex 1
    1,-1,0, // vertex 2
  };

  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 0
    0,1,0, // color 1
    0,0,1, // color 2
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
  return create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
}

// Creates the rectangle object used in this sample code
VAO* createRectangle ()
{
  // GL3 accepts only Triangles. Quads are not supported
  static const GLfloat vertex_buffer_data [] = {
    -1.2,-1,0, // vertex 1
    1.2,-1,0, // vertex 2
    1.2, 1,0, // vertex 3

    1.2, 1,0, // vertex 3
    -1.2, 1,0, // vertex 4
    -1.2,-1,0  // vertex 1
  };

  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 1
    0,0,1, // color 2
    0,1,0, // color 3

    0,1,0, // color 3
    0.3,0.3,0.3, // color 4
    1,0,0  // color 1
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}
