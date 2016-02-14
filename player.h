#include "cuboid.h"

class Player{
	public:
		int score;
		int lives;
		float xshift,yshift;
		float x,y,z;
		float xpos,xneg,ypos,yneg,zpos,zneg;
		float rotationang;
		bool rotToggle;
		float jumpAlt;
		Player();
		int getboxx();//betw 0 to 9
		int getboxy();//betw 0 to 9
		void setpos();
		void makePlayer();
		void rotate();
		void handleJump();
		void handleKeyboard(float xx,float yy,float zz);
		void setTranslationsToBody(float xx,float yy,float zz);
		glm::mat4 handleTorso(float xx,float yy,float zz);
		glm::mat4 handleLeftLeg(float xx,float yy,float zz);
		glm::mat4 handleRightLeg(float xx,float yy,float zz);
		glm::mat4 handleLeftHand(float xx,float yy,float zz);
		glm::mat4 handleRightHand(float xx,float yy,float zz);
		glm::mat4 handleHead(float xx,float yy,float zz);
		Cuboid torso,leftleg,rightleg,lefthand,righthand,head;
		virtual ~Player();
	protected:
	private:
};

Player::Player(){
	xshift = 0;
	yshift = 0;
	x = 0;
	y = 0;
	z = 0;
	xpos = 8.75;
	xneg = -10.75;
	ypos = 8.75;
	yneg = -10.75;
	rotationang = 0;
	rotToggle = true;
	jumpAlt = 0;
	score = 0;
	lives = 3;
}

void Player::handleJump(){
	if(AEqual(jumpAlt,0)){
		jumpAlt = 1;
		handleKeyboard(0,0,jumpAlt);
	}
	else{
		if(AEqual(jumpAlt,0.1)){
			jump = false;
			handleKeyboard(0,0,-0.1);
			jumpAlt = 0;
			return;
		}
		jumpAlt -= 0.1;
		handleKeyboard(0,0,-0.1);
	}
}

void Player::setpos(){
	x = -(xshift);
	y = -(yshift);
}

void Player::handleKeyboard(float xx,float yy,float zz){
	setTranslationsToBody(xx,yy,zz);
  	x+=xx;
  	y+=yy;
  	z+=zz;
}

void  Player::rotate(){
	if(isMoving){
		if(rotationang < 30 && rotToggle){
			rotationang += 5;
			lefthand.rotateCuboid(5,1,0,0);
			righthand.rotateCuboid(-5,1,0,0);
			leftleg.rotateCuboid(-5,1,0,0);
			rightleg.rotateCuboid(5,1,0,0);
		}
		else if(rotationang >= 30 && rotToggle)
			rotToggle = false;
		else if(rotationang > -30 && !rotToggle){
			rotationang -= 5;
			lefthand.rotateCuboid(-5,1,0,0);
			righthand.rotateCuboid(5,1,0,0);
			leftleg.rotateCuboid(5,1,0,0);
			rightleg.rotateCuboid(-5,1,0,0);	
		}
		else if(rotationang <= -30 && !rotToggle)
			rotToggle = true;
	}
	else if(!isMoving && rotationang > 0){
		lefthand.rotateCuboid(-rotationang,1,0,0);
		righthand.rotateCuboid(rotationang,1,0,0);
		leftleg.rotateCuboid(rotationang,1,0,0);
		rightleg.rotateCuboid(-rotationang,1,0,0);
		rotationang = 0;
		rotToggle = true;
	}
	else if(!isMoving && rotationang < 0){
		lefthand.rotateCuboid(-rotationang,1,0,0);
		righthand.rotateCuboid(rotationang,1,0,0);
		leftleg.rotateCuboid(rotationang,1,0,0);
		rightleg.rotateCuboid(-rotationang,1,0,0);
		rotationang = 0;
		rotToggle = false;	
	}
}

void Player::setTranslationsToBody(float xx,float yy,float zz){
	torso.translateCuboid(xx,yy,zz);
	head.translateCuboid(xx,yy,zz);
	leftleg.translateCuboid(xx,yy,zz);
	rightleg.translateCuboid(xx,yy,zz);
	lefthand.translateCuboid(xx,yy,zz);
	righthand.translateCuboid(xx,yy,zz);
}

glm::mat4 Player::handleTorso(float xx,float yy,float zz){
	torso.translateCuboid(xx,yy,zz);
	torso.setModelMatrixIdentity();
	torso.transformCuboidAndSetModel();
  	torso.setMVPMatrix();
  	torso.translateCuboid(-xx,-yy,-zz);
  	return torso.getMVPMatrix();
}

glm::mat4 Player::handleHead(float xx,float yy,float zz){
	head.translateCuboid(xx,yy,zz);
	head.setModelMatrixIdentity();
	head.transformCuboidAndSetModel();
  	head.setMVPMatrix();
  	head.translateCuboid(-xx,-yy,-zz);
  	return head.getMVPMatrix();
}

glm::mat4 Player::handleLeftLeg(float xx,float yy,float zz){
	leftleg.translateCuboid(xx,yy,zz);
	leftleg.setModelMatrixIdentity();
	leftleg.transformCuboidAndSetModel();
  	leftleg.setMVPMatrix();
  	leftleg.translateCuboid(-xx,-yy,-zz);
  	return leftleg.getMVPMatrix();
}

glm::mat4 Player::handleRightLeg(float xx,float yy,float zz){
	rightleg.translateCuboid(xx,yy,zz);
	rightleg.setModelMatrixIdentity();
	rightleg.transformCuboidAndSetModel();
  	rightleg.setMVPMatrix();
  	rightleg.translateCuboid(-xx,-yy,-zz);
  	return rightleg.getMVPMatrix();
}

glm::mat4 Player::handleLeftHand(float xx,float yy,float zz){
	lefthand.translateCuboid(xx,yy,zz);
	lefthand.setModelMatrixIdentity();
	lefthand.transformCuboidAndSetModel();
  	lefthand.setMVPMatrix();
  	lefthand.translateCuboid(-xx,-yy,-zz);
  	return lefthand.getMVPMatrix();
}

glm::mat4 Player::handleRightHand(float xx,float yy,float zz){
	righthand.translateCuboid(xx,yy,zz);
	righthand.setModelMatrixIdentity();
	righthand.transformCuboidAndSetModel();
  	righthand.setMVPMatrix();
  	righthand.translateCuboid(-xx,-yy,-zz);
  	return righthand.getMVPMatrix();
}

void Player::makePlayer(){
	torso.setFillMode(GL_FILL);
	torso.callCreate3DTexturedObject(textureID4);
	torso.scaleCuboid(0.40,0.2,0.5);
	torso.translateCuboid(2,2,1.25);

	head.setFillMode(GL_FILL);
	head.callCreate3DTexturedObject(textureID5);
	head.scaleCuboid(0.2,0.2,0.25);
	head.translateCuboid(2,2,2);

	leftleg.setFillMode(GL_FILL);
	leftleg.callCreate3DTexturedObject(textureID3);
	leftleg.scaleCuboid(0.2,0.2,0.6);
	leftleg.translateCuboid(1.75,2,0.25);

	rightleg.setFillMode(GL_FILL);
	rightleg.callCreate3DTexturedObject(textureID3);
	rightleg.scaleCuboid(0.2,0.2,0.6);
	rightleg.translateCuboid(2.25,2,0.25);

	lefthand.setFillMode(GL_FILL);
	lefthand.callCreate3DTexturedObject(textureID5);
	lefthand.scaleCuboid(0.15,0.15,0.45);
	lefthand.translateCuboid(1.5,2,1.25);

	righthand.setFillMode(GL_FILL);
	righthand.callCreate3DTexturedObject(textureID5);
	righthand.scaleCuboid(0.15,0.15,0.45);
	righthand.translateCuboid(2.5,2,1.25);

	xshift = -2;
	yshift = -2;
	setpos();
}

int Player::getboxx(){
	if(x>=-11 && x<=-9)
		return 0;
	else if(x>-9 && x<=-7)
		return 1;
	else if(x>-7 && x<=-5)
		return 2;
	else if(x>-5 && x<=-3)
		return 3;
	else if(x>-3 && x<=-1)
		return 4;
	else if(x>-1 && x<=1)
		return 5;
	else if(x>1 && x<=3)
		return 6;
	else if(x>3 && x<=5)
		return 7;
	else if(x>5 && x<=7)
		return 8;
	else if(x>7 && x<=9)
		return 9;
}

int Player::getboxy(){
	if(y>=-11 && y<=-9)
		return 0;
	else if(y>-9 && y<=-7)
		return 1;
	else if(y>-7 && y<=-5)
		return 2;
	else if(y>-5 && y<=-3)
		return 3;
	else if(y>-3 && y<=-1)
		return 4;
	else if(y>-1 && y<=1)
		return 5;
	else if(y>1 && y<=3)
		return 6;
	else if(y>3 && y<=5)
		return 7;
	else if(y>5 && y<=7)
		return 8;
	else if(y>7 && y<=9)
		return 9;
}

Player::~Player(){

}