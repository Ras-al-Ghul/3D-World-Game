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

#include "misc.h"

class Cuboid{
	public:
		bool active;
		Cuboid();
		GLfloat* getVertexBuffer();
		GLfloat* getColorBuffer();
		GLfloat* getTextureBuffer();
		void setFillMode(GLenum newmode);
		void setDefaultVertices();
		void setDefaultColor();
		void setDefaultTexture();
		VAO* callCreate3DObject();
		VAO* callCreate3DTexturedObject(GLuint texID);
		GLenum getFillMode();
		void setModelMatrixIdentity();
		void setViewMatrix(glm::vec3 eye,glm::vec3 target,glm::vec3 up);
		void translateCuboid(float x=0.0f,float y=0.0f,float z=0.0f);
		void rotateCuboid(float angInDeg=0.0f,float x=0.0f,float y=0.0f,float z=0.0f);
		void scaleCuboid(float x=1.0f,float y=1.0f,float z=1.0f);
		void transformCuboidAndSetModel();
		void setMVPMatrix();
		glm::mat4 getMVPMatrix();
		VAO* getCuboidVAO();
		void setCollapse(bool val);
		bool getCollapse();
		void setRestore(bool val);
		bool getRestore();
		void setInc(int val);
		int getInc();
		void setUp(int val);
		int getUp();
		void setDepth(float val);
		float getDepth();
		virtual ~Cuboid();
	protected:
	private:
		GLenum primitive_mode;
		int numVertices;
		GLfloat* vertex_buffer_data;
		GLfloat* color_buffer_data;
		GLfloat* texture_buffer_data;
		GLenum fill_mode;
		VAO* cubevao;
		glm::mat4 MVP;
		glm::mat4 translate;
		glm::mat4 rotate;
		glm::mat4 scale;
		bool collapse;
		bool restore;
		int up;//0 from up to down 1 from down to up 2 still
		int inc;
		float currentDepth;
};

Cuboid::Cuboid(){
	primitive_mode = GL_TRIANGLES;
	numVertices = 12 * 3;
	vertex_buffer_data = new GLfloat[3*numVertices];
	color_buffer_data = new GLfloat[3*numVertices];
	texture_buffer_data = new GLfloat[2*numVertices];
	fill_mode = GL_FILL;
	setDefaultVertices();
	setDefaultColor();
	setDefaultTexture();
	translate = glm::mat4(1.0f);
	rotate = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);
	collapse = false;
	restore = false;
	inc = 0;
	up = 2;
	currentDepth = 0.0f;
	active = true;
}

void Cuboid::setDepth(float val){
	currentDepth = val;
}

float Cuboid::getDepth(){
	return currentDepth;
}

void Cuboid::setUp(int val){
	up = val;
}

int Cuboid::getUp(){
	return up;
}

void Cuboid::setInc(int val){
	inc = val;
}

int Cuboid::getInc(){
	return inc;
}

void Cuboid::setCollapse(bool val){
	collapse = val;
}

bool Cuboid::getCollapse(){
	return collapse;
}

void Cuboid::setRestore(bool val){
	restore = val;
}

bool Cuboid::getRestore(){
	return restore;
}

GLfloat* Cuboid::getVertexBuffer(){
	return vertex_buffer_data;
}

GLfloat* Cuboid::getColorBuffer(){
	return color_buffer_data;
}

GLfloat* Cuboid::getTextureBuffer(){
	return texture_buffer_data;
}

void Cuboid::setDefaultVertices(){
	vertex_buffer_data[0]  = -1.0f;  vertex_buffer_data[1] = -1.0f;  vertex_buffer_data[2] = -1.0f;
	vertex_buffer_data[3]  = -1.0f;  vertex_buffer_data[4] = -1.0f;  vertex_buffer_data[5] = 1.0f;
	vertex_buffer_data[6]  = -1.0f;  vertex_buffer_data[7] = 1.0f;   vertex_buffer_data[8] = 1.0f;
	vertex_buffer_data[9]  = 1.0f;   vertex_buffer_data[10] = 1.0f;  vertex_buffer_data[11] = -1.0f;
	vertex_buffer_data[12] = -1.0f;  vertex_buffer_data[13] = -1.0f; vertex_buffer_data[14] = -1.0f;
	vertex_buffer_data[15] = -1.0f;  vertex_buffer_data[16] = 1.0f;  vertex_buffer_data[17] = -1.0f;
	vertex_buffer_data[18] = 1.0f;   vertex_buffer_data[19] = -1.0f; vertex_buffer_data[20] = 1.0f;
	vertex_buffer_data[21] = -1.0f;  vertex_buffer_data[22] = -1.0f; vertex_buffer_data[23] = -1.0f;
	vertex_buffer_data[24] = 1.0f;   vertex_buffer_data[25] = -1.0f; vertex_buffer_data[26] = -1.0f;
	vertex_buffer_data[27] = 1.0f;	 vertex_buffer_data[28] = 1.0f;	 vertex_buffer_data[29] = -1.0f;
	vertex_buffer_data[30] = 1.0f; 	 vertex_buffer_data[31] = -1.0f; vertex_buffer_data[32] = -1.0f;
	vertex_buffer_data[33] = -1.0f;	 vertex_buffer_data[34] = -1.0f; vertex_buffer_data[35] = -1.0f;
	vertex_buffer_data[36] = -1.0f;	 vertex_buffer_data[37] = -1.0f; vertex_buffer_data[38] = -1.0f;
	vertex_buffer_data[39] = -1.0f;	 vertex_buffer_data[40] = 1.0f;	 vertex_buffer_data[41] = 1.0f;
	vertex_buffer_data[42] = -1.0f;	 vertex_buffer_data[43] = 1.0f;	 vertex_buffer_data[44] = -1.0f;
	vertex_buffer_data[45] = 1.0f;	 vertex_buffer_data[46] = -1.0f; vertex_buffer_data[47] = 1.0f;
	vertex_buffer_data[48] = -1.0f;	 vertex_buffer_data[49] = -1.0f; vertex_buffer_data[50] = 1.0f;
	vertex_buffer_data[51] = -1.0f;	 vertex_buffer_data[52] = -1.0f; vertex_buffer_data[53] = -1.0f;
	vertex_buffer_data[54] = -1.0f;	 vertex_buffer_data[55] = 1.0f;	 vertex_buffer_data[56] = 1.0f;
	vertex_buffer_data[57] = -1.0f;	 vertex_buffer_data[58] = -1.0f; vertex_buffer_data[59] = 1.0f;
	vertex_buffer_data[60] = 1.0f;	 vertex_buffer_data[61] = -1.0f; vertex_buffer_data[62] = 1.0f;
	vertex_buffer_data[63] = 1.0f;	 vertex_buffer_data[64] = 1.0f;	 vertex_buffer_data[65] = 1.0f;
	vertex_buffer_data[66] = 1.0f;	 vertex_buffer_data[67] = -1.0f; vertex_buffer_data[68] = -1.0f;
	vertex_buffer_data[69] = 1.0f;	 vertex_buffer_data[70] = 1.0f;	 vertex_buffer_data[71] = -1.0f;
	vertex_buffer_data[72] = 1.0f;	 vertex_buffer_data[73] = -1.0f; vertex_buffer_data[74] = -1.0f;
	vertex_buffer_data[75] = 1.0f;	 vertex_buffer_data[76] = 1.0f;	 vertex_buffer_data[77] = 1.0f;
	vertex_buffer_data[78] = 1.0f;	 vertex_buffer_data[79] = -1.0f; vertex_buffer_data[80] = 1.0f;
	vertex_buffer_data[81] = 1.0f;	 vertex_buffer_data[82] = 1.0f;	 vertex_buffer_data[83] = 1.0f;
	vertex_buffer_data[84] = 1.0f;	 vertex_buffer_data[85] = 1.0f;	 vertex_buffer_data[86] = -1.0f;
	vertex_buffer_data[87] = -1.0f;	 vertex_buffer_data[88] = 1.0f;	 vertex_buffer_data[89] = -1.0f;
	vertex_buffer_data[90] = 1.0f;	 vertex_buffer_data[91] = 1.0f;	 vertex_buffer_data[92] = 1.0f;
	vertex_buffer_data[93] = -1.0f;	 vertex_buffer_data[94] = 1.0f;	 vertex_buffer_data[95] = -1.0f;
	vertex_buffer_data[96] = -1.0f;	 vertex_buffer_data[97] = 1.0f;	 vertex_buffer_data[98] = 1.0f;
	vertex_buffer_data[99] = 1.0f;	 vertex_buffer_data[100] = 1.0f; vertex_buffer_data[101] = 1.0f;
	vertex_buffer_data[102] = -1.0f; vertex_buffer_data[103] = 1.0f; vertex_buffer_data[104] = 1.0f;
	vertex_buffer_data[105] = 1.0f;	 vertex_buffer_data[106] = -1.0f;vertex_buffer_data[107] = 1.0f;
}

void Cuboid::setDefaultColor(){
	for (int i=0; i<numVertices; i++) {
        color_buffer_data[3*i] = 1.0f;
        color_buffer_data[3*i + 1] = 0.0f;
        color_buffer_data[3*i + 2] = 0.0f;
    }
}

