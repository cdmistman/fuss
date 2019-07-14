#include <stdlib.h>
#include "cstack.h"

void
cstack_push(cstack** at, char val)
{
	cstack* new = (cstack*)malloc(sizeof(cstack));
	new->down = *at;
	new->top = val;
	*at = new;
}

void
cstack_pop(cstack** this)
{
	cstack* old = *this;
	*this = old->down;
	old->top = 0;
	old->down = 0;
	free(old);
}

char
cstack_peek(cstack* this)
{
	if (this == 0) return 0;
	return this->top;
}
