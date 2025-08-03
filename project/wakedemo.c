#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

char blue = 31, green = 0, red = 31;
unsigned char step = 0;
short redrawScreen = 1;

u_int backgroundColors[] = {COLOR_WHITE, COLOR_BLACK, COLOR_GREEN, COLOR_BLUE};
int currentColorIndex = 0;

// Function to update switch interrupt sense
static char switch_update_interrupt_sense(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

// Function to initialize switches
void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
}

int switches = 0;

// Switch interrupt handler
void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;

  if (switches & SW1)
    currentColorIndex = 0;
  else if (switches & SW2)
    currentColorIndex = 1;
  else if (switches & SW3)
    currentColorIndex = 2;
  else if (switches & SW4)
    currentColorIndex = 3;

  clearScreen(backgroundColors[currentColorIndex]);
  redrawScreen = 1;
}

u_int controlFrontColor = COLOR_GREEN;

// Function to handle interrupt
void wdt_c_handler(){
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25){
    {
      if (switches & SW3) green = (green + 1) % 64;
      if (switches & SW2) blue = (blue + 2) % 32;
      if (switches & SW1) red = (red - 3) % 32;
      if (step <= 30)
	step ++;
      else
	step = 0;
    }
    secCount = 0;
    redrawScreen = 1;
  }
}

void update_shape();

// Main function
void main(){
  P1DIR |= LED;
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  enableWDTInterrupts();
  or_sr(0x8);
  clearScreen(COLOR_PURPLE);

  while (1){
    if (redrawScreen){
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;
    or_sr(0x10);
    P1OUT |= LED;
  }
}

void drawHeart(int x, int y, int color){
  fillRectangle(x, y, 1, 1, color);
  fillRectangle(x + 2, y, 1, 1, color);

  fillRectangle(x, y + 1, 1, 1, color);
  fillRectangle(x + 1, y + 1, 1, 1, color);
  fillRectangle(x + 2, y + 1, 1, 1, color);

  fillRectangle(x + 1, y + 2, 1, 1, color);
}

void update_shape(){
  //Head Band
  fillRectangle(49, 128, 27, 1, COLOR_BLACK);
  fillRectangle(50, 126, 1, 2, COLOR_RED);
  fillRectangle(74, 126, 1, 2, COLOR_RED);
  fillRectangle(51, 124, 23, 4, COLOR_RED);
  fillRectangle(51, 123, 23, 1, COLOR_BLACK);

  //Bangs
  fillRectangle(48, 128, 1, 2, COLOR_BLACK);
  fillRectangle(47, 131, 1, 2, COLOR_BLACK);
  fillRectangle(46, 133, 1, 3, COLOR_BLACK);
  fillRectangle(45, 136, 1, 8, COLOR_BLACK);
  fillRectangle(46, 140, 1, 1, COLOR_BLACK);
  fillRectangle(47, 141, 1, 1, COLOR_BLACK);
  fillRectangle(48, 137, 1, 5, COLOR_BLACK);
  fillRectangle(49, 135, 1, 2, COLOR_BLACK);
  fillRectangle(50, 137, 1, 2, COLOR_BLACK);
  fillRectangle(51, 139, 1, 1, COLOR_BLACK);
  fillRectangle(52, 138, 1, 3, COLOR_BLACK);
  fillRectangle(53, 134, 1, 4, COLOR_BLACK);
  fillRectangle(54, 133, 1, 1, COLOR_BLACK);
  fillRectangle(55, 131, 1, 8, COLOR_BLACK);
  fillRectangle(56, 139, 1, 1, COLOR_BLACK);
  fillRectangle(57, 140, 1, 1, COLOR_BLACK);
  fillRectangle(58, 139, 1, 2, COLOR_BLACK);
  fillRectangle(59, 141, 1, 1, COLOR_BLACK);
  fillRectangle(60, 142, 1, 1, COLOR_BLACK);
  fillRectangle(61, 143, 2, 1, COLOR_BLACK);
  fillRectangle(63, 141, 1, 2, COLOR_BLACK);
  fillRectangle(64, 140, 1, 1, COLOR_BLACK);
  fillRectangle(65, 138, 1, 3, COLOR_BLACK);
  fillRectangle(66, 139, 1, 1, COLOR_BLACK);
  fillRectangle(67, 136, 1, 3, COLOR_BLACK);
  fillRectangle(68, 131, 1, 5, COLOR_BLACK);
  fillRectangle(69, 133, 1, 1, COLOR_BLACK);
  fillRectangle(70, 134, 1, 1, COLOR_BLACK);
  fillRectangle(71, 135, 1, 1, COLOR_BLACK);
  fillRectangle(72, 136, 1, 1, COLOR_BLACK);
  fillRectangle(73, 137, 1, 2, COLOR_BLACK);
  fillRectangle(74, 139, 1, 2, COLOR_BLACK);
  fillRectangle(75, 140, 1, 1, COLOR_BLACK);
  fillRectangle(76, 139, 1, 1, COLOR_BLACK);
  fillRectangle(77, 138, 1, 1, COLOR_BLACK);
  fillRectangle(78, 139, 1, 2, COLOR_BLACK);
  fillRectangle(79, 138, 1, 5, COLOR_BLACK);
  fillRectangle(78, 134, 1, 3, COLOR_BLACK);
  fillRectangle(77, 131, 1, 3, COLOR_BLACK);
  fillRectangle(76, 129, 1, 2, COLOR_BLACK);
  fillRectangle(49, 129, 27, 1, COLOR_PINK);
  fillRectangle(48, 130, 28, 1, COLOR_PINK);
  fillRectangle(56, 131, 12, 5, COLOR_PINK);
  fillRectangle(46, 136, 1, 4, COLOR_PINK);
  fillRectangle(47, 133, 1, 8, COLOR_PINK);
  fillRectangle(48, 131, 1, 6, COLOR_PINK);
  fillRectangle(49, 131, 1, 4, COLOR_PINK);
  fillRectangle(50, 131, 1, 6, COLOR_PINK);
  fillRectangle(51, 131, 1, 8, COLOR_PINK);
  fillRectangle(52, 131, 1, 7, COLOR_PINK);
  fillRectangle(53, 131, 1, 3, COLOR_PINK);
  fillRectangle(54, 131, 1, 2, COLOR_PINK);
  fillRectangle(69, 131, 1, 2, COLOR_PINK);
  fillRectangle(70, 131, 1, 3, COLOR_PINK);
  fillRectangle(71, 131, 1, 4, COLOR_PINK);
  fillRectangle(72, 131, 1, 5, COLOR_PINK);
  fillRectangle(73, 131, 1, 6, COLOR_PINK);
  fillRectangle(74, 131, 1, 8, COLOR_PINK);
  fillRectangle(75, 131, 1, 9, COLOR_PINK);
  fillRectangle(76, 131, 1, 8, COLOR_PINK);
  fillRectangle(77, 134, 1, 4, COLOR_PINK);
  fillRectangle(56, 136, 1, 3, COLOR_PINK);
  fillRectangle(57, 136, 1, 4, COLOR_PINK);
  fillRectangle(58, 136, 1, 3, COLOR_PINK);
  fillRectangle(59, 136, 1, 5, COLOR_PINK);
  fillRectangle(60, 136, 1, 6, COLOR_PINK);
  fillRectangle(61, 136, 1, 7, COLOR_PINK);
  fillRectangle(62, 136, 1, 7, COLOR_PINK);
  fillRectangle(63, 136, 1, 5, COLOR_PINK);
  fillRectangle(64, 136, 1, 4, COLOR_PINK);
  fillRectangle(65, 136, 1, 2, COLOR_PINK);
  fillRectangle(66, 136, 1, 3, COLOR_PINK);
  fillRectangle(78, 137, 1, 2, COLOR_PINK);
  
  //Skin tone
  fillRectangle(52, 155, 21, 5, COLOR_BEIGE);
  fillRectangle(45, 144, 1, 1, COLOR_BEIGE);
  fillRectangle(45, 152, 1, 1, COLOR_BEIGE);
  fillRectangle(46, 141, 1, 3, COLOR_BEIGE);
  fillRectangle(46, 153, 1, 3, COLOR_BEIGE);
  fillRectangle(47, 142, 1, 1, COLOR_BEIGE);
  fillRectangle(47, 154, 1, 4, COLOR_BEIGE);
  fillRectangle(48, 142, 1, 1, COLOR_BEIGE);
  fillRectangle(48, 154, 1, 3, COLOR_BEIGE);
  fillRectangle(49, 137, 1, 1, COLOR_BEIGE);
  fillRectangle(49, 139, 1, 4, COLOR_BEIGE);
  fillRectangle(49, 154, 1, 4, COLOR_BEIGE);
  fillRectangle(50, 139, 1, 3, COLOR_BEIGE);
  fillRectangle(50, 155, 1, 4, COLOR_BEIGE);
  fillRectangle(51, 140, 1, 2, COLOR_BEIGE);
  fillRectangle(51, 155, 1, 4, COLOR_BEIGE);
  fillRectangle(52, 141, 1, 1, COLOR_BEIGE);
  fillRectangle(53, 139, 1, 4, COLOR_BEIGE);
  fillRectangle(54, 134, 1, 4, COLOR_BEIGE);
  fillRectangle(54, 139, 1, 4, COLOR_BEIGE);
  fillRectangle(55, 139, 1, 5, COLOR_BEIGE);
  fillRectangle(55, 154, 1, 1, COLOR_BEIGE);
  fillRectangle(56, 140, 1, 5, COLOR_BEIGE);
  fillRectangle(56, 154, 1, 1, COLOR_BEIGE);
  fillRectangle(57, 141, 1, 6, COLOR_BEIGE);
  fillRectangle(57, 154, 1, 1, COLOR_BEIGE);
  fillRectangle(58, 141, 1, 7, COLOR_BEIGE);
  fillRectangle(58, 151, 1, 4, COLOR_BEIGE);
  fillRectangle(59, 142, 1, 13, COLOR_BEIGE);
  fillRectangle(60, 143, 1, 12, COLOR_BEIGE);
  fillRectangle(61, 144, 2, 11, COLOR_BEIGE);
  fillRectangle(63, 143, 1, 12, COLOR_BEIGE);
  fillRectangle(64, 141, 1, 14, COLOR_BEIGE);
  fillRectangle(65, 141, 1, 14, COLOR_BEIGE);
  fillRectangle(66, 140, 1, 15, COLOR_BEIGE);
  fillRectangle(67, 139, 1, 7, COLOR_BEIGE);
  fillRectangle(68, 136, 1, 2, COLOR_BEIGE);
  fillRectangle(68, 139, 1, 5, COLOR_BEIGE);
  fillRectangle(69, 134, 1, 4, COLOR_BEIGE);
  fillRectangle(69, 139, 1, 4, COLOR_BEIGE);
  fillRectangle(70, 135, 1, 2, COLOR_BEIGE);
  fillRectangle(70, 139, 1, 4, COLOR_BEIGE);
  fillRectangle(71, 136, 1, 1, COLOR_BEIGE);
  fillRectangle(71, 139, 1, 3, COLOR_BEIGE);
  fillRectangle(72, 139, 1, 3, COLOR_BEIGE);
  fillRectangle(73, 139, 1, 3, COLOR_BEIGE);
  fillRectangle(74, 141, 1, 1, COLOR_BEIGE);
  fillRectangle(75, 141, 1, 1, COLOR_BEIGE);
  fillRectangle(76, 140, 1, 3, COLOR_BEIGE);
  fillRectangle(77, 139, 1, 5, COLOR_BEIGE);
  fillRectangle(78, 141, 1, 4, COLOR_BEIGE);
  fillRectangle(79, 143, 1, 3, COLOR_BEIGE);
  fillRectangle(67, 147, 1, 8, COLOR_BEIGE);
  fillRectangle(68, 152, 1, 3, COLOR_BEIGE);
  fillRectangle(69, 153, 1, 2, COLOR_BEIGE);
  fillRectangle(70, 154, 1, 1, COLOR_BEIGE);
  fillRectangle(73, 155, 1, 4, COLOR_BEIGE);
  fillRectangle(74, 155, 1, 4, COLOR_BEIGE);
  fillRectangle(75, 155, 1, 3, COLOR_BEIGE);
  fillRectangle(76, 154, 1, 3, COLOR_BEIGE);
  fillRectangle(77, 154, 1, 2, COLOR_BEIGE);
  fillRectangle(78, 152, 1, 3, COLOR_BEIGE);
  fillRectangle(79, 151, 1, 2, COLOR_BEIGE);
  fillRectangle(57, 147, 1, 1, COLOR_BEIGE);
  fillRectangle(57, 151, 1, 3, COLOR_BEIGE);
  fillRectangle(58, 148, 1, 3, COLOR_BEIGE);
  fillRectangle(49, 138, 1, 1, COLOR_PINK);
  fillRectangle(53, 138, 2, 1, COLOR_PINK);
  fillRectangle(68, 138, 5, 1, COLOR_PINK);
  fillRectangle(70, 137, 3, 1, COLOR_PINK);
  
  //Clothes
  fillRectangle(51, 159, 1, 1, COLOR_BLACK);
  fillRectangle(49, 158, 1, 1, COLOR_BLACK);
  fillRectangle(48, 157, 1, 1, COLOR_BLACK);
  fillRectangle(47, 158, 1, 1, COLOR_BLACK);
  fillRectangle(46, 156, 1, 4, COLOR_BLACK);
  fillRectangle(49, 159, 2, 1, COLOR_RED);
  fillRectangle(48, 158, 1, 2, COLOR_WHITE);
  fillRectangle(47, 159, 1, 1, COLOR_WHITE);
  fillRectangle(73, 159, 1, 1, COLOR_BLACK);
  fillRectangle(75, 158, 1, 1, COLOR_BLACK);
  fillRectangle(76, 157, 1, 1, COLOR_BLACK);
  fillRectangle(77, 156, 1, 1, COLOR_BLACK);
  fillRectangle(78, 155, 1, 5, COLOR_BLACK);
  fillRectangle(74, 159, 2, 1, COLOR_RED);
  fillRectangle(76, 158, 2, 2, COLOR_WHITE);
  fillRectangle(77, 157, 1, 1, COLOR_WHITE);

  //Left eye line
  fillRectangle(56, 145, 1, 2, COLOR_BLACK);
  fillRectangle(55, 144, 1, 1, COLOR_BLACK);
  fillRectangle(47, 143, 8, 1, COLOR_BLACK);
  fillRectangle(50, 142, 3, 1, COLOR_BLACK);
  fillRectangle(46, 144, 3, 1, COLOR_BLACK);
  fillRectangle(45, 145, 3, 1, COLOR_BLACK);
  fillRectangle(45, 146, 1, 6, COLOR_BLACK);
  fillRectangle(46, 146, 1, 1, COLOR_BLACK);
  fillRectangle(47, 153, 1, 1, COLOR_BLACK);

  //Right eye line
  fillRectangle(67, 146, 1, 1, COLOR_BLACK);
  fillRectangle(68, 144, 1, 2, COLOR_BLACK);
  fillRectangle(69, 143, 1, 2, COLOR_BLACK);
  fillRectangle(70, 143, 7, 1, COLOR_BLACK);
  fillRectangle(72, 142, 4, 1, COLOR_BLACK);
  fillRectangle(76, 144, 2, 1, COLOR_BLACK);
  fillRectangle(77, 145, 2, 1, COLOR_BLACK);
  fillRectangle(78, 146, 2, 1, COLOR_BLACK);
  fillRectangle(78, 147, 2, 1, COLOR_BLACK);
  fillRectangle(79, 148, 1, 3, COLOR_BLACK);
  fillRectangle(78, 151, 1, 1, COLOR_BLACK);
  fillRectangle(77, 153, 1, 1, COLOR_BLACK);

  //Right eye
  fillRectangle(69, 146, 3, 1, COLOR_WHITE);
  fillRectangle(70, 147, 1, 1, COLOR_WHITE);
  fillRectangle(71, 144, 4, 2, COLOR_PURPLE);
  fillRectangle(70, 145, 1, 1, COLOR_PURPLE);
  fillRectangle(74, 146, 3, 2, COLOR_PURPLE);
  fillRectangle(76, 148, 1, 1, COLOR_PURPLE);
  fillRectangle(72, 146, 2, 3, COLOR_PINK);
  fillRectangle(69, 148, 3, 2, COLOR_PINK);
  fillRectangle(74, 148, 2, 2, COLOR_PINK);
  fillRectangle(76, 149, 1, 1, COLOR_PURPLE);
  fillRectangle(75, 144, 1, 1, COLOR_WHITE);
  fillRectangle(76, 145, 1, 1, COLOR_WHITE);
  fillRectangle(77, 146, 1, 7, COLOR_WHITE);
  fillRectangle(78, 148, 1, 3, COLOR_WHITE);
  fillRectangle(68, 146, 1, 1, COLOR_PINK);
  fillRectangle(69, 145, 1, 1, COLOR_PINK);
  fillRectangle(70, 144, 1, 1, COLOR_PINK);
  fillRectangle(68, 147, 1, 5, COLOR_PURPLE);
  fillRectangle(69, 147, 1, 1, COLOR_PURPLE);
  fillRectangle(71, 147, 1, 1, COLOR_PURPLE);
  fillRectangle(69, 150, 1, 3, COLOR_PURPLE);
  fillRectangle(70, 150, 7, 1, COLOR_PURPLE);
  fillRectangle(72, 149, 2, 1, COLOR_PURPLE);
  fillRectangle(75, 151, 2, 1, COLOR_PURPLE);
  fillRectangle(70, 151, 5, 3, COLOR_PINK);
  fillRectangle(76, 152, 1, 1, COLOR_PURPLE);
  fillRectangle(75, 153, 1, 2, COLOR_PINK);
  fillRectangle(71, 154, 4, 1, COLOR_PINK);
  fillRectangle(76, 153, 1, 1, COLOR_WHITE);
  fillRectangle(75, 145, 1, 1, COLOR_PURPLE);
  fillRectangle(75, 152, 1, 1, COLOR_PINK);
  
  //Left eye
  fillRectangle(48, 146, 3, 1, COLOR_WHITE);
  fillRectangle(49, 147, 1, 1, COLOR_WHITE);
  fillRectangle(50, 144, 4, 2, COLOR_PURPLE);
  fillRectangle(49, 145, 1, 1, COLOR_PURPLE);
  fillRectangle(53, 146, 3, 2, COLOR_PURPLE);
  fillRectangle(55, 148, 1, 1, COLOR_PURPLE);
  fillRectangle(51, 146, 2, 3, COLOR_PINK);
  fillRectangle(48, 148, 3, 2, COLOR_PINK);
  fillRectangle(53, 148, 2, 2, COLOR_PINK);
  fillRectangle(55, 149, 1, 1, COLOR_PURPLE);
  fillRectangle(54, 144, 1, 1, COLOR_BEIGE);
  fillRectangle(55, 145, 1, 1, COLOR_BEIGE);
  fillRectangle(56, 147, 1, 7, COLOR_BEIGE);
  fillRectangle(57, 148, 1, 3, COLOR_BEIGE);
  fillRectangle(47, 146, 1, 1, COLOR_PINK);
  fillRectangle(48, 145, 1, 1, COLOR_PINK);
  fillRectangle(49, 144, 1, 1, COLOR_PINK);
  fillRectangle(47, 147, 1, 5, COLOR_PURPLE);
  fillRectangle(48, 147, 1, 1, COLOR_PURPLE);
  fillRectangle(50, 147, 1, 1, COLOR_PURPLE);
  fillRectangle(48, 150, 1, 3, COLOR_PURPLE);
  fillRectangle(49, 150, 7, 1, COLOR_PURPLE);
  fillRectangle(51, 149, 2, 1, COLOR_PURPLE);
  fillRectangle(54, 151, 2, 1, COLOR_PURPLE);
  fillRectangle(49, 151, 5, 3, COLOR_PINK);
  fillRectangle(55, 152, 1, 1, COLOR_PURPLE);
  fillRectangle(54, 153, 1, 2, COLOR_PINK);
  fillRectangle(50, 154, 4, 1, COLOR_PINK);
  fillRectangle(55, 153, 1, 1, COLOR_WHITE);
  fillRectangle(46, 147, 1, 6, COLOR_WHITE);
  fillRectangle(47, 151, 1, 2, COLOR_WHITE);
  fillRectangle(48, 153, 1, 1, COLOR_WHITE);
  fillRectangle(54 ,145, 1, 1, COLOR_PURPLE);
  fillRectangle(54, 152, 1, 1, COLOR_PINK);

  //Top hair
  fillRectangle(58, 122, 1, 1, COLOR_BLACK);
  fillRectangle(57, 120, 1, 2, COLOR_BLACK);
  fillRectangle(56, 119, 1, 1, COLOR_BLACK);
  fillRectangle(55, 117, 1, 2, COLOR_BLACK);
  fillRectangle(54, 116, 1, 1, COLOR_BLACK);
  fillRectangle(52, 115, 2, 1, COLOR_BLACK);
  fillRectangle(50, 114, 2, 1, COLOR_BLACK);
  fillRectangle(49, 111, 1, 3, COLOR_BLACK);
  fillRectangle(50, 111, 3, 1, COLOR_BLACK);
  fillRectangle(50, 110, 7, 1, COLOR_BLACK);
  fillRectangle(53, 109, 2, 1, COLOR_BLACK);
  fillRectangle(54, 108, 17, 1, COLOR_BLACK);
  fillRectangle(55, 107, 1, 1, COLOR_BLACK);
  fillRectangle(56, 106, 2, 1, COLOR_BLACK);
  fillRectangle(57, 105, 2, 1, COLOR_BLACK);
  fillRectangle(59, 104, 7, 1, COLOR_BLACK);
  fillRectangle(66, 105, 2, 1, COLOR_BLACK);
  fillRectangle(68, 106, 1, 1, COLOR_BLACK);
  fillRectangle(69, 107, 1, 1, COLOR_BLACK);
  fillRectangle(70, 109, 2, 1, COLOR_BLACK);
  fillRectangle(68, 110, 7, 1, COLOR_BLACK);
  fillRectangle(72, 111, 2, 1, COLOR_BLACK);
  fillRectangle(72, 113, 1, 1, COLOR_BLACK);
  fillRectangle(72, 114, 2, 1, COLOR_BLACK);
  fillRectangle(71, 115, 1, 1, COLOR_BLACK);
  fillRectangle(70, 116, 1, 1, COLOR_BLACK);
  fillRectangle(69, 117, 1, 1, COLOR_BLACK);
  fillRectangle(68, 118, 1, 1, COLOR_BLACK);
  fillRectangle(67, 119, 1, 2, COLOR_BLACK);
  fillRectangle(66, 121, 1, 2, COLOR_BLACK);
  fillRectangle(56, 107, 13, 1, COLOR_PINK);
  fillRectangle(58, 106, 10, 1, COLOR_PINK);
  fillRectangle(59, 105, 7, 1, COLOR_PINK);
  fillRectangle(55, 109, 15, 1, COLOR_PINK);
  fillRectangle(57, 110, 11, 9, COLOR_PINK);
  fillRectangle(57, 119, 10, 1, COLOR_PINK);
  fillRectangle(58, 120, 9, 1, COLOR_PINK);
  fillRectangle(58, 121, 8, 1, COLOR_PINK);
  fillRectangle(59, 122, 7, 1, COLOR_PINK);
  fillRectangle(56, 111, 1, 8, COLOR_PINK);
  fillRectangle(55, 111, 1, 6, COLOR_PINK);
  fillRectangle(54, 111, 1, 5, COLOR_PINK);
  fillRectangle(53, 111, 1, 4, COLOR_PINK);
  fillRectangle(52, 112, 1, 3, COLOR_PINK);
  fillRectangle(51, 112, 1, 2, COLOR_PINK);
  fillRectangle(50, 112, 1, 2, COLOR_PINK);
  fillRectangle(68, 111, 1, 7, COLOR_PINK);
  fillRectangle(69, 111, 1, 6, COLOR_PINK);
  fillRectangle(70, 111, 1, 5, COLOR_PINK);
  fillRectangle(71, 111, 1, 4, COLOR_PINK);
  
  //Left ear
  fillRectangle(48, 110, 1, 1, COLOR_BLACK);
  fillRectangle(47, 109, 1, 1, COLOR_BLACK);
  fillRectangle(46, 108, 1, 1, COLOR_BLACK);
  fillRectangle(45, 107, 1, 1, COLOR_BLACK);
  fillRectangle(44, 106, 1, 1, COLOR_BLACK);
  fillRectangle(43, 105, 1, 1, COLOR_BLACK);
  fillRectangle(42, 104, 1, 1, COLOR_BLACK);
  fillRectangle(41, 103, 1, 1, COLOR_BLACK);
  fillRectangle(40, 102, 1, 1, COLOR_BLACK);
  fillRectangle(39, 100, 1, 2, COLOR_BLACK);
  fillRectangle(38, 102, 1, 14, COLOR_BLACK);
  fillRectangle(39, 116, 1, 2, COLOR_BLACK);
  fillRectangle(40, 118, 1, 1, COLOR_BLACK);
  fillRectangle(39, 102, 1, 14, COLOR_RED);
  fillRectangle(40, 103, 1, 15, COLOR_RED);
  fillRectangle(41, 104, 1, 14, COLOR_RED);
  fillRectangle(42, 105, 1, 11, COLOR_RED);
  fillRectangle(43, 106, 1, 9, COLOR_RED);
  fillRectangle(44, 107, 1, 7, COLOR_RED);
  fillRectangle(45, 108, 1, 5, COLOR_RED);
  fillRectangle(46, 109, 1, 4, COLOR_RED);
  fillRectangle(47, 110, 1, 3, COLOR_RED);
  fillRectangle(48, 111, 1, 2, COLOR_RED);
  
  //Right ear
  fillRectangle(75, 107, 1, 3, COLOR_BLACK);
  fillRectangle(76, 106, 2, 1, COLOR_BLACK);
  fillRectangle(78, 107, 2, 1, COLOR_BLACK);
  fillRectangle(80, 106, 1, 1, COLOR_BLACK);
  fillRectangle(81, 105, 1, 1, COLOR_BLACK);
  fillRectangle(82, 104, 1, 1, COLOR_BLACK);
  fillRectangle(83, 103, 1, 1, COLOR_BLACK);
  fillRectangle(84, 102, 1, 1, COLOR_BLACK);
  fillRectangle(85, 101, 1, 1, COLOR_BLACK);
  fillRectangle(86, 102, 1, 11, COLOR_BLACK);
  fillRectangle(87, 114, 1, 1, COLOR_BLACK);
  fillRectangle(86, 115, 1, 1, COLOR_BLACK);
  fillRectangle(85, 116, 1, 2, COLOR_BLACK);
  fillRectangle(79, 108, 1, 2, COLOR_BLACK);
  fillRectangle(80, 110, 1, 2, COLOR_BLACK);
  fillRectangle(81, 111, 2, 1, COLOR_BLACK);  
  fillRectangle(82, 112, 4, 1, COLOR_BLACK);
  fillRectangle(87, 113, 1, 1, COLOR_BLACK);
  fillRectangle(80, 107, 1, 3, COLOR_RED);
  fillRectangle(81, 106, 1, 5, COLOR_RED);
  fillRectangle(82, 105, 1, 6, COLOR_RED);
  fillRectangle(83, 104, 1, 8, COLOR_RED);
  fillRectangle(84, 103, 1, 9, COLOR_RED);
  fillRectangle(85, 102, 1, 10, COLOR_RED);
  fillRectangle(79, 112, 1, 1, COLOR_BLACK);
  fillRectangle(81, 114, 1, 1, COLOR_BLACK);
  fillRectangle(82, 113, 1, 1, COLOR_BLACK);
  fillRectangle(74, 112, 1, 1, COLOR_RED);
  fillRectangle(75, 111, 1, 1, COLOR_RED);
  fillRectangle(76, 109, 1, 2, COLOR_RED);
  fillRectangle(77, 107, 1, 2, COLOR_RED);
  fillRectangle(83, 116, 1, 2, COLOR_RED);
  fillRectangle(84, 115, 1, 1, COLOR_RED);
  fillRectangle(85, 114, 1, 1, COLOR_RED);
  fillRectangle(86, 113, 1, 1, COLOR_RED);
  fillRectangle(72, 112, 1, 1, COLOR_PINK);
  fillRectangle(73, 112, 1, 2, COLOR_WHITE);
  fillRectangle(74, 111, 1, 1, COLOR_WHITE);
  fillRectangle(75, 110, 1, 1, COLOR_WHITE);
  fillRectangle(76, 107, 1, 2, COLOR_WHITE);
  fillRectangle(75, 112, 1, 1, COLOR_WHITE);
  fillRectangle(76, 111, 1, 2, COLOR_WHITE);
  fillRectangle(77, 109, 1, 4, COLOR_WHITE);
  fillRectangle(78, 108, 1, 5, COLOR_WHITE);
  fillRectangle(79, 110, 1, 2, COLOR_WHITE);
  fillRectangle(79, 113, 1, 1, COLOR_WHITE);
  fillRectangle(80, 112, 1, 3, COLOR_WHITE);
  fillRectangle(81, 112, 1, 2, COLOR_WHITE);
  fillRectangle(81, 113, 1, 1, COLOR_WHITE);
  fillRectangle(82, 114, 1, 3, COLOR_WHITE);
  fillRectangle(83, 113, 1, 3, COLOR_WHITE);
  fillRectangle(84, 113, 1, 2, COLOR_WHITE);
  fillRectangle(85, 113, 1, 1, COLOR_WHITE);
  fillRectangle(84, 116, 1, 2, COLOR_WHITE);
  fillRectangle(85, 115, 1, 1, COLOR_WHITE);
  fillRectangle(86, 114, 1, 1, COLOR_WHITE);

  //Left hair
  fillRectangle(45, 113, 4, 1, COLOR_BLACK);
  fillRectangle(44, 114, 2, 1, COLOR_BLACK);
  fillRectangle(43, 115, 1, 1, COLOR_BLACK);
  fillRectangle(42, 116, 1, 2, COLOR_BLACK);
  fillRectangle(41, 118, 1, 1, COLOR_BLACK);
  fillRectangle(40, 119, 1, 1, COLOR_BLACK);
  fillRectangle(39, 119, 1, 3, COLOR_BLACK);
  fillRectangle(38, 121, 1, 3, COLOR_BLACK);
  fillRectangle(37, 123, 1, 3, COLOR_BLACK);
  fillRectangle(36, 125, 1, 3, COLOR_BLACK);
  fillRectangle(35, 128, 1, 4, COLOR_BLACK);
  fillRectangle(34, 132, 1, 5, COLOR_BLACK);
  fillRectangle(33, 137, 1, 12, COLOR_BLACK);
  fillRectangle(34, 149, 1, 4, COLOR_BLACK);
  fillRectangle(35, 153, 1, 2, COLOR_BLACK);
  fillRectangle(36, 155, 1, 2, COLOR_BLACK);
  fillRectangle(37, 157, 2, 1, COLOR_BLACK);
  fillRectangle(38, 158, 2, 1, COLOR_BLACK);
  fillRectangle(39, 159, 1, 1, COLOR_BLACK);
  fillRectangle(37, 151, 1, 4, COLOR_BLACK);
  fillRectangle(38, 154, 1, 3, COLOR_BLACK);
  fillRectangle(39, 156, 1, 1, COLOR_BLACK);
  fillRectangle(40, 157, 1, 2, COLOR_BLACK);
  fillRectangle(41, 159, 1, 1, COLOR_BLACK);
  fillRectangle(45, 153, 1, 4, COLOR_BLACK);
  fillRectangle(44, 141, 1, 12, COLOR_BLACK);
  fillRectangle(34, 137, 1, 12, COLOR_PINK);
  fillRectangle(35, 132, 1, 21, COLOR_PINK);
  fillRectangle(36, 128, 1, 27, COLOR_PINK);
  fillRectangle(37, 126, 1, 25, COLOR_PINK);
  fillRectangle(38, 124, 1, 30, COLOR_PINK);
  fillRectangle(39, 122, 1, 34, COLOR_PINK);
  fillRectangle(40, 120, 1, 37, COLOR_PINK);
  fillRectangle(41, 119, 1, 40, COLOR_PINK);
  fillRectangle(42, 118, 1, 42, COLOR_PINK);
  fillRectangle(43, 116, 1, 44, COLOR_PINK);
  fillRectangle(44, 115, 1, 26, COLOR_PINK);
  fillRectangle(45, 115, 1, 21, COLOR_PINK);
  fillRectangle(46, 114, 1, 19, COLOR_PINK);
  fillRectangle(47, 114, 1, 17, COLOR_PINK);
  fillRectangle(48, 114, 1, 14, COLOR_PINK);
  fillRectangle(49, 114, 1, 14, COLOR_PINK);
  fillRectangle(50, 115, 1, 11, COLOR_PINK);
  fillRectangle(51, 115, 1, 8, COLOR_PINK);
  fillRectangle(52, 116, 1, 7, COLOR_PINK);
  fillRectangle(53, 116, 1, 7, COLOR_PINK);
  fillRectangle(54, 117, 1, 6, COLOR_PINK);
  fillRectangle(55, 119, 1, 4, COLOR_PINK);
  fillRectangle(56, 120, 1, 3, COLOR_PINK);
  fillRectangle(57, 122, 1, 1, COLOR_PINK);
  fillRectangle(44, 153, 1, 7, COLOR_PINK);
  fillRectangle(45, 157, 1, 3, COLOR_PINK);
  fillRectangle(37, 155, 1, 2, COLOR_PINK);
  fillRectangle(39, 157, 1, 1, COLOR_PINK);
  fillRectangle(40, 159, 1, 1, COLOR_PINK);
  
  //Right hair
  fillRectangle(74, 113, 5, 1, COLOR_BLACK);
  fillRectangle(79, 114, 1, 1, COLOR_BLACK);
  fillRectangle(80, 115, 1, 1, COLOR_BLACK);
  fillRectangle(81, 116, 1, 1, COLOR_BLACK);
  fillRectangle(82, 117, 1, 1, COLOR_BLACK);
  fillRectangle(83, 118, 1, 1, COLOR_BLACK);
  fillRectangle(84, 118, 1, 3, COLOR_BLACK);
  fillRectangle(85, 120, 1, 2, COLOR_BLACK);
  fillRectangle(86, 121, 1, 3, COLOR_BLACK);
  fillRectangle(87, 123, 1, 3, COLOR_BLACK);
  fillRectangle(88, 126, 1, 3, COLOR_BLACK);
  fillRectangle(89, 129, 1, 6, COLOR_BLACK);
  fillRectangle(90, 134, 1, 3, COLOR_BLACK);
  fillRectangle(91, 137, 1, 10, COLOR_BLACK);
  fillRectangle(90, 147, 1, 4, COLOR_BLACK);
  fillRectangle(89, 151, 1, 3, COLOR_BLACK);
  fillRectangle(88, 154, 1, 1, COLOR_BLACK);
  fillRectangle(87, 155, 1, 2, COLOR_BLACK);
  fillRectangle(86, 155, 1, 3, COLOR_BLACK);
  fillRectangle(85, 157, 1, 1, COLOR_BLACK);
  fillRectangle(87, 149, 1, 4, COLOR_BLACK);
  fillRectangle(86, 152, 1, 2, COLOR_BLACK);
  fillRectangle(85, 154, 1, 2, COLOR_BLACK);
  fillRectangle(84, 156, 1, 1, COLOR_BLACK);
  fillRectangle(83, 157, 1, 2, COLOR_BLACK);
  fillRectangle(82, 159, 1, 1, COLOR_BLACK);
  fillRectangle(80, 142, 1, 11, COLOR_BLACK);
  fillRectangle(79, 153, 1, 4, COLOR_BLACK);
  fillRectangle(67, 121, 1, 2, COLOR_PINK);
  fillRectangle(68, 119, 1, 4, COLOR_PINK);
  fillRectangle(69, 118, 1, 5, COLOR_PINK);
  fillRectangle(70, 117, 1, 6, COLOR_PINK);
  fillRectangle(71, 116, 1, 7, COLOR_PINK);
  fillRectangle(72, 115, 1, 8, COLOR_PINK);
  fillRectangle(73, 115, 1, 8, COLOR_PINK);
  fillRectangle(74, 114, 1, 12, COLOR_PINK);
  fillRectangle(75, 114, 1, 14, COLOR_PINK);
  fillRectangle(76, 114, 1, 15, COLOR_PINK);
  fillRectangle(77, 114, 1, 17, COLOR_PINK);
  fillRectangle(78, 114, 1, 20, COLOR_PINK);
  fillRectangle(79, 115, 1, 23, COLOR_PINK);
  fillRectangle(80, 116, 1, 26, COLOR_PINK);
  fillRectangle(81, 117, 1, 43, COLOR_PINK);
  fillRectangle(82, 118, 1, 41, COLOR_PINK);
  fillRectangle(83, 119, 1, 38, COLOR_PINK);
  fillRectangle(84, 121, 1, 35, COLOR_PINK);
  fillRectangle(85, 122, 1, 32, COLOR_PINK);
  fillRectangle(86, 124, 1, 28, COLOR_PINK);
  fillRectangle(87, 126, 1, 23, COLOR_PINK);
  fillRectangle(88, 129, 1, 25, COLOR_PINK);
  fillRectangle(89, 135, 1, 16, COLOR_PINK);
  fillRectangle(90, 137, 1, 10, COLOR_PINK);
  fillRectangle(87, 153, 1, 2, COLOR_PINK);
  fillRectangle(86, 154, 1, 1, COLOR_PINK);
  fillRectangle(85, 156, 1, 1, COLOR_PINK);
  fillRectangle(80, 153, 1, 7, COLOR_PINK);
  fillRectangle(79, 157, 1, 3, COLOR_PINK);

  //Logo
  //U
  fillRectangle(34, 60, 1, 6, COLOR_BLACK);
  fillRectangle(35, 66, 1, 1, COLOR_BLACK);
  fillRectangle(36, 67, 54, 1, COLOR_BLACK);
  fillRectangle(42, 61, 48, 1, COLOR_BLACK);
  
  fillRectangle(35, 60, 1, 6, COLOR_YELLOW);
  fillRectangle(36, 66, 5, 1, COLOR_YELLOW);
  fillRectangle(40, 60, 1, 6, COLOR_YELLOW);

  fillRectangle(34, 59, 3, 1, COLOR_BLACK);
  fillRectangle(36, 60, 1, 6, COLOR_BLACK);
  fillRectangle(37, 65, 3, 1, COLOR_BLACK);
  fillRectangle(39, 60, 1, 5, COLOR_BLACK);
  fillRectangle(39, 59, 3, 1, COLOR_BLACK);
  
  fillRectangle(41, 60, 1, 7, COLOR_BLACK);

  //m
  fillRectangle(42, 62, 5, 1, COLOR_YELLOW);
  fillRectangle(42, 63, 1, 4, COLOR_YELLOW);
  fillRectangle(44, 63, 1, 4, COLOR_YELLOW);
  fillRectangle(46, 63, 1, 4, COLOR_YELLOW);
  
  fillRectangle(47, 62, 1, 5, COLOR_BLACK);
  fillRectangle(43, 63, 1, 4, COLOR_BLACK);
  fillRectangle(45, 63, 1, 4, COLOR_BLACK);
  
  //a
  fillRectangle(48, 63, 1, 4, COLOR_YELLOW);
  fillRectangle(49, 62, 4, 1, COLOR_YELLOW);
  fillRectangle(49, 64, 3, 1, COLOR_YELLOW);
  fillRectangle(52, 63, 1, 4, COLOR_YELLOW);

  fillRectangle(53, 62, 1, 5, COLOR_BLACK);
  fillRectangle(48, 62, 1, 1, COLOR_BLACK);
  fillRectangle(49, 63, 3, 1, COLOR_BLACK);
  fillRectangle(49, 65, 3, 2, COLOR_BLACK);
  
  //m
  fillRectangle(54, 62, 5, 1, COLOR_PINK);
  fillRectangle(54, 63, 1, 4, COLOR_PINK);
  fillRectangle(56, 63, 1, 4, COLOR_PINK);
  fillRectangle(58, 63, 1, 4, COLOR_PINK);

  fillRectangle(59, 62, 1, 5, COLOR_BLACK);
  fillRectangle(55, 63, 1, 4, COLOR_BLACK);
  fillRectangle(57, 63, 1, 4, COLOR_BLACK);
  
  //u
  fillRectangle(60, 62, 1, 4, COLOR_PINK);
  fillRectangle(61, 66, 3, 1, COLOR_PINK);
  fillRectangle(64, 62, 1, 5, COLOR_PINK);

  fillRectangle(65, 62, 1, 5, COLOR_BLACK);
  fillRectangle(60, 66, 1, 1, COLOR_BLACK);
  fillRectangle(61, 62, 3, 4, COLOR_BLACK);
  
  //s
  fillRectangle(67, 62, 4, 1, COLOR_PINK);
  fillRectangle(66, 63, 1, 1, COLOR_PINK);
  fillRectangle(66, 64, 5, 1, COLOR_PINK);
  fillRectangle(70, 65, 1, 1, COLOR_PINK);
  fillRectangle(66, 66, 4, 1, COLOR_PINK);

  fillRectangle(71, 62, 1, 5, COLOR_BLACK);
  fillRectangle(66, 62, 1, 1, COLOR_BLACK);
  fillRectangle(67, 63, 4, 1, COLOR_BLACK);
  fillRectangle(66, 65, 4, 1, COLOR_BLACK);
  fillRectangle(70, 66, 1, 1, COLOR_BLACK);
  
  //u
  fillRectangle(72, 62, 1, 4, COLOR_PINK);
  fillRectangle(73, 66, 3, 1, COLOR_PINK);
  fillRectangle(76, 62, 1, 5, COLOR_PINK);

  fillRectangle(77, 62, 1, 5, COLOR_BLACK);
  fillRectangle(72, 66, 1, 1, COLOR_BLACK);
  fillRectangle(73, 62, 3, 4, COLOR_BLACK);
  
  //m
  fillRectangle(78, 62, 5, 1, COLOR_PINK);
  fillRectangle(78, 63, 1, 4, COLOR_PINK);
  fillRectangle(80, 63, 1, 4, COLOR_PINK);
  fillRectangle(82, 63, 1, 4, COLOR_PINK);

  fillRectangle(83, 62, 1, 5, COLOR_BLACK);
  fillRectangle(79, 63, 1, 4, COLOR_BLACK);
  fillRectangle(81, 63, 1, 4, COLOR_BLACK);

  
  //e
  fillRectangle(84, 63, 1, 4, COLOR_PINK);
  fillRectangle(85, 62, 4, 1, COLOR_PINK);
  fillRectangle(85, 64, 4, 1, COLOR_PINK);
  fillRectangle(85, 66, 4, 1, COLOR_PINK);

  fillRectangle(89, 62, 1, 5, COLOR_BLACK);
  fillRectangle(84, 62, 1, 1, COLOR_BLACK);
  fillRectangle(85, 63, 4, 1, COLOR_BLACK);
  fillRectangle(85, 65, 4, 1, COLOR_BLACK);
  
  //Outline
  fillRectangle(33, 58, 1, 8, COLOR_RED);
  fillRectangle(34, 58, 9, 1, COLOR_RED);
  fillRectangle(42, 59, 1, 2, COLOR_RED);
  fillRectangle(43, 60, 48, 1, COLOR_RED);
  fillRectangle(90, 61, 1, 8, COLOR_RED);
  fillRectangle(36, 68, 55, 1, COLOR_RED);
  fillRectangle(34, 66, 1, 1, COLOR_RED);
  fillRectangle(35, 67, 1, 1, COLOR_RED);
  fillRectangle(37, 59, 2, 6, COLOR_RED);
  
  for (int i = 0; i < 10; i++){
    int x = rand() % screenWidth - 3;
    int y = rand() % screenHeight - 3;
 
    drawHeart(x, y, COLOR_RED);
  }
  
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
