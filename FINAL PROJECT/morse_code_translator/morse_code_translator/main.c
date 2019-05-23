/*
* morse_code_translator.c
*
* Created: 5/19/2019 11:55:48 AM
* Author : jenny
*/

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define DOT  273, 0,
#define DASH 273, 273, 273, 0,
#define END   -1

#define PLAY_SIZE 128
short play[PLAY_SIZE];

void encode(char val)
{
	printf("Encode letter %c\n", val);
	switch(val)
	{
		case 'a':
		case 'A':
		{
			short tBuf[] = { 273, 0, 273, 273, 273, -1 };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'b':
		case 'B':
		{
			short tBuf[] = { 273, 273, 273, 0, 273, 0, 273, 0, 273, 0, -1 };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'c':
		case 'C':
		{
			short tBuf[] = { DASH DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'd':
		case 'D':
		{
			short tBuf[] = { DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'e':
		case 'E':
		{
			short tBuf[] = { DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'f':
		case 'F':
		{
			short tBuf[] = { DOT DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'g':
		case 'G':
		{
			short tBuf[] = { DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'h':
		case 'H':
		{
			short tBuf[] = { DOT DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'i':
		case 'I':
		{
			short tBuf[] = { DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'j':
		case 'J':
		{
			short tBuf[] = { DOT DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'k':
		case 'K':
		{
			short tBuf[] = { DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'l':
		case 'L':
		{
			short tBuf[] = { DOT DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'm':
		case 'M':
		{
			short tBuf[] = { DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'n':
		case 'N':
		{
			short tBuf[] = { DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'o':
		case 'O':
		{
			short tBuf[] = { DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'p':
		case 'P':
		{
			short tBuf[] = { DOT DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'q':
		case 'Q':
		{
			short tBuf[] = { DASH DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'r':
		case 'R':
		{
			short tBuf[] = { DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 's':
		case 'S':
		{
			short tBuf[] = { DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 't':
		case 'T':
		{
			short tBuf[] = { DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'u':
		case 'U':
		{
			short tBuf[] = { DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'v':
		case 'V':
		{
			short tBuf[] = { DOT DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'w':
		case 'W':
		{
			short tBuf[] = { DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'x':
		case 'X':
		{
			short tBuf[] = { DASH DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'y':
		case 'Y':
		{
			short tBuf[] = { DASH DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
		case 'z':
		case 'Z':
		{
			short tBuf[] = { DASH DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			return;
		}
	}
}

void playit()
{
	int index = 0;
	while ( (play[index] != END) && (index < PLAY_SIZE))
	{
		printf("%3d) %3d\n", index, play[index]);
		index++;
	}
}

int main(void)
{
	encode ( 'a' );
	playit();

	encode ( 'B' );
	playit();

	encode ( 'c' );
	playit();
}