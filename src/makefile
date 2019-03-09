OUT=../build

Terrae: Renderer.o lodepng.o
	g++ -o $(OUT)/Terrae $(OUT)/Renderer.o $(OUT)/lodepng.o -lm

lodepng.o: lodepng.h lodepng.cpp
	g++ -c -o $(OUT)/lodepng.o lodepng.cpp

Renderer.o: Renderer.cpp lodepng.h Math/Vec3.h
	g++ -c -o $(OUT)/Renderer.o Renderer.cpp

clean:
	rm -r $(OUT)
