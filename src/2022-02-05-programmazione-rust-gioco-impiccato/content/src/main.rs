// @Author: Leonardo Tamiano
// @Youtube-video: https://youtu.be/-iyhalPUy4A
// 
// ---------------------------------------------

extern crate ncurses;

use ncurses::*;

const MAX_TRIES : i8 = 8;

const INFO_MSG : i16 = 4;
const GOOD_MSG : i16 = 5;
const BAD_MSG  : i16 = 6;

enum MessageType {
    InfoMessage,
    GoodMessage,
    BadMessage,
}

fn print_message(msg_type : MessageType, msg: &str) {

    match msg_type {

	MessageType::InfoMessage => {
	    attron(COLOR_PAIR(INFO_MSG));
	    addstr(&format!("[INFO] - {}", msg));
	    attroff(COLOR_PAIR(INFO_MSG));
	},

	MessageType::GoodMessage => {
	    attron(COLOR_PAIR(GOOD_MSG));
	    addstr(&format!("[GOOD :D] - {}", msg));
	    attroff(COLOR_PAIR(GOOD_MSG));
	},

	MessageType::BadMessage => {
	    attron(COLOR_PAIR(BAD_MSG));
	    addstr(&format!("[BAD :/] - {}", msg));	    
	    attroff(COLOR_PAIR(BAD_MSG));
	},	
    }
}

fn show_logo() {
    mvaddstr(4,  5,  "              _______                             _______        ");
    mvaddstr(5,  5,  "             |/      |                           |/      |       ");
    mvaddstr(6,  5,  "             |                                   |      (X)      ");
    mvaddstr(7,  5,  "             |                                   |       |       ");
    mvaddstr(8,  5,  "             |                  -------->        |      /|\\     ");
    mvaddstr(9,  5,  "             |                                   |       |       ");
    mvaddstr(10, 5,  "             |                                   |      / \\     ");
    mvaddstr(11, 5,  "             |                                   |               ");
    mvaddstr(12, 5,  "            _|___                                |               "); 
}


fn show_hangman(tries : i8) {
    if tries == 8 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |                                                   ");
	mvaddstr(7,  5,  "             |                                                   ");
	mvaddstr(8,  5,  "             |                                                   ");
	mvaddstr(9,  5,  "             |                                                   ");
	mvaddstr(10, 5,  "             |                                                   ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    	

    } else if tries == 7 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |                                                   ");
	mvaddstr(8,  5,  "             |                                                   ");
	mvaddstr(9,  5,  "             |                                                   ");
	mvaddstr(10, 5,  "             |                                                   ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    	

    } else if tries == 6 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |       |                                           ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |                                                   ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");  
    } else if tries == 5 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |       |                                           ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |      /                                            ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");   
	
    } else if tries == 4 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |       |                                           ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |      / \\                                         ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    
    } else if tries == 3 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |      /|                                           ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |      / \\                                         ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    
    } else if tries == 2 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (_)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |      /|\\                                         ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |      / \\                                         ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    
    } else if tries == 1 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      (X)                                          ");
	mvaddstr(7,  5,  "             |       |                                           ");
	mvaddstr(8,  5,  "             |      /|\\                                         ");
	mvaddstr(9,  5,  "             |       |                                           ");
	mvaddstr(10, 5,  "             |      / \\                                         ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                ");    
    } else if tries == 0 {
	mvaddstr(4,  5,  "              _______                                            ");
	mvaddstr(5,  5,  "             |/      |                                           ");
	mvaddstr(6,  5,  "             |      rip                                          ");
	mvaddstr(7,  5,  "             |                                                   ");
	mvaddstr(8,  5,  "             |                                                   ");
	mvaddstr(9,  5,  "             |                                                   ");
	mvaddstr(10, 5,  "             |                                                   ");
	mvaddstr(11, 5,  "             |                                                   ");
	mvaddstr(12, 5,  "            _|___                                                "); 
    }
}
    
fn main() {
    initscr();

    curs_set(CURSOR_VISIBILITY::CURSOR_INVISIBLE);

    start_color();
    init_pair(INFO_MSG, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GOOD_MSG, COLOR_GREEN, COLOR_BLACK);
    init_pair(BAD_MSG, COLOR_RED, COLOR_BLACK);

    mv(2, 5);
    print_message(MessageType::InfoMessage, "Welcome to the HANGMAN Game!");
    mv(3, 5);
    addstr("                                ");

    show_logo();

    refresh();

    mv(16, 5);
    print_message(MessageType::InfoMessage, "Inserire parola da indovinare: ");
    
    // read password from user w/o showing it on screen
    noecho();
    let mut secret = String::new();
    getstr(&mut secret);
    echo();

    mv(17, 5);    
    addstr("=======================================");
    
    let mut guessed_word_so_far = "_".repeat(secret.len());
    let mut tries = MAX_TRIES;

    loop {
	// 1. Stampare stato corrente:
	//    - stato corrente parola
	//    - tentativi rimanenti

	show_hangman(tries);
	
	mv(18, 5);
	print_message(MessageType::InfoMessage, &format!("Stato corrente: {}", &guessed_word_so_far));
	
	mv(19, 5);
	print_message(MessageType::InfoMessage, &format!("Tentativi rimanenti: {}", tries));

	// 2. Controllare stato di terminazione
	mv(25, 5);
	if tries == 0 {
	    print_message(MessageType::BadMessage, &format!("You lost, the secret was: `{}`", &secret));
	    break;
	} else if guessed_word_so_far.find("_") == None {
	    print_message(MessageType::GoodMessage, "You won!");
	    break;
	}

	// 3. Chiediamo all'utente di inserire una nuova lettera
	mv(23, 5);
	print_message(MessageType::InfoMessage, "Inserire lettera: ");
	let key = getch();

	// 4. Check della parola
	let mut found = false;
	for (index, c) in secret.chars().enumerate() {
	    if c == key as u8 as char {
		found = true;
		guessed_word_so_far.replace_range(index..index+1, &(key as u8 as char).to_string());	    
	    }
	}

	// 5. Aggiorniamo lo stato del programma
	mv(25, 5);
	if found {
	    print_message(MessageType::GoodMessage, "Found :)    ");
	} else {
	    print_message(MessageType::BadMessage,  "Not Found :/");
	    tries -= 1;
	}    
    }
    
    mv(28, 0);
    noecho();
    getch();
    
    endwin();
}
