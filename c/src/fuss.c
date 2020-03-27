#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

#include "cstack/cstack.h"
#include "svec/svec.h"

#define FALSE 0
#define TRUE 1

#define STDIN 0
#define STDOUT 1

#define BUF_SZ 512

void tokenize(char*, svec*, cstack**);
long get_inputs(svec*, FILE*, long, cstack**);

long perform(svec*, long, long);
long and(svec*, long, long, long);
long or(svec*, long, long, long);
long do_pipe(svec*, long, long, long);
long first_then(svec*, long, long, long);
long background(svec*, long, long, long);
long evaluate(svec*, long, long);
long change_input(svec*, long, long, long);
long change_output(svec*, long, long, long);

int
main(int argc, char* argv[])
{
	svec* cmd;
	// if we passed arguments to fuss, run the file
	if (argc == 2) {
		// open file and instantiate svec and cstack for the tokenizing
		FILE* file = fopen(argv[1], "r");
		cmd = svec_make();
		cstack* pq = 0;

		// the return value we will get from the execution
		int res = 0;
		
		// read in from every line in the file, then perform it
		while (get_inputs(cmd, file, TRUE, &pq) != 0) {
			while (pq != 0) cstack_pop(&pq);
			fflush(file);
			res = perform(cmd, 0, cmd->size);
			svec_free(cmd);
			cmd = svec_make();
		}
		svec_free(cmd);
		fclose(file);
		// run it and be done
		return res;
	}
	
	while (TRUE) {
		cmd = svec_make();
		// prompt
		printf("fuss$ ");
		fflush(stdout);
		
		// cstack for tokenizing
		cstack* pq = 0;
		// if EOF reached, quit fuss
		if (get_inputs(cmd, stdin, FALSE, &pq) == 0) {
			puts("");
			break;
		}
		// just in case we end up with unmatched starting parens, we can still have the memory space
		while (pq != 0) cstack_pop(&pq);
		
		// perform the input
		perform(cmd, 0, cmd->size);
		
		svec_free(cmd);
	}
	
	svec_free(cmd);
	return 0;
}

void
tokenize(char* in, svec* out, cstack** paren_quote)
{
	long curr_index = 0;
	long strt_index = -1;
	
	for (; ; curr_index++) {
		char cc = in[curr_index];
		
		// if single/double quotes or parens
		if (cc == 34 || cc == 39 || cc == 40 || cc == 41) {
			// get the value at the top of the stack, for ease
			char val = cstack_peek(*paren_quote);
			
			// if there isn't something on the stack
			if (val == 0) {
				// if we're at a closed paren when there isn't a matching paren
				if (cc == 41) {
					perror("Mismatched )");
					exit(1);
				}
				// otherwise, put the current quote or paren on the stack
				cstack_push(paren_quote, cc);
				// beginning of a token
				strt_index = curr_index;
			} else if (val < 40) {
				// if we're in quotes
				// and it matches what's on the stack
				if (cc == val) {
					// pop the last quote off the stack
					cstack_pop(paren_quote);
					// if there isn't anything else on the stack, then we're done with this token
					if (paren_quote == 0) {
						char* substr = strndup(&in[strt_index], curr_index - strt_index + 1);
						svec_add(out, substr);
						strt_index = -1;
					}
				}
			} else {
				// if the top value of the stack is (
				// if we're looking at a )
				if (cc == 41) {
					// pop the ( off the stack
					cstack_pop(paren_quote);
					// if there isn't anything else on the stack, done with this token
					if (*paren_quote == 0) {
						char* substr = strndup(&in[strt_index], curr_index - strt_index + 1);
						svec_add(out, substr);
						strt_index = -1;
					}
				} else {
					// otherwise, we're looking at the beginning of another "subtoken"
					cstack_push(paren_quote, cc);
				}
			}
		} else if (cc == 0 || cc == 9 || cc == 10 || 
			cc == 32 || cc == 38 || cc == 59 || cc == 60 || 
			cc == 62 || cc == 124) {
			// if at operator or delimiter, normal stuff
			if (*paren_quote != 0 && cc != 0 && cc != 10) continue;
			if (strt_index != -1) {
				char* substr = strndup(&in[strt_index], curr_index - strt_index);
				svec_add(out, substr);
				strt_index = -1;
			}
			
			if (cc == 0 || cc == 10) break;
			if (cc == 9 || cc == 32) continue;
			
			long op_sz;
			
			if ((cc == 38 || cc == 124) && (cc == in[curr_index + 1])) {
				op_sz = 2;
			} else {
				op_sz = 1;
			}
			
			char* substr = strndup(&in[curr_index], op_sz);
			svec_add(out, substr);
			curr_index += op_sz - 1;
		} else if (strt_index == -1) {
			strt_index = curr_index;
		}
	}
}

