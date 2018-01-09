CC = aarch64-linux-gnu-gcc
CFLAGS = -static -O3
MAIN = test
SRC = driver.c
OBJ = $(SRC:.c=.o)

$(MAIN): $(OBJ)
		$(CC) $(SRC) $(CFLAGS) -o $(MAIN)

clean:
	$(RM) *.o $(MAIN)
