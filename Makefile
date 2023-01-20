minecraft2D:
	g++ -o minecraft2D src/main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs

clean:
	rm ./minecraft2D
