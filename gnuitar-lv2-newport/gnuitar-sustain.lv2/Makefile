all:
	gcc -Ofast `pkg-config lv2-plugin --libs -cflags` -g -w -fvisibility=hidden -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread -I/usr/include/lv2 -lm sustain.c -o sustain.so
