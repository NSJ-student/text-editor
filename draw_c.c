#include <string.h>
#include "default.h"
#include "cursor.h"
#include "draw.h"
#include "draw_c.h"

void Draw_Text_All()
{
	
	Clr_Text();
	Draw_Scroll(gbTopL);
}

void Draw_Text_Cursor()
{
	int x;
	int y;
	
	if( gbCurC == 0 )	x = POS_TEXT_X;
	else				x = POS_TEXT_X + gbCurC->char_number - gbTopCharNum;
	
	y = POS_TEXT_Y + gbCurL->line_number - gbTopL->line_number;
	
	Move_Cursor(x, y);

	x = x-POS_TEXT_X+ gbTopCharNum;
	y = gbCurL->line_number;
	VGA_CHAR(70, POS_MENU_Y, '0'+y/100);
	VGA_CHAR(71, POS_MENU_Y, '0'+(y%100)/10);
	VGA_CHAR(72, POS_MENU_Y, '0'+y%10);
	VGA_CHAR(73, POS_MENU_Y, '-');
	VGA_CHAR(74, POS_MENU_Y, '0'+x/100);
	VGA_CHAR(75, POS_MENU_Y, '0'+(x%100)/10);
	VGA_CHAR(76, POS_MENU_Y, '0'+x%10);
}

void Draw_Text_PgUp()
{	
	int i;
	
	for(i = 0; (i<TEXT_H-1)&&(gbTopL->up != 0); i++)
	{
		gbTopL = gbTopL->up;
	}
	
	gbCurL = gbTopL;
	gbCurC = 0;
	Clr_Text();
	Draw_Scroll(gbTopL);
	Draw_Text_Cursor();
}
void Draw_Text_PgDn()
{	
	int i;
	
	for(i = 0; (i<TEXT_H-1)&&(gbTopL->down != 0); i++)
	{
		gbTopL = gbTopL->down;
	}
	
	gbCurL = gbTopL;
	gbCurC = 0;
	Clr_Text();
	Draw_Scroll(gbTopL);
	Draw_Text_Cursor();
}
void Draw_Text_End()
{
	if(gbCurC == 0)	gbCurC = gbCurL->char_point;

	if(gbCurC != 0)
	{
		while(gbCurC->next != 0)
		{
			gbCurC = gbCurC->next;
		}
	}
	
	if(gbCurC->character == '\n' ) gbCurC = gbCurC->prev;

	if(gbCurC == 0)
	{
		gbTopCharNum=0;
	}
	else if(gbCurC->char_number > MAX_TEXT)
	{
		gbTopCharNum = gbCurC->char_number + 1 - MAX_TEXT;
	}
	
	Draw_Text_All();
}

void Draw_Text_RMove()
{
	int iPrevChar = 0;
	int bDrawFlag = 0;
	
	if( 0 == gbCurL->char_point ) 	return; 
	if(( 0 == gbCurC->next ) &&(0 == gbCurL->down))	return;

	if( 0 == gbCurC ) 	gbCurC = gbCurL->char_point;
	else 			gbCurC = gbCurC->next;
	
	if( '\n' == gbCurC->character )
	{
		gbCurL = gbCurL->down;
		gbCurC = 0;
	}
	
	iPrevChar = gbTopCharNum;
	
	if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
	{
		gbTopL = gbTopL->down;
		bDrawFlag = 1;
	}
	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
		
		if(iPrevChar != gbTopCharNum)	bDrawFlag = 1;
	}
	else
	{
		gbTopCharNum = 0;
		if(iPrevChar != 0)	 bDrawFlag = 1;
	}
	
	if(bDrawFlag == 1)
	{
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_LMove()
{
	int bDrawFlag = 0;
	int iPrevChar = 0;
	stChar * TmpC;
	
	iPrevChar = gbTopCharNum;
	if( 0 == gbCurC ) 
	{
		if(0 == gbCurL->up)	return;		// 커서 위치가 맨 앞이면 종료
		else
		{
			gbCurL = gbCurL->up;
			TmpC = gbCurL->char_point;
			if(TmpC->character== '\n') 	gbCurC = 0;
			else
			{
				while( '\n' != TmpC->next->character)
				{
					TmpC = TmpC->next;
				}
				gbCurC = TmpC;
			}
		}
	}
	else 
	{
		gbCurC = gbCurC->prev;
	}
	
	if(gbCurL->line_number < gbTopL->line_number)
	{
		gbTopL = gbTopL->up;
		bDrawFlag = 1;
	}
	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
		
		if(iPrevChar != gbTopCharNum)	bDrawFlag = 1;
	}
	else
	{
		gbTopCharNum = 0;
		if(iPrevChar != 0)	 bDrawFlag = 1;
	}
	
	if(bDrawFlag == 1)
	{
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_UMove()
{
	int bDrawFlag = 0;
	int iPrevChar = 0;
	stChar * TmpC;
	
 	if(0 == gbCurL->up)	return;
	else 
	{
		gbCurL = gbCurL->up;
		iPrevChar = gbTopCharNum;
		if(0 != gbCurC)
		{
			TmpC = gbCurL->char_point;
			while( TmpC->char_number < gbCurC->char_number )
			{
				if( '\n' == TmpC->character ) 
				{
					TmpC = TmpC->prev;
					break;
				}
				TmpC = TmpC->next;
			}
			if( '\n' == TmpC->character ) 	gbCurC = TmpC->prev;
			else 						gbCurC = TmpC;
		}
	}
	
	if(gbCurL->line_number < gbTopL->line_number)
	{
		gbTopL = gbTopL->up;
		bDrawFlag = 1;
	}
	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
		
		if(iPrevChar != gbTopCharNum)	bDrawFlag = 1;
	}
	else
	{
		gbTopCharNum = 0;
		if(iPrevChar != 0)	 bDrawFlag = 1;
	}
	
	if(bDrawFlag == 1)
	{
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_DMove()
{
	int bDrawFlag = 0;
	int iPrevChar = 0;
	stChar * TmpC;
	
	if(0 == gbCurL->down)	return;
	else
	{
		gbCurL = gbCurL->down;
		iPrevChar = gbTopCharNum;
		
		if(gbCurL->char_point == 0)	gbCurC = 0;
		else if(0 != gbCurC)
		{
			TmpC = gbCurL->char_point;
			while( TmpC->char_number < gbCurC->char_number )
			{
				if( '\n' == TmpC->character )  break;
				if( TmpC->next == 0 ) break;
				TmpC = TmpC->next;
			}
			if( '\n' == TmpC->character ) 	gbCurC = TmpC->prev;
			else 						gbCurC = TmpC;
		}
	}
	
	if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
	{
		gbTopL = gbTopL->down;
		bDrawFlag = 1;
	}
	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
		
		if(iPrevChar != gbTopCharNum)	bDrawFlag = 1;
	}
	else
	{
		gbTopCharNum = 0;
		if(iPrevChar != 0)	 bDrawFlag = 1;
	}
	
	if(bDrawFlag == 1)
	{
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else		Draw_Text_Cursor();
}

void Draw_Text_Input(stLine * Line, stChar * Char)
{	
	int check = 0;
	
	Draw_Text_Cursor();

	if( 0 == Char ) Char = Line->char_point;
	while( Line != 0 )
	{
		for( ; (Char != 0); Char = Char->next )
		{
			if( Char->character == '\n' )
			{
				Draw_Text_RestLineClear(Line, Char, -1);
				if((Char->char_number == 1) ||(check == 1))
				{
					check = 1;
					break;
				}
				else		return;
			}
			VGA_CHAR(	POS_TEXT_X + Char->char_number	- 1, 
						POS_TEXT_Y + Line->line_number 	- gbTopL->line_number, 
						Char->character);
		}
		Line = Line->down ;
		if(Line->line_number > gbTopL->line_number+TEXT_H-1)	break;
		Char = Line->char_point;
	}
}

void Draw_Text_Enter()
{	
	stLine * Line = gbCurL;
	stChar * Char = gbCurC;
	
	Draw_Text_Cursor();

	if( 0 == Char ) Char = Line->char_point;
	while( Line != 0 )
	{
		for( ; (Char != 0); Char = Char->next )
		{
			if( Char->character == '\n' )
			{
				Draw_Text_RestLineClear(Line, Char, -1);
				break;
			}
			VGA_CHAR(	POS_TEXT_X + Char->char_number	- 1, 
						POS_TEXT_Y + Line->line_number 	- gbTopL->line_number, 
						Char->character);
		}
		Line = Line->down ;
		if(Line->line_number > gbTopL->line_number+TEXT_H-1)	break;
		Char = Line->char_point;
	}
}

void Draw_Text_RestLineClear(stLine * Line, stChar * Char, int offset)
{	
	int x = 0;
	int y = 0;

	if( 0 == Char ) 	x = POS_TEXT_X + offset;
	else 			x = POS_TEXT_X + Char->char_number + offset;
	y = POS_TEXT_Y + Line->line_number - gbTopL->line_number;
	
	while( x < POS_TEXT_X + MAX_TEXT)
	{
		VGA_CHAR(x, y, 0);
		x++;
	}
}

void Draw_Text_Remove()
{
	int x;
	int y;
	stLine * Line = gbCurL;
	stChar * Char = gbCurC;

	Draw_Text_Cursor();

	if( 0 == Line->char_point)
	{
		x = POS_TEXT_X;
		y = POS_TEXT_Y + Line->line_number - gbTopL->line_number;
		VGA_CHAR(x, y, 0);
		return;
	}
	if( 0 == Char ) Char = Line->char_point;
	
	while( Line != 0 )
	{
		y = POS_TEXT_Y + Line->line_number - gbTopL->line_number;
		if(Char == 0)	Draw_Text_RestLineClear(Line, Char, 0);
		for( ; (Char != 0); Char = Char->next )
		{
			if( Char->character == '\n' )
			{
				Draw_Text_RestLineClear(Line, Char, -1);
				break;
			}
			x = POS_TEXT_X + Char->char_number - 1;
			VGA_CHAR(x, y, Char->character);
			VGA_CHAR(x+1, y, ' ');
			if(  (Char->next == 0)&&(x < POS_TEXT_X+MAX_TEXT-1)  )
			{
				Draw_Text_RestLineClear(Line, Char, 0);
				break;
			}
		}
		Line = Line->down;
		if(Line->line_number > gbTopL->line_number+TEXT_H-1)	break;
		Char = Line->char_point;
	}

	if(y >= POS_TEXT_Y + TEXT_H -1)		return;
	
	for( x = POS_TEXT_X; x<POS_TEXT_X+MAX_TEXT-1; x++)
	{
		VGA_CHAR(x, y+1, ' ');
	}
	// Delete의 리턴값으로 아래줄 삭제확인으로 동작 결정
}

void Draw_Scroll(stLine * pTop)
{	
	int x = 0;
	int y = 0;
	stChar * Char;
	
	Draw_Text_Cursor();
	while( pTop != 0 )
	{
		y = POS_TEXT_Y + pTop->line_number - gbTopL->line_number;
		for( Char = pTop->char_point; (Char != 0)&&((Char->char_number - gbTopCharNum)<= MAX_TEXT); Char = Char->next )
		{
			if( Char->character == '\n' )	break;
			if(Char->char_number  > gbTopCharNum )
			{
				x = POS_TEXT_X + Char->char_number - gbTopCharNum-1 ;
				VGA_CHAR(x, y, Char->character);
			}
		}
		pTop = pTop->down;
		
		if(pTop->line_number > gbTopL->line_number+TEXT_H-1)	break;
	}
}

