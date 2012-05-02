CFLAGS = `pkg-config --cflags --libs gtk+-2.0`
myorg: main.o model.o
	gcc -Wall -g $(CFLAGS) -lsqlite3 $^ -o $@
main.o: main.c model.h types.h
	gcc -Wall -g -c $(CFLAGS) main.c -o $@
model.o: model.c types.h
	gcc -Wall -g -c $(CFLAGS) model.c -o $@
clean:
	-rm myorg *.o
.PHONY: clean