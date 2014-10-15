main:
	g++ -I raspberry_osio_client/ -o program sample/main.cpp raspberry_osio_client/raspberry_osio_client.cpp -lmosquitto
