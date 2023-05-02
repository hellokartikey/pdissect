CC	=	gcc

all: attacker victim

attacker: attacker.c
	$(CC) -o attacker attacker.c

victim: victim.c
	$(CC) -o victim victim.c

.PHONY = clean
clean:
	rm victim
	rm attacker