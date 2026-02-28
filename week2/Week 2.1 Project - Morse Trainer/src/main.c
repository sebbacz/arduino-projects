#include <usart.h>
#include <util/delay.h>
#include <BTN_LIB.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <LED_LIB.h>
#include <avr/io.h>



#define ALL_CHARS 26 





void waittime(){
  all_led_off();           
  _delay_ms(1000);
  lightUpLed(3);
  _delay_ms(1000);
  lightUpLed(2);
  _delay_ms(1000);
  lightUpLed(1);
  _delay_ms(1000);
   lightUpLed(0);
    _delay_ms(1000);
    all_led_off();
    _delay_ms(1000);


}

//letter  to number
void letters(int letter, int all[],int lenght){
    for(int i=0; i<lenght; i++){
      if(all[i]){
        all_led_on();
        _delay_ms(1000);
        all_led_off();

      }else{
        all_led_on();
        _delay_ms(400);
        all_led_off();
      }
      _delay_ms(200);
    }

}


int main(){
  //aray size=4
  int *all_letters[ALL_CHARS];
  //morse code to characters
  int code[ALL_CHARS] = {2,4,4,3,1,4,3,4,2,4,3,4,2,2,3,4,4,3,3,1,3,4,3,4,4,4};


  //Mors code coressponds
    int a[] = {0,1};
  int b[] = {1,0,0,0};
  int c[] = {1,0,1,0};
  int d[] = {1,0,0};
  int e[] = {0};
  int f[] = {0,0,1,0};
  int g[] = {1,1,0,};
  int h[] = {0,0,0,0};
  int i[] = {0,0};
  int j[] = {0,1,1,1};
  int k[] = {1,0,1};
  int l[] = {0,1,0,0};
  int m[] = {1,1};
  int n[] = {1,0};
  int o[] = {1,1,1};
  int p[] = {0,1,1,0};
  int q[] = {1,1,0,1};
  int r[] = {0,1,0};
  int s[] = {0,0,0};
  int t[] = {1};
  int u[] = {0,0,1};
  int v[] = {0,0,0,1};
  int w[] = {0,1,1};
  int x[] = {1,0,0,1};
  int y[] = {1,0,1,1};
  int z[] = {1,1,0,0};
  //

  //normal alphabet to array
   all_letters[0] = a;
  all_letters[1] = b;
  all_letters[2] = c;
  all_letters[3] = d;
  all_letters[4] = e;
  all_letters[5] = f;
  all_letters[6] = g;
  all_letters[7] = h;
  all_letters[8] = i;
  all_letters[9] = j;
  all_letters[10] = k;
  all_letters[11] = l;
  all_letters[12] = m;
  all_letters[13] = n;
  all_letters[14] = o;
  all_letters[15] = p;
  all_letters[16] = q;
  all_letters[17] = r;
  all_letters[18] = s;
  all_letters[19] = t;
  all_letters[20] = u;
  all_letters[21] = v;
  all_letters[22] = w;
  all_letters[23] = x;
  all_letters[24] = y;
  all_letters[25] = z;

  //global configure
  initUSART(); //serial
  enableAllLeds();
  enableButton(1);
  enableButton(2);
  enableButton(3);

  //start counting
  waittime();
   //display 
  for (int i = 0; i < 5; i++)
  {

    //choose random number for morse code
    int character = rand() % ALL_CHARS;
    
    letters(character, all_letters[character], code[character]);
    _delay_ms(2000);

    //array - output
    int output[] = {0, 0, 0};

    //generate random output
    output[rand() % 3] = character;
    printf("%d + %c%c%c", character, (char) output[0] + 97, (char) output[1] + 97, (char) output[2] + 97);
    for (int i = 0; i < 3; i++)
    {
      if (output[i] == 0)
      {
        output[i] = rand() % ALL_CHARS;
      }
    }

    printf("Answer 1: %c   |   Answer 2: %c   | Answer 3: %c ", (char) output[0] + 97, (char) output[1] + 97, (char) output[2] + 97);

    //set wating tiem for button
    int checkoutput = 0;
    while (checkoutput == 0)
    {
      if (buttonPushed(1) && output[0] == character)
      {
        checkoutput = 1;
      }
      else if (buttonPushed(2) && output[1] == character)
      {
        checkoutput = 1;
      }
      else if (buttonPushed(3) && output[2] == character)
      {
        checkoutput = 1;
      } else if (buttonPushed(1) || buttonPushed(2) || buttonPushed(3))
      {
        checkoutput = -1;
      }
    }
    if (checkoutput > 0) {
      printf("Right");
    } else {

      printf("False %c ", (char)character + 97);
    }
  }
  return 0;  
}
