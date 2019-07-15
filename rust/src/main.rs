pub extern crate libc;
pub extern crate nix;

pub mod command;
pub mod tokenizer;

use command::Evaluatable;

use std::env::{args, Args};

use std::process::exit;

fn main() -> ! {
	let filename = args().nth(1);
	let mut res = 0;
	if filename.is_some() {
		unimplemented!();
	} else {
		unimplemented!();
	}
	exit(res);
}
