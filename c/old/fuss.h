#ifndef FUSS_H
#define FUSS_H

#include <stdio.h>

#include "svec/svec.h"
#include "cstack/cstack.h"

#define FALSE 0
#define TRUE 1

#define STDIN 0
#define STDOUT 1

// fuss_syntaxparse.c
long get_inputs(svec*, FILE*, long, cstack**);
void tokenize(char*, svec*, cstack**);

// fuss_exec.c
long and(svec*, long, long, long);
long change_input(svec*, long, long, long);
long change_output(svec*, long, long, long);
long background(svec*, long, long, long);
long do_pipe(svec*, long, long, long);
long evaluate(svec*, long, long);
long first_then(svec*, long, long, long);
long or(svec*, long, long, long);
long perform(svec*, long, long);

#endif
