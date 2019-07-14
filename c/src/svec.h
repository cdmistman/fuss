#ifndef SVEC_H
#define SVEC_H

typedef struct svec {
	char** data;
	long size;
	long cap;
} svec;

svec* svec_make();
char* svec_get(svec*, long);
void svec_add(svec*, char*);
void svec_addn(svec*, long, char*);
void svec_free(svec*);

#endif
