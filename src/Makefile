CC=gcc
CFLAGS=-Wall -I./include
CLIBS=-lcurl
OUT=crlt
OBJS=curl_imp.o ntt.o program.o utils.o

$(OUT): $(OBJS)
    $(CC) $(CFLAGS) $(CLIBS) -o $(OUT) $(OBJS)
	
%.o : %.c
    $(CC) $(CFLAGS) $< -o $@
	
.PHONY: clean

clean:
    rm -rf *.o $(OUT)