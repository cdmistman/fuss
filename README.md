# FUSS

**F**riendly **U**ser **S**uper **S**hell

The most user-customizable shell.

## Description

This is an educational project by me, Colton Donnelly.
It's my goal to implement this in as many languages as
I can. Ultimately, this helps me gain a deeper understanding
of any language I learn, and how each language's design
plays to its usages and interactions with other languages.

## Plans

The very first thing I want to do for this project with
any language is to bring it up to speed with the other
language implementations. Features will come, but since
this is a learning project, they aren't the priority.

## Current Features

Currently, the only features of the implementations are:
- Print the basic prompt (`fuss$ `) to stdout
- Token parsing
- Execution of files specified from the command line (`fuss myscript.sh`)
- Execution of commands from user input

## Current Languages

- C
	- The first implementation is written in C
	- Since C is a functional language, focuses on using functions heavily (only structs are string vectors and a charstack)
	- Relies on pointer-pointers to achieve tokenizing
- Rust
	- Second implementation is written in Rust
	- Rust is not as functionally reliant as C, however not strictly object-oriented. Instead, is very reliant on interfaces (Traits)
	- Currently being implemented
- Go
	- Third implementation planned to be Go
	- Functionally dependent, but pointers will be interesting

