all: 3DObstacle

3DObstacle: main.cpp cuboid.cpp glad.c
	g++ -o 3DObstacle main.cpp glad.c -lGL -lglfw -lftgl -lSOIL -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib

clean:
	rm 3DObstacle
