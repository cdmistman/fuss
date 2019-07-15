use libc::pid_t;

use nix::unistd::{ execvp, fork, ForkResult };
use nix::sys::wait::waitpid;

use std::borrow::Borrow;
use std::ffi::CString;

pub trait Evaluatable {
	fn evaluate(&self) -> i32;
}

pub struct ExecBin {
	bin: Box<CString>,
	args: Box<[CString]>,
	is_background: bool,
	input_file: Option<Box<CString>>,
	output_file: Option<Box<CString>>,
}

pub struct Pipe {
	from: Box<Evaluatable>,
	to: Box<Evaluatable>,
}

pub struct And {
	left: Box<Evaluatable>,
	right: Box<Evaluatable>,
}

pub struct Or {
	left: Box<Evaluatable>,
	right: Box<Evaluatable>,
}

pub struct Then {
	left: Box<Evaluatable>,
	right: Box<Evaluatable>,
}

impl Evaluatable for bool {
	fn evaluate(&self) -> i32 {
		let mut res = 0;
		if *self != true {
			res = 1;
		};
		res
	}
}

impl Evaluatable for i32 {
	fn evaluate(&self) -> i32 {
		*self
	}
}