long
get_inputs(svec* cmd, FILE* file, long is_file, cstack** pq)
{
	char* buf = malloc(BUF_SZ * sizeof(char));
	long offset = 0;
	while (TRUE) {
		char* res = fgets(&buf[offset], BUF_SZ - offset, file);
		fflush(file);
		
		if (res == 0) {
			free(buf);
			return 0;
		}
		
		long read_in = strlen(buf);
		if (buf[read_in - 2] == '\\') {
			// test to see if we're at a valid ending location in our line
			// if we are, tokenize the line so far and keep reading in
			// otherwise, just ignore the current buffer
			svec* test_sv = svec_make();
			cstack* test_cs = 0;
			buf[read_in - 2] = 0;
			tokenize(buf, test_sv, &test_cs);
			if (test_cs == 0) {
				tokenize(buf, cmd, pq);
				read_in = 2;
				memset(buf, 0, BUF_SZ);
			}
			svec_free(test_sv);
			while (test_cs != 0) cstack_pop(&test_cs);
			if (is_file == FALSE) {
				printf("> ");
			}
			offset = read_in - 2;
		} else break;
		
		fflush(file);
	}
	fflush(file);
	tokenize(buf, cmd, pq);
	free(buf);
	return 1;
}

long
perform(svec* cmd, long start, long end)
{
	// look for semicolons
	for (int ii = start; ii < end; ii++) {
		if (cmd->data[ii][0] == ';') {
			return first_then(cmd, start, ii, end);
		}
	}
	
	// look for ands or ors
	for (int ii = start; ii < end; ii++) {
		if ((cmd->data[ii][0] == '&' && cmd->data[ii][1] != 0) ||
			(cmd->data[ii][0] == '|' && cmd->data[ii][1] != 0)) {
			if (cmd->data[ii][0] == '&') return and(cmd, start, ii, end);
			return or(cmd, start, ii, end);
		}
	}
	
	// look for pipes
	for (int ii = end - 1; ii >= start; ii--) {
		if (cmd->data[ii][0] == '|') {
			return do_pipe(cmd, start, ii, end);
		}
	}
	
	return evaluate(cmd, start, end);
}

long
and(svec* cmd, long left_start, long op_index, long right_end)
{
	long left = perform(cmd, left_start, op_index);
	
	if (left == TRUE) {
		return perform(cmd, op_index + 1, right_end);
	} else {
		return left;
	}
}

long
or(svec* cmd, long left_start, long op_index, long right_end)
{
	long left = perform(cmd, left_start, op_index);
	
	if (left == FALSE) {
		return perform(cmd, op_index + 1, right_end);
	} else {
		return left;
	}
}


long
do_pipe(svec* cmd, long left_start, long op_index, long right_end)
{
	int cpid;
	
	if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		return status;
	} else {
		int pipes[2];
		if (pipe(pipes) != 0) exit(1);
		
		int pipe_r = pipes[0];
		int pipe_w = pipes[1];
		int ccpid;
		if ((ccpid = fork())) {
			// read in parent since we must wait for results from child
			close(pipe_w);
			close(STDIN);
			dup(pipe_r);
			
			perform(cmd, op_index + 1, right_end);
			
			int cstatus;
			waitpid(ccpid, &cstatus, 0);
			
			if (cstatus == 0) exit(0);
			exit(1);
		} else {
			close(pipe_r);
			close(STDOUT);
			dup(pipe_w);
			
			if (perform(cmd, left_start, op_index) == TRUE) exit(0);
			exit(1);
		}
	}
}

