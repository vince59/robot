AR=ar rc libnode.a
LIB=libnode.a
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(LIB)
	@echo "compile libnode.a"

libnode.a: $(OBJ)
	@$(AR) $@ $^ 

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(LIB)
