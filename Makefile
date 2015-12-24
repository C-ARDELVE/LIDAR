all: main.c

main.c: build
	cc -o build/Lidar_data -c lidar_data/$@

build:
	mkdir $@
