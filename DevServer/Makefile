all:devServer

LIBS = -lev
CFLAGS = 
LDFLAGS = 

GCC=g++

CFLAGS = ();

devServer:av_redis.o av_sql.o av_app_server.o
	$(GCC) mainLoop.cpp $^ -o $@ $(LIBS)
	
clean:
	rm *.o