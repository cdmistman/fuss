#ifndef CSTACK_H
#define CSTACK_H

typedef struct cstack {
	char top;
	struct cstack* down;
} cstack;

void cstack_push(cstack**, char);
void cstack_pop(cstack**);
char cstack_peek(cstack*);

#endif
