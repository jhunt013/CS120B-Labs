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