void Cuboid::setDefaultTexture(){
	texture_buffer_data[0] = 0;		texture_buffer_data[1] = 1;
	texture_buffer_data[2] = 1;		texture_buffer_data[3] = 1;
	texture_buffer_data[4] = 1;		texture_buffer_data[5] = 0;
	texture_buffer_data[6] = 0;		texture_buffer_data[7] = 1;
	texture_buffer_data[8] = 1;		texture_buffer_data[9] = 1;
	texture_buffer_data[10] = 1;	texture_buffer_data[11] = 0;
	texture_buffer_data[12] = 0;	texture_buffer_data[13] = 1;
	texture_buffer_data[14] = 1;	texture_buffer_data[15] = 1;
	texture_buffer_data[16] = 1;	texture_buffer_data[17] = 0;
	texture_buffer_data[18] = 0;	texture_buffer_data[19] = 1;
	texture_buffer_data[20] = 1;	texture_buffer_data[21] = 1;
	texture_buffer_data[22] = 1;	texture_buffer_data[23] = 0;
	texture_buffer_data[24] = 0;	texture_buffer_data[25] = 1;
	texture_buffer_data[26] = 1;	texture_buffer_data[27] = 1;
	texture_buffer_data[28] = 1;	texture_buffer_data[29] = 0;
	texture_buffer_data[30] = 0;	texture_buffer_data[31] = 1;
	texture_buffer_data[32] = 1;	texture_buffer_data[33] = 1;
	texture_buffer_data[34] = 1;	texture_buffer_data[35] = 0;
	texture_buffer_data[36] = 0;	texture_buffer_data[37] = 1;
	texture_buffer_data[38] = 1;	texture_buffer_data[39] = 1;
	texture_buffer_data[40] = 1;	texture_buffer_data[41] = 0;
	texture_buffer_data[42] = 0;	texture_buffer_data[43] = 1;
	texture_buffer_data[44] = 1;	texture_buffer_data[45] = 1;
	texture_buffer_data[46] = 1;	texture_buffer_data[47] = 0;
	texture_buffer_data[48] = 0;	texture_buffer_data[49] = 1;
	texture_buffer_data[50] = 1;	texture_buffer_data[51] = 1;
	texture_buffer_data[52] = 1;	texture_buffer_data[53] = 0;
	texture_buffer_data[54] = 0;	texture_buffer_data[55] = 1;
	texture_buffer_data[56] = 1;	texture_buffer_data[57] = 1;
	texture_buffer_data[58] = 1;	texture_buffer_data[59] = 0;
	texture_buffer_data[60] = 0;	texture_buffer_data[61] = 1;
	texture_buffer_data[62] = 1;	texture_buffer_data[63] = 1;
	texture_buffer_data[64] = 1;	texture_buffer_data[65] = 0;
	texture_buffer_data[66] = 0;	texture_buffer_data[67] = 1;
	texture_buffer_data[68] = 1;	texture_buffer_data[69] = 1;
	texture_buffer_data[70] = 1;	texture_buffer_data[71] = 0;
}

void Cuboid::setFillMode(GLenum newmode){
	fill_mode = newmode;
}

VAO* Cuboid::callCreate3DObject(){
	return (cubevao = create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode));
}

VAO* Cuboid::callCreate3DTexturedObject(GLuint texID){
	return (cubevao = create3DTexturedObject(primitive_mode, numVertices, vertex_buffer_data, texture_buffer_data,texID, fill_mode));
}

void Cuboid::setModelMatrixIdentity(){
	Matrices.model = glm::mat4(1.0f);
}

void Cuboid::setViewMatrix(glm::vec3 eye,glm::vec3 target,glm::vec3 up){
	Matrices.view = glm::lookAt(eye, target, up );
}

void Cuboid::translateCuboid(float x,float y,float z){
	translate = glm::translate(translate,glm::vec3(x, y, z));
}

void Cuboid::rotateCuboid(float angInDeg,float x,float y,float z){
	rotate = glm::rotate(rotate,(float)(angInDeg*M_PI/180.0f), glm::vec3(x,y,z));
}

void Cuboid::scaleCuboid(float x,float y,float z){
	scale = glm::scale(scale,glm::vec3(x, y, z));
}

void Cuboid::transformCuboidAndSetModel(){
	Matrices.model = translate*rotate*scale*Matrices.model;
}

void Cuboid::setMVPMatrix(){
	MVP = Matrices.projection * Matrices.view * Matrices.model;
}

glm::mat4 Cuboid::getMVPMatrix(){
	return MVP;
}

VAO* Cuboid::getCuboidVAO(){
	return cubevao;
}

Cuboid::~Cuboid(){

}
