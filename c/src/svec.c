#include <stdlib.h>
#include <string.h>

#include "svec.h"

void
ensure_capacity(svec* this)
{
	if (this->size == this->cap) {
		this->cap *= 2;
		this->data = (char**)realloc(this->data, this->cap * sizeof(char*));
	}
}

svec*
svec_make()
{
	svec* res = malloc(sizeof(svec));
	res->size = 0;
	res->cap = 4;
	res->data = malloc(4 * sizeof(char*));
	// use from lec notes
	memset(res->data, 0, sizeof(char*));
	return res;
}

char*
svec_get(svec* this, long index)
{
	if (index < 0 || index >= this->size) return NULL;
	if (this == NULL) return NULL;
	return this->data[index];
}

void
svec_add(svec* this, char* str)
{
	if (this == NULL) return;
	svec_addn(this, this->size, str);
}

void
svec_addn(svec* this, long index, char* str)
{
	if (index < 0 || index > this->size) return;
	if (this == NULL || str == NULL) return;
	ensure_capacity(this);
	
	for (int ii = this->size; ii > index; ii--) {
		this->data[ii + 1] = this->data[ii];
	}
	
	this->data[index] = str;
	this->size++;
}

void
svec_free(svec* this)
{
	for (int ii = 0; ii < this->size; ii++) {
		free(this->data[ii]);
	}
	free(this->data);
	free(this);
}
