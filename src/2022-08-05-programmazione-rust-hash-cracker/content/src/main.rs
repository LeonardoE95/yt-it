
// Usage:
// hash_cracker: <hash_type> <hash_value> <wordlist.txt> 

use std::env;
use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};

use sha1::Digest;

const SHA1_ID : &str = "sha1";
const MD5_ID : &str = "md5";

fn init_hash_supported_map() -> HashMap<String, u32> {
    // returns hashmap that maps name of hash (i.e. "sha1") to size in
    // bits (i.e 160)
    
    let mut hash_supported : HashMap<String, u32> = HashMap::new();

    hash_supported.insert(String::from(SHA1_ID), 160);
    hash_supported.insert(String::from(MD5_ID), 128);    
    
    return hash_supported;
}

fn main() {
    let args : Vec<String> = env::args().collect();

    // check args length
    if args.len() != 4 {
	println!("Usage: ");
	println!("./hash_cracker <hash_type> <hash_value> <wordlist.txt>");
	return;
    }

    // read arguments
    let hash_type : &str = args[1].trim();
    let hash_value : &str = args[2].trim();
    let wordlist : &str = args[3].trim();

    println!("[INFO]: Binary hash_cracker called with following arguments: ");
    println!("        hash_type  = {}", hash_type);
    println!("        hash_value = {}", hash_value);
    println!("        wordlist   = {}", wordlist);

    // hash_type is supported?
    let hash_supported : HashMap<String, u32> = init_hash_supported_map();

    match hash_supported.get(hash_type) {
	None => {
	    println!("[ERROR]: has_type ({}) is not supported!", hash_type);
	    return;
	},

	Some(val) => {
	    let hash_supposed_length : u32 = *val;
	    let hash_actual_length : u32 = (hash_value.len() * 4) as u32;

	    if hash_actual_length != hash_supposed_length {
		println!("[ERROR]: hash_value ({}) has invalid length!", hash_value);
		println!("         hash_actual_length   = {}", hash_actual_length);
		println!("         hash_supposed_length = {}", hash_supposed_length);		
		return;		
	    }
	}
    }

    // wordlist file exists on filesystem?
    let file = match File::open(wordlist) {
	Err(e) => {
	    println!("[ERROR]: Could not open file {}", wordlist);
	    println!("         {}", e);
	    return;
	},
	
	Ok(file) => file
    };

    // read file and crack the hash
    let reader : BufReader<File> = BufReader::new(file);
    for line in reader.lines() {

	match line {
	    Err(e) => {
		println!("[ERROR]: Could not read line from file");
		println!("         {}", e);
		return;	
	    },

	    Ok(val) => {
		let clear_password = val.trim();

		let computed_hash = match hash_type {
		    SHA1_ID => {
			format!("{:x}", sha1::Sha1::digest(clear_password.as_bytes()))			    
		    },

		    MD5_ID => {
			format!("{:x}", md5::compute(clear_password.as_bytes()))
		    },

		    _ => {
			"".to_string()
		    }

		};

		if computed_hash == hash_value {
		    println!("[INFO]: Password found: {}, {}({})={}",
			     clear_password,
			     hash_type,
			     clear_password,
			     hash_value
		    );
		}
	    }
	}
    }    
}
