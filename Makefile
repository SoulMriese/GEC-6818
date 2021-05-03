main:
	arm-linux-gcc *.c -o main -I./libjpeg -L./libjpeg -ljpeg -pthread

clean:
	rm main

