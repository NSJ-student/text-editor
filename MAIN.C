#include <stdio.h>
#include <conio.h>
#include "default.h"
#include "draw.h"
#include "draw_c.h"
#include "cursor.h"
#include "text.h"
#include "menu.h"

stLine * gbHead = 0;			// 문자가 저장된 공간 맨 앞을 가리키는 포인터
stLine * gbCurL = 0;				// 현재 커서가 위치한 공간을 가리키는 포인터
stLine * gbTopL = 0;
int gbTopCharNum = 0;
stChar * gbCurC = 0;			// 현재 커서가 위치한 공간을 가리키는 포인터

void Delete_Key();

int main(void)
{
	char ch = 0;					// 입력받은 문자
	char key = 0;					// 특수키 입력
	stChar * TmpC;
	int i;
	
	Set_File_Path(DEFAULT_PATH);
	clrscr();
	Clr_All();
	Draw_MainWindow();

	if(Init() == FAIL)	return 0;
	
	Draw_Text_Cursor();
	
	while(ALT_X != key)
	{
		ch = getch();
		if( ch == 0 )	// 특수 키가 입력됨
		{
			key = getch();			// 특수키 저장
			switch(key)
			{
				// F10을 누르면 메뉴로 포커스를 이동
				case F10: 		if( END == Menu()) 
								{	// EXIT나 ALT_X
									FreeAll();
									return 0;
								}
								Draw_Text_Cursor();
								break;
				
				case R_ARROW :   Draw_Text_RMove(); break;
				case L_ARROW :	Draw_Text_LMove(); 	break;
				case U_ARROW : 	Draw_Text_UMove();	break;
				case D_ARROW : 	Draw_Text_DMove();	break;
				
				case HOME_KEY:	gbCurC = 0;	
								gbTopCharNum = 0;
								Draw_Text_All();	break;
				case END_KEY:	Draw_Text_End();	break;
				case PAGE_UP:	Draw_Text_PgUp(); 	break;
				case PAGE_DOWN: Draw_Text_PgDn(); 	break;
				
				case DELETE : 	if(( 0 == gbCurC->next )&&( 0 == gbCurL->down ))	
									continue;
								Delete_Key();	break;
				case F1 : 		break;
				default: 			break;
			}
		}
		else
		{
			switch(ch)
			{
				case BACK_SPACE : 
					if(( 0 == gbCurC ) && ( 0 == gbCurL->up ))	continue;
					
					if(gbCurC == 0)	BackSpaceUp();
					else				BackSpace();
					
					break;
					
				case ENTER :	Insert_Enter();	break;
				
				case TAB : 
					for(i = 0; i<TAB_CNT; i++)	
					{
						Insert(' ');
					}	break;
					
				default : 
					if(ch>=SPACE)	
					{
						Insert(ch);
					}	break;
						
			}
			
			TextChanged();
			Draw_Text_All();
		}	
	}
	
	FreeAll();
	return 0;
}

void Delete_Key()
{
	if(gbCurC == 0) 	
	{
		gbCurC = gbCurL->char_point;
	}
	else if(gbCurC->next == 0)		// 만약 커서가 라인 맨 뒤면
	{
		gbCurL = gbCurL->down;
		gbCurC = gbCurL->char_point;
	}
	else				
	{
		gbCurC = gbCurC->next;
	}

	if(gbCurC->character == '\n') 	
	{
		gbCurL = gbCurL->down;
		gbCurC = 0;
	}
	
	if(gbCurC == 0)	BackSpaceUp();
	else				BackSpace();


	TextChanged();
	Draw_Text_All();
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