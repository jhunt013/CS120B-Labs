/*
* morse_code_translator.c
*
* Created: 5/19/2019 11:55:48 AM
* Author : jenny
*/

#include <avr/io.h>

//freq[]; //used for each letter
char user_input[];
char play[]; //morse code translation 


// function to encode a alphabet as
// Morse code
void morseEncode(char x){

	// refer to the Morse table
	// image attached in the article
	switch (x) {
		case 'a':
			//return ".-";
			play[] = {273, 0, 273, 273, 273};
			break;
		case 'b':
			//return "-...";
			play[] = {273, 273, 273, 0, 273, 0, 273, 0, 273};
			break;
		case 'c':
			return "-.-.";
			break;
		case 'd':
			return "-..";
			break;
		case 'e':
			return ".";
			break;
		case 'f':
			return "..-.";
			break;
		case 'g':
			return "--.";
			break;
		case 'h':
			return "....";
			break;
		case 'i':
			return "..";
			break;
		case 'j':
			return ".---";
			break;
		case 'k':
			return "-.-";
			break;
		case 'l':
			return ".-..";
			break;
		case 'm':
			return "--";
			break;
		case 'n':
			return "-.";
			break;
		case 'o':
			return "---";
			break;
		case 'p':
			return ".--.";
			break;
		case 'q':
			return "--.-";
			break;
		case 'r':
			return ".-.";
			break;
		case 's':
			return "...";
			break;
		case 't':
			return "-";
			break;
		case 'u':
			return "..-";
			break;
		case 'v':
			return "...-";
			break;
		case 'w':
			return ".--";
			break;
		case 'x':
			return "-..-";
			break;
		case 'y':
			return "-.--";
			break;
		// for space
		case 'z':
			return "--..";
			break;
	}
}

void morseCode(char userInput[]) {

	// character by character print
	// Morse code
	for (int i = 0; userInput[i]; i++){
			cout << morseEncode(userInput[i]);
	}

	cout << endl;
}


int main(void)
{
	cout << "enter string: ";
	cin >> userInput;
	
	morseCode(userInput);
	return 0;
	while (1)
	{
	}
}

