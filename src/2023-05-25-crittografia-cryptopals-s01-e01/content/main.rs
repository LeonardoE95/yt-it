use base64::{engine::general_purpose, Engine };

fn hex_to_bytes(hex: &str) -> Vec<u8> {
    match hex::decode(hex) {
	Ok(result) => result,
	Err(e) => panic!("error during conversion: {}", e)
    }
}

fn bytes_to_base64(bytes: Vec<u8>) -> String {
    general_purpose::STANDARD.encode(bytes)
}

fn challenge1(hex: &str) -> String {
    let bytes : Vec<u8> = hex_to_bytes(hex);
    let base64 : String = bytes_to_base64(bytes);
    base64
}

fn main() {
}

#[cfg(test)]
mod tests {
    use crate::challenge1;
    
    #[test]
    fn set1_challenge1_test() {
	let input : &str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	let output : String = challenge1(input);
	assert_eq!(output, "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
    }
    
}