long
first_then(svec* cmd, long first_start, long semicolon_index, long then_end)
{
	long left = perform(cmd, first_start, semicolon_index);
	long right = perform(cmd, semicolon_index + 1, then_end);
	
	return left * right;
}

long
background(svec* cmd, long start, long op_index, long end)
{
	if ((fork())) {
		if (op_index == end - 1) return TRUE;
		return perform(cmd, op_index + 1, end);
	} else {
		perform(cmd, start, op_index);
		exit(0);
	}
}

long
evaluate(svec* cmd, long start, long end)
{
	// if we don't have anything to do, do nothing
	if (start == end) return TRUE;
	if (strcmp(cmd->data[start], "exit") == 0) exit(0);
	if (strcmp(cmd->data[start], "cd") == 0) {
		if (end > start + 1) {
			if (chdir(cmd->data[start + 1]) == 0) return TRUE;
		} 
		return FALSE;
	}
	if (start == end - 1) {
		char* ez = cmd->data[start];
		
		if (ez[0] == 40) {
			svec* subcmd = svec_make();
			char* no_parens = strndup(&ez[1], strlen(ez) - 2);
			
			cstack* pq = 0;
			tokenize(no_parens, subcmd, &pq);
			while (pq != 0) cstack_pop(&pq);
			
			long res = perform(subcmd, 0, subcmd->size);
			free(no_parens);
			svec_free(subcmd);
			return res;
		}
	}
	
	for (long ii = end - 1; ii >= start; ii--) {
		if (strcmp(cmd->data[ii], "&") == 0) {
			return background(cmd, start, ii, end);
		} else if (strcmp(cmd->data[ii], "<") == 0) {
			return change_input(cmd, start, ii, end);
		} else if (strcmp(cmd->data[ii], ">") == 0) {
			return change_output(cmd, start, ii, end);
		}
	}
	
	long cpid;
	
	if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		if (status == 0) return TRUE;
		return FALSE;
	} else {
		char** args = malloc((end - start + 1) * sizeof(char*));
		memset(args, 0, (end - start + 1) * sizeof(char*));
		for (long ii = start; ii < end; ii++) {
			char* ez = cmd->data[ii];
			if (ez[0] == 34 || ez[0] == 39) {
				args[ii - start] = strndup(&ez[1], strlen(ez) - 2);
			} else {
				args[ii - start] = strdup(ez);
			}
		}
		
		execvp(cmd->data[start], args);
		exit(1);
	}
}

long
change_input(svec* cmd, long start, long op_index, long end)
{
	long cpid;
	
	if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		if (status == 0) return TRUE;
		return FALSE;
	} else {
		if (op_index < end - 1) {
			int in = open(cmd->data[op_index + 1], O_CREAT | O_APPEND | O_RDONLY, 0644);
			close(STDIN);
			dup(in);
			close(in);
		}
		if (evaluate(cmd, start, op_index) == TRUE) {
			exit(0);
		}
		exit(1);
	}
}

long
change_output(svec* cmd, long start, long op_index, long end)
{
	long cpid;
	
	if ((cpid = fork())) {
		int status;
		waitpid(cpid, &status, 0);
		if (status == 0) return TRUE;
		return FALSE;
	} else {
		if (op_index < end - 1) {
			int out = open(cmd->data[op_index + 1], O_CREAT | O_APPEND | O_WRONLY, 0644);
			close(STDOUT);
			dup(out);
			close(out);
		}
		if (evaluate(cmd, start, op_index) == TRUE) {
			exit(0);
		}
		exit(1);
	}
}
