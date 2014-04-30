#include "mbed.h"
#include <time.h>
#include "C12832_lcd.h"
#include "mtrix.h"

MBEDtrisTheGame<char> game;
 
C12832_LCD lcd;
BusIn joy(p15,p12,p13,p16);
DigitalIn fire(p14);
BusOut leds(LED1,LED2,LED3,LED4);

#define JOY_KEY_LEFT    1
#define JOY_KEY_RIGHT   2
#define JOY_KEY_UP      4
#define JOY_KEY_DOWN    8

#define BOUNCER_SIZE    4

int main()
{
    // Welcome message
    lcd.cls();
    
    std::pair<int, int> brick_prev_pos = game.get_brick_pos();
    clock_t t1, t2;
    
    while(1) {
        t1 = clock();

        int joy_key = joy.read();
        switch (joy_key) {
            case JOY_KEY_LEFT:  { game.try_move_left();   break; }
            case JOY_KEY_RIGHT: { game.try_move_right();  break; }
            case JOY_KEY_UP:    { game.try_rotate();      break; }
            case JOY_KEY_DOWN:  { game.move_brick_down(); break; }
        }
 
        // Erase prev brick from screen
        for (int x = 0; x < BRICK_SIZE_X; x++)
        {
            for (int y = 0; y < BRICK_SIZE_Y; y++)
            {
                int color = game.get_brick_cell(x, y);
                {
                    int draw_pix_x = (brick_prev_pos.first + x) * BOUNCER_SIZE;
                    int draw_pix_y = (brick_prev_pos.second + y) * BOUNCER_SIZE;
                    lcd.rect(draw_pix_y, draw_pix_x, draw_pix_y+BOUNCER_SIZE-1, draw_pix_x+BOUNCER_SIZE-1, 0);
                }
            }
        }
    
        // PRINT MATRIX
        for (int x = 0; x < LCD_W; x++)
        {
            for (int y = 0; y < LCD_H; y++)
            {         
    
                // This should be replaces with just 'putpixel on LCD'
                int draw_pix_x = x*BOUNCER_SIZE;
                int draw_pix_y = y*BOUNCER_SIZE;

                if (game.get_matrix_cell(x, y))
                {
                    //lcd.rect(draw_pix_x, draw_pix_y, draw_pix_x+BOUNCER_SIZE-1, draw_pix_y+BOUNCER_SIZE-1, 1);
                    lcd.rect(draw_pix_y, draw_pix_x, draw_pix_y+BOUNCER_SIZE-1, draw_pix_x+BOUNCER_SIZE-1, 1);
                }
                else
                {
                    //lcd.rect(draw_pix_x, draw_pix_y, draw_pix_x+BOUNCER_SIZE-1, draw_pix_y+BOUNCER_SIZE-1, 0);
                    // lcd.rect(draw_pix_y, draw_pix_x, draw_pix_y+BOUNCER_SIZE-1, draw_pix_x+BOUNCER_SIZE-1, 0);
                }
            }
        }

        // PRINT BRICK
        for (int x = 0; x < BRICK_SIZE_X; x++)
        {
            for (int y = 0; y < BRICK_SIZE_Y; y++)
            {
                int color = game.get_brick_cell(x, y);
                if (color)
                {
                    std::pair<int, int> brick_pos = game.get_brick_pos();

                    int draw_pix_x = (brick_pos.first + x) * BOUNCER_SIZE;
                    int draw_pix_y = (brick_pos.second + y) * BOUNCER_SIZE;
                    //lcd.rect(draw_pix_x, draw_pix_y, draw_pix_x+BOUNCER_SIZE-1, draw_pix_y+BOUNCER_SIZE-1, 1);
                    lcd.rect(draw_pix_y, draw_pix_x, draw_pix_y+BOUNCER_SIZE-1, draw_pix_x+BOUNCER_SIZE-1, color);
                }
            }
        }

        lcd.copy_to_lcd();
        
        brick_prev_pos = game.get_brick_pos();
        
        game.move_brick_down();
        t2 = clock();
        float diff = (((float)t2 - (float)t1) / CLOCKS_PER_SEC ); 
        wait(0.2);
    }
}
