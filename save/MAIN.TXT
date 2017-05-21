#include <stdio.h>
#include <conio.h>
#include "default.h"
#include "draw.h"
#include "draw_c.h"
#include "cursor.h"
#include "text.h"
#include "menu.h"

stLine * gbHead = 0;            //       
stLine * gbCurL = 0;                //      
stLine * gbTopL = 0;
stChar * gbCurC = 0;            //      

void Delete_Key();
void BackSpace_Key();

int main(void)
{
    char ch = 0;                    //  
    char key = 0;                    //  
    stChar * TmpC;
    int i;
    
    clrscr();
    Draw_MainWindow();

    if(Init() == FAIL)    return 0;
    
    Draw_Text_Cursor();
    
    while(ALT_X != key)
    {
        ch = getch();
        if( ch == 0 )    //   
        {
            key = getch();            //  
            switch(key)
            {
                // F10    
                case F10:         if( END == Menu()) 
                                {    // EXIT ALT_X
                                    FreeAll();
                                    return 0;
                                }
                                Draw_Text_Cursor();
                                break;
                        
                case R_ARROW :   Draw_Text_RMove();     break;
                case L_ARROW :    Draw_Text_LMove();     break;
                case U_ARROW :     Draw_Text_UMove();    break;
                case D_ARROW :     Draw_Text_DMove();    break;
                
                case HOME_KEY:    gbCurC = 0;            
                                Draw_Text_Cursor();    break;
                case END_KEY:    Draw_Text_End();    break;
                    
                
                case DELETE :     if(( 0 == gbCurC->next )&&( 0 == gbCurL->down ))    
                                    continue;
                                Delete_Key();
                                TextChanged();    break;
                case F1 :         break;
                default:             break;
            }
        }
        else
        {
            // -----------------------------------
            if(ch == BACK_SPACE)
            {    
                if(( 0 == gbCurC ) && ( 0 == gbCurL->up ))    continue;

                if((gbCurC->char_number == MAX_TEXT)&&(gbCurL->down != 0))
                {
                    gbCurL = gbCurL->down;
                    gbCurC = 0;
                }
                
                if(gbCurC == 0)    DeleteUp();
                else                Delete();

                if(gbCurL->line_number < gbTopL->line_number)
                {
                    gbTopL = gbTopL->up;
                    Clr_Text();
                    Draw_Scroll(gbTopL);
                }
                else    Draw_Text_Remove();

                TextChanged();
            }
            // -----------------------------------
            else if(ch == ENTER )
            {
                if( MAX_TEXT == gbCurC->char_number) 
                        Draw_Text_RestLineClear(gbCurL->down, 0, 0);
                else     Draw_Text_RestLineClear(gbCurL, gbCurC, 0);
                
                Insert_Enter();
                TextChanged();

                if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
                {
                    gbTopL = gbTopL->down;
                    Clr_Text();
                    Draw_Scroll(gbTopL);
                }
                else  Draw_Text_Enter();
            }
            // -----------------------------------
            else if(ch>=SPACE)
            {
                Insert(ch);

                TextChanged();
                
                if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
                {
                    gbTopL = gbTopL->down;
                    Clr_Text();
                    Draw_Scroll(gbTopL);
                }
                else  Draw_Text_Input(gbCurL, gbCurC);
            }
            else if(ch == TAB)
            {
                TmpC = gbCurC;
                for(i = 0; i<TAB_CNT; i++)
                {
                    Insert(' ');
                }
                TextChanged();
                
                if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
                {
                    gbTopL = gbTopL->down;
                    Clr_Text();
                    Draw_Scroll(gbTopL);
                }
                else  Draw_Text_Input(gbCurL, TmpC);
            }
        }    
    }
    
    FreeAll();
    return 0;
}

void BackSpace_Key()
{

}
void Delete_Key()
{
    if(gbCurC == 0)     
    {
        gbCurC = gbCurL->char_point;
        if(gbCurC->character == '\n')     
        {
            gbCurL = gbCurL->down;
            gbCurC = 0;
        }
    }
    else                
    {
        gbCurC = gbCurC->next;
        //      
        if(gbCurC->character == '\n')
        {
            gbCurL = gbCurL->down;
            gbCurC = 0;
        }
        //     
        else if(gbCurC == 0)
        {
            gbCurL = gbCurL->down;
            //     
            if(gbCurL->char_point->character != '\n')    
            {
                gbCurC = gbCurL->char_point;
            }
            //        
            else
            {
                gbCurL = gbCurL->down;
                gbCurC = 0;
            }
        }
    }
    
    if(gbCurC == 0)    DeleteUp();
    else                Delete();

    if(gbCurL->line_number < gbTopL->line_number)
    {
        gbTopL = gbTopL->up;
        Clr_Text();
        Draw_Scroll(gbTopL);
    }
    else    Draw_Text_Remove(gbCurL, gbCurC);
}

/*
else
{
    VGA_CHAR(1, 15, '0'+ch/100);
    VGA_CHAR(2, 15, '0'+(ch%100)/10);
    VGA_CHAR(3, 15, '0'+ch%10);
}
    x = (int)(Head->down);
        VGA_CHAR(5, 15, MakeHex((x&0x0000F000)>>12));
        VGA_CHAR(6, 15, MakeHex((x&0x00000F00)>>8));
        VGA_CHAR(7, 15, MakeHex((x&0x000000F0)>>4));
        VGA_CHAR(8, 15, MakeHex((x&0x0000000F)>>0));
    VGA_CHAR(1, 15, '0'+x/100);
    VGA_CHAR(2, 15, '0'+(x%100)/10);
    VGA_CHAR(3, 15, '0'+x%10);
    VGA_CHAR(1, 16, '0'+y/100);
    VGA_CHAR(2, 16, '0'+(y%100)/10);
    VGA_CHAR(3, 16, '0'+y%10);
    VGA_CHAR(1, 17, '0'+c/100);
    VGA_CHAR(2, 17, '0'+(c%100)/10);
    VGA_CHAR(3, 17, '0'+c%10);
*/
test