use ::command;

use std::ffi::CString;

pub fn tokenize(input: String) -> command::Performable {
	let _input = CString::new(input);
	
	let mut curr_token = String::new();
	let mut in_quotes = false;
	let mut 
}