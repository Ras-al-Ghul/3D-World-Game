# 3D-World-Game

This game uses **OpenGl3** and **GLFW**.
The following libraries may need to be installed.

**GLFW:**
 - Install CMake
 - Obtain & Extract the GLFW source code from  
   `https://github.com/glfw/glfw/archive/master.zip`
 - Compile with below commands  
   `cd glfw-master`  
   `mkdir build`  
   `cd build`  
   `cmake -DBUILD_SHARED_LIBS=ON ..`  
   `make && sudo make install`  

**GLAD:**
 - Go to `http://glad.dav1d.de`  
 - `Language: C/C++`  
   `Specification: OpenGL`  
   `gl: Version 4.5`  
   `gles1: Version 1.0`  
   `gles2: Version 3.2`  
   `Profile: Core`  
   Select `Add All` under extensions and click `Generate`.
 - Download the zip file generated.
 - Copy contents of include/ folder in the downloaded directory to `/usr/local/include/`
 - `src/glad.c` should be always compiled along with your OpenGL code

**GLM:**
 - Download the zip file from  
   `https://github.com/g-truc/glm/releases/tag/0.9.7.2`
 - Unzip it and copy the folder `glm/glm/` to `/usr/local/include`

**FTGL:** - Find zip in the folder  
- `cd ftgl3-master`
- `./autogen.sh`
- `./configure`
- `cd src`
- `make && sudo make install`

**freetype2:**
- Download and Install freetype2 library from  
  `http://download.savannah.gnu.org/releases/freetype/freetype-2.6.2.tar.gz`

**SOIL Textures:**
- Download and Install SOIL library from  
  `http://www.lonesock.net/soil.html`

For the audio, **SFML** library is used. Install using:

`sudo apt-get install libsfml-dev`

#### Keyboard Controls

The keyboard controls for the game are:

	UP    - Positive Y  
	DOWN  - Negative Y  
	LEFT  - Negative X  
	RIGHT - Positive X  
	SPACE - Jump  
	f     - Move Faster  
	s     - Move Slower  
	0     - Tower View Cam  
	1     - Top View Cam  
	2     - Adventure View Cam  
	3     - Follow View Cam  
	4     - Helicopter View Cam  
	o     - Increase EyeX  
	p     - Decrease EyeX  
	k     - Increase EyeY  
	l     - Decrease EyeY  
	n     - Increase EyeZ  
	m     - Decrease EyeZ  
	u     - Increase TargetX  
	i     - Decrease TargetX  
	h     - Increase TargetY  
	j     - Decrease TargetY  
	v     - Increase TargetZ  
	b     - Decrease TargetZ  
	q/Esc - Quit

#### Mouse Controls

The mouse controls for the game are:

	        Move Mouse Left/Right/Up/Down - Move Character Left/Right/Up/Down (Only in Cam modes 0,1,2,3)  
	 	Hold Left Button and Move Mouse Left/Right/Up/Down - Move Eye Left/Right/Up/Down (Only in Cam mode 4)  
	        Hold Right Button and Move Mouse Left/Right/Up/Down - Move Target Left/Right/Up/Down (Only in Cam mode 4)  
	      	Scroll Up/Down - Zoom In/Zoom Out (Only in Cam mode 4)  

The keyboard will still need to be used for some functionality.

#### Starting the game

- `make`
- `./3DObstacle`

#### Playing the game

Enter your name.

The **objective** of the game is to collect as many **yellow rewards** as possible which increases the score and finally move onto the **green block** which indicates the terminal block.  
You will originate in one of the blocks in the 10x10 grid.  
You can jump **over** or **into** the blocks which keep moving up and down.  
There will be some blocks which move **up and down**, and if the block is **above** a certain height, you won't be able to jump over it.  
There are **blank spaces** in the grid and they are without blocks and if you step into such a space, you fall into the **abyss** and the **game ends**.  
Falling over the **boundaries** of the grid will result in **game termination**.  
There will be **orange blocks** which indicate **fire** and **touching** which or **jumping onto** which will lead to reduced **life** and **score**.  
There will be **red blocks** which are rising up and which indicate that **jumping into** such a **depth** will cause loss of **life**.  
You will have **three** lives to finish the game.  
You will have to avoid running into the **red dangers** failing which results in a loss of **score** and **lives**.  

You can **increase/decrease** the speed of the character.  
You can toggle between different **camera views**.  
You can view the scene at **different angles** from each of the camera views.  

Enjoy and challenge your friends to a battle of highscores.

**TODO** Maybe add Lighting and Headlight effect
