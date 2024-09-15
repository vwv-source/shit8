CC=gcc
CFLAGS=-Os -s
LIBS=-lm -lSDL2
O=build

OBJS=	\
		$(O)/main.o \
		$(O)/instructions.o \
		$(O)/draw.o

all:	$(O) $(O)/shit8

clean:
	rm ./build/*.o

$(O):
	mkdir -p $(O)

$(O)/shit8: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(O)/shit8 $(LIBS)

$(O)/%.o: %.c | $(O)
	$(CC) $(CFLAGS) -c $< -o $@
