#include "cuboid.h"

class Player:public Cuboid{
	public:
		float xshift,yshift;
		float x,y,z;
		float xpos,xneg,ypos,yneg,zpos,zneg;
		Player();
		int getboxx();//betw 0 to 9
		int getboxy();//betw 0 to 9
		void setpos();
		void makePlayer();
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
}

void Player::setpos(){
	x = -(xshift);
	y = -(yshift);
}

void Player::makePlayer(){
	setFillMode(GL_FILL);
	callCreate3DObject();
	scaleCuboid(0.5,0.5,0.5);
	translateCuboid(2,2,0);
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