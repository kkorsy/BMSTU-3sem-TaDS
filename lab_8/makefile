CC := gcc
CFLAGS := -g3 -Wall -Werror -Wextra -pedantic

CFILES := $(wildcard *.c)
INC := $(wildcard *.h)
OBJ := $(CFILES:%.c=%.o)

app.exe: $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.exe 
