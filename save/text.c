#include <stdlib.h>
#include <stdio.h>
#include "default.h"
#include "draw.h"
#include "draw_c.h"
#include "text.h"

void ArrangeLineInsert(stLine * Line, stChar * Char);
void ArrangeLineDelete(stLine * Line, stChar * Char);
stLine * MakeNewLine(stLine * CurLine, stChar * CurChar, int LineCnt);

char MakeHex(int In)
{
	if(In < 10)	return '0'+In;
	else			return 'A'+(In%10);
}

int Init()
{
	stLine * stTmpL;
	
	stTmpL = (stLine *)malloc(sizeof(stLine));
	if( 0 == stTmpL)	return FAIL;
	
	stTmpL->down = 0;
	stTmpL->up = 0;
	stTmpL->line_count = 0;
	stTmpL->line_number = 1;
	stTmpL->char_point = 0;

	gbHead = stTmpL;
	gbCurL = stTmpL;
	gbCurC = 0;
	gbTopL = gbHead;

	return GOOD;
}

int DeleteUp()
{
	stLine * stTmpL	= gbCurL;
	stChar * stTmpC 	= gbCurC;
	stChar * NextC;
	int Cur_LineCount = gbCurL->line_count;

	if( 0 == gbCurL)	return FAIL;

	stTmpC = (gbCurL->up)->char_point;
	
	if( stTmpC->character == '\n' )		// 윗 라인에 엔터만 존재하면
	{
		// 윗 라인 삭제
		stTmpL = gbCurL->up;
		DELETE_ULINE(gbCurL, stTmpL);
		// 엔터 삭제
		free(stTmpC);
		// line_number 정리
		stTmpL = gbCurL;
		LINE_ALIGN(stTmpL);
		
		return GOOD;
	}
	
	gbCurC = gbCurL->char_point;	// 윗줄에 붙일 문자
	while( 0 != stTmpC->next )	 
	{
		stTmpC = stTmpC->next;	// 윗줄의 마지막 글자로 이동
	}
	NextC = stTmpC->prev;
	DELETE_PCHAR(gbCurC, stTmpC);	// 엔터 앞과 현재 문자를 잇는다

	gbCurC = NextC;	// 커서를 한 칸 앞으로
	gbCurL = gbCurL->up;			// 라인을 한 칸 위로
	gbCurL->line_count = gbCurL->line_count + Cur_LineCount - 1;
	
	if( gbCurL->line_count <= MAX_TEXT )
	{
		stTmpC = gbCurC->next;		CHAR_ALIGN(stTmpC);
		stTmpL = gbCurL->down;		DELETE_DLINE(gbCurL, stTmpL);
		stTmpL = gbCurL->down;		LINE_ALIGN(stTmpL);
	}
	else
	{
		ArrangeLineDelete(gbCurL, gbCurC->next);
	}

	return GOOD;
}

int Delete()
{
	stLine * CurL	= gbCurL;
	stChar * stTmpC 	= gbCurC;

	if( 0 == gbCurL)	return FAIL;

// ---------------------------------		삭제 & 연결
	(CurL->line_count)--;
	
	if( 0 == stTmpC->prev )	CurL->char_point = stTmpC->next;		// 앞 문자에 뒷 문자를 붙임
	else 					(stTmpC->prev)->next = stTmpC->next;	
	
	if( 0 != stTmpC->next )	(stTmpC->next)->prev = stTmpC->prev;	// 뒷 문자를 앞 문자랑 연결

	gbCurC = stTmpC->prev;
	free(stTmpC);		// 현재 위치의 삭제

//---------------------------------- 	준비

	// 삭제 후 텍스트가 없으면 종료
	if(( CurL->line_number == 1 )&&(CurL->line_count == 0))	return GOOD;		

	if( gbCurC == 0)						// 삭제 후 커서가 맨 앞
	{
		(CurL->char_point)->char_number = 1;	// 맨 앞글자의 char_number를 1로 초기화
		stTmpC = (CurL->char_point)->next;		// stTmpC를 글자 위치 2 로 
	}
	else		stTmpC = gbCurC->next;			// 삭제 후 커서가 맨 앞이 아님
//----------------------------------	char_number 수정

	if(	CurL->line_count == MAX_TEXT - 1) 	ArrangeLineDelete(CurL, stTmpC);
	else 								{  CHAR_ALIGN(stTmpC);  }

	return GOOD;
}

