all: exe run

exe: projeto.cpp RgbImage.cpp
	g++ -o exe projeto.cpp RgbImage.cpp -framework GLUT -framework OpenGL -Wno-deprecated

run: exe
	./exe

clean:
	rm exe