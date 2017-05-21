#include <string.h>
#include "default.h"
#include "cursor.h"
#include "draw.h"
#include "draw_c.h"

static int cur_x = POS_TEXT_X;	// cursor x-axis
static int cur_y = POS_TEXT_Y;	// cursor y-axis

void Draw_Text_Cursor()
{
	int x;
	int y;
	
	if( gbCurC == 0 )	x = POS_TEXT_X;
	else				x = POS_TEXT_X + gbCurC->char_number;
	
	y = POS_TEXT_Y + gbCurL->line_number - gbTopL->line_number;
	
	Move_Cursor(x, y);
	/*
	VGA_CHAR(POS_TEXT_X+MAX_TEXT, y, 0);
	x = gbCurC->next;
	VGA_CHAR(60, 1, '0'+x/100);
	VGA_CHAR(61, 1, '0'+(x%100)/10);
	VGA_CHAR(62, 1, '0'+x%10);
	x = gbCurC;
	VGA_CHAR(60, 2, '0'+x/100);
	VGA_CHAR(61, 2, '0'+(x%100)/10);
	VGA_CHAR(62, 2, '0'+x%10);
	y = gbCurC->prev;
	VGA_CHAR(60, 3, '0'+y/100);
	VGA_CHAR(61, 3, '0'+(y%100)/10);
	VGA_CHAR(62, 3, '0'+y%10);
	y = gbCurC->char_number;
	VGA_CHAR(60, 4, '0'+y/100);
	VGA_CHAR(61, 4, '0'+(y%100)/10);
	VGA_CHAR(62, 4, '0'+y%10);
	y = gbCurC->character;
	VGA_CHAR(60, 5, '0'+y/100);
	VGA_CHAR(61, 5, '0'+(y%100)/10);
	VGA_CHAR(62, 5, '0'+y%10);
	y = gbCurL->char_point;
	VGA_CHAR(60, 6, '0'+y/100);
	VGA_CHAR(61, 6, '0'+(y%100)/10);
	VGA_CHAR(62, 6, '0'+y%10);
	*/
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
	
	Draw_Text_Cursor();	
}

void Draw_Text_RMove()
{
	if( 0 == gbCurL->char_point ) 	return; 
	if(( 0 == gbCurC->next ) &&(0 == gbCurL->down))	return;

	if( 0 == gbCurC ) 	gbCurC = gbCurL->char_point;
	else if( gbCurC->char_number == MAX_TEXT )
	{
		gbCurL = gbCurL->down;
		gbCurC = 0;
	}
	else 	gbCurC = gbCurC->next;
	
	if( '\n' == gbCurC->character )
	{
		if(gbCurL->down != 0)	gbCurL = gbCurL->down;
		gbCurC = 0;
	}
	
	if(gbCurL->line_number >= gbTopL->line_number+TEXT_H)
	{
		gbTopL = gbTopL->down;
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_LMove()
{
	stChar * TmpC;
	
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
				while(( '\n' != TmpC->next->character )&&( TmpC->next != 0) )
				{
					TmpC = TmpC->next;
				}
				gbCurC = TmpC;
			}
		}
	}
	else gbCurC = gbCurC->prev;
	
	if(gbCurL->line_number < gbTopL->line_number)
	{
		gbTopL = gbTopL->up;
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_UMove()
{
	stChar * TmpC;
	
 	if(0 == gbCurL->up)	return;
	else 
	{
		gbCurL = gbCurL->up;
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
		Clr_Text();
		Draw_Scroll(gbTopL);
	}
	else 	Draw_Text_Cursor();
}

void Draw_Text_DMove()
{
	stChar * TmpC;
	
	if(0 == gbCurL->down)	return;
	else
	{
		gbCurL = gbCurL->down;
		
		if(gbCurL->char_point == 0)	gbCurC = 0;
		else if(0 != gbCurC)
		{
			TmpC = gbCurL->char_point;
			while( TmpC->char_number < gbCurC->char_number )
			{
				if( '\n' == TmpC->character ) 
				{
					TmpC = TmpC->prev;
					break;
				}
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
		for( Char = pTop->char_point; (Char != 0); Char = Char->next )
		{
			if( Char->character == '\n' )	break;
			x = POS_TEXT_X + Char->char_number - 1;
			VGA_CHAR(x, y, Char->character);
		}
		pTop = pTop->down;
		
		if(pTop->line_number > gbTopL->line_number+TEXT_H-1)	break;
	}
}