int Insert(char ch)
{
	stChar * NewC;
	stLine * CurL = gbCurL;
	
	if( 0 == gbCurL )	return FAIL;

	NewC = (stChar *)malloc(sizeof(stChar));
	if( 0 == NewC )	return FAIL;
	
	NewC->character = ch;
// ----------------------------	삽입 & 연결
	if( 0 == gbCurC)
	{
		NewC->char_number = 1;
		NewC->prev = 0;
		NewC->next = CurL->char_point;
		
		if( 0 != CurL->char_point )  (CurL->char_point)->prev = NewC;
		CurL->char_point = NewC;
		
		gbCurC = NewC;
	}
	else if( MAX_TEXT == gbCurC->char_number )
	{
		if(CurL->down == 0)	
		{
			NewC->char_number = 1;
			NewC->prev = 0;
			NewC->next = 0;
			gbCurL = MakeNewLine(CurL, NewC, 1);
			gbCurC = NewC;
			return GOOD;
		}
		else	
		{
			NewC->char_number = 1;
			NewC->prev = gbCurC;
			NewC->next = 0;
			
			gbCurC->next = NewC;
		}
	}
	else
	{
		NewC->char_number = gbCurC->char_number + 1;
		NewC->prev = gbCurC;
		NewC->next = gbCurC->next;
		
		if( 0 != gbCurC->next )		(gbCurC->next)->prev = NewC;
		gbCurC->next = NewC;
		
		gbCurC = NewC;
	}

// --------------------------- 	라인 정리
	if( CurL->line_count == MAX_TEXT )
	{
		MakeNewLine(CurL, 0, 0);
		 if( 0 == NewC->next )		gbCurL = CurL->down;
		ArrangeLineInsert(CurL, gbCurC );
		
		gbCurC = NewC;
	}
	else
	{
		gbCurL->line_count = gbCurL->line_count + 1;
		
		NewC = NewC->next;
		CHAR_ALIGN(NewC)
	}
	
	return GOOD;
}
int Insert_Enter()
{
	stChar * stTmpC;
	stChar * NextC;
	stLine * stTmpL;
	int NxtLineCnt;

	stTmpC = (stChar *)malloc(sizeof(stChar));
	if( 0 == stTmpC )	return FAIL;
	
	stTmpC->character = '\n';
	stTmpC->next = 0;

// 맨 앞에 엔터
//화면에 다 찬 문장뒤에 엔터
	if(      ( 0 == gbCurC) || ( MAX_TEXT== gbCurC->char_number )	)
	{
		stTmpC->char_number = 1;
		stTmpC->prev = 0;

		if( 0 == gbCurC)		gbCurL = gbCurL->up;

		gbCurL = MakeNewLine(gbCurL, stTmpC, 1);	// 엔터를 위한 새 라인

		gbCurL = gbCurL->down;
		if(0 == gbCurL)		gbCurL = MakeNewLine(gbCurL, 0, 0);
		gbCurC = 0;
		
		return GOOD;
	}
//중간 & 맨 뒤에서 엔터
	else
	{
		// 다음 라인 정보
		NextC = gbCurC->next;
		NxtLineCnt = gbCurL->line_count - gbCurC->char_number;
		// 현재 라인 정보 정리
		stTmpC->char_number = (gbCurC->char_number) + 1;
		stTmpC->prev = gbCurC;
		gbCurC->next = stTmpC;
		gbCurL->line_count = stTmpC->char_number;
		
		// 엔터 뒷문자들을 담을 새 라인 생성
		gbCurL = MakeNewLine(gbCurL, NextC, NxtLineCnt);
		gbCurC = 0;

		// 새 라인에 들어갈 문자들의 char_number를 조절
		if( 0 == NextC ) 	return GOOD;
		NextC->char_number = 1;
		NextC->prev = 0;

		ArrangeLineInsert(gbCurL, NextC);
		return GOOD;
	}
}

void FreeAll()
{
	stChar * FreeC;
	stLine * FreeL;
	stChar * CurC;
	stLine * CurL = gbHead;
	
	while( 0 != CurL)
	{
		CurC= CurL->char_point;
		while( 0 != CurC)
		{
			FreeC = CurC;
			CurC = CurC->next;
			free(FreeC);
		}
		
		FreeL = CurL;
		CurL= CurL->down;
		free(FreeL);
	}
}

void ArrangeLineInsert(stLine * Line, stChar * Char)
{
	stLine * NewLine;
	stLine * TmpL;
	stChar * NextC;
	
	while( 0 != Char )
	{
		if(Char->prev != 0) Char->char_number = Char->prev->char_number + 1;
		if(( Char->char_number == MAX_TEXT)&&( 0 != Char->next ))
		{
			Line->line_count = MAX_TEXT;

			NextC = Char->next;
			NextC->prev = 0;
			NextC->char_number = 1;
			
			Char->next = 0;
			Char = NextC;
			
			Line = Line->down;
			Line->char_point = NextC;
		}
		if( 0 == Char->next )
		{
			if(( '\n' == Char->character )||( 0 == Line->down ))
			{
				Line->line_count = Char->char_number;

				return;
			}
			else		// 아래줄의 문자를 위로 댕긴다
			{		
				NextC = Line->down->char_point;
				Char->next = NextC;
				NextC->prev = Char;

				if( Line->down->line_count +Char->char_number <= MAX_TEXT )
				{
					TmpL = Line->down;		DELETE_DLINE(Line, TmpL);
					TmpL = Line->down;		LINE_ALIGN(TmpL);
				}
			}
		}
		Char = Char->next;
	}
}

void ArrangeLineDelete(stLine * Line, stChar * Char)
{
	stLine * NewLine;
	stLine * TmpL;
	stChar * NextC;
	
	while( 0 != Char )
	{
		if(Char->prev != 0) Char->char_number = Char->prev->char_number + 1;
		if( 0 == Char->next )
		{
			if(( '\n' == Char->character )||( 0 == Line->down ))
			{
				Line->line_count = Char->char_number;
				return;
			}
			else		// 아래줄의 문자를 위로 댕긴다
			{		
				NextC = Line->down->char_point;
				Char->next = NextC;
				NextC->prev = Char;

				if( Line->down->line_count +Char->char_number <= MAX_TEXT )
				{
					TmpL = Line->down;	DELETE_DLINE(Line, TmpL);
					TmpL = Line->down;	LINE_ALIGN(TmpL);
				}
			}
		}
		else if( Char->char_number == MAX_TEXT)	
		{
			Line->line_count = MAX_TEXT;
			Line = Line->down;

			NextC = Char->next;
			NextC->prev = 0;
			NextC->char_number = 1;
			Char->next = 0;
			Char = NextC;
			
			Line->char_point = NextC;
			Line->line_count = 1;
		}
		Char = Char->next;
	}
}

stLine * MakeNewLine(stLine * CurLine, stChar * CurChar, int LineCnt)
{
	stLine * NewLine;
	stLine * stTmpL;
	
	NewLine = (stLine *)malloc(sizeof(stLine));
	if(NewLine == 0) 	return 0;
	
	NewLine->up 			= CurLine;
	NewLine->down 		= CurLine->down;
	NewLine->line_number 	= CurLine->line_number+1;
	NewLine->line_count 	= LineCnt;
	NewLine->char_point 	= CurChar;

	if(CurLine->down != 0)	(CurLine->down)->up = NewLine;
	CurLine->down = NewLine;

	// line_number 조절
	stTmpL = NewLine->down;
	LINE_ALIGN(stTmpL);
	
	return NewLine;
}
/*
void Free_AllC(stLine * stHead)
{
	stChar * FreeC;
	stChar * CurC = stHead->char_point;
	stLine * CurL = stHead;
	
	while( 0 != CurL)
	{
		CurC= CurL->char_point;
		while( 0 != CurC)
		{
			FreeC = CurC;
			CurC = CurC->next;
			free(FreeC);
		}
		CurL= CurL->down;
	}
}
void Free_AllL(stLine * stHead)
{
	stLine * FreeL;
	stLine * CurL = stHead->down;
	
	while( 0 != CurL)
	{
		FreeL = CurL;
		CurL= CurL->down;
		free(FreeL);
	}
}
*/
