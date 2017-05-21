#include <stdlib.h>
#include <stdio.h>
#include "default.h"
#include "draw.h"
#include "draw_c.h"
#include "text.h"

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
	gbTopCharNum = 0;
	gbTopL = gbHead;

	return GOOD;
}

int BackSpaceUp()
{
	stLine * stTmpL	= gbCurL;
	stChar * stUpC 	= gbCurC;
	stChar * PrevC;
	int Cur_LineCount = gbCurL->line_count;

	if( 0 == gbCurL)	return FAIL;

	stUpC = (gbCurL->up)->char_point;	// 윗줄의 맨 앞 문자
	
	if( stUpC->character == '\n' )		// 윗 라인에 엔터만 존재하면
	{
		free(stUpC);
		stTmpL = gbCurL->up;
		
		// 윗 라인 삭제
		DELETE_ULINE(gbCurL, stTmpL);
		
		if(stTmpL == gbHead)
		{
			gbHead = gbCurL;
			gbTopL = gbCurL;
		}
		
		// line_number 정리
		stTmpL = gbCurL;
		LINE_ALIGN(stTmpL);
		
		gbTopCharNum = 0;
		return GOOD;
	}
	
	gbCurC = gbCurL->char_point;	// 현재 줄의 첫 문자
	while( 0 != stUpC->next )	 
	{
		stUpC = stUpC->next;	// 윗줄의 마지막 글자로 이동
	}
	PrevC = stUpC->prev;
	DELETE_PCHAR(gbCurC, stUpC);	// 엔터 앞과 현재 문자를 잇는다

	gbCurC = PrevC;	// 커서를 한 칸 앞으로
	gbCurL = gbCurL->up;			// 라인을 한 칸 위로
	gbCurL->line_count = gbCurL->line_count + Cur_LineCount - 1;

	// 문자 번호 정리
	CHAR_ALIGN(PrevC);
	// 라인 정리
	stTmpL = gbCurL->down;		DELETE_DLINE(gbCurL, stTmpL);
	stTmpL = gbCurL->down;		LINE_ALIGN(stTmpL);

	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
	}
	else
	{
		gbTopCharNum = 0;
	}
	return GOOD;
}

int BackSpace()
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
	else		stTmpC = gbCurC;			// 삭제 후 커서가 맨 앞이 아님
	
//----------------------------------	char_number 수정
	CHAR_ALIGN(stTmpC); 

	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
	}
	else
	{
		gbTopCharNum = 0;
	}
	
	return GOOD;
}

int Insert(char ch)
{
	stChar * NewC;
	stChar ** pPrevC_next;
	stChar * NextC;
	stLine * CurL = gbCurL;
	
	if( 0 == gbCurL )	return FAIL;
	
	NewC = (stChar *)malloc(sizeof(stChar));
	if( 0 == NewC )	return FAIL;
	
	NewC->character = ch;
	if( 0 == gbCurC)
	{
		NewC->next = CurL->char_point;
		NewC->prev = 0;
		
		if(0 != CurL->char_point->prev)		CurL->char_point->prev = NewC;
		CurL->char_point = NewC;
	}
	else
	{
		if( 0 != gbCurC->next )	gbCurC->next->prev = NewC;
		
		NewC->next = gbCurC->next;
		NewC->prev = gbCurC;
		NewC->char_number = gbCurC->char_number + 1;
		gbCurC->next = NewC;
	}
	gbCurC = NewC;

	gbCurL->line_count += 1;
	CHAR_ALIGN(NewC);
	
	if(gbCurC != 0)
	{
		gbTopCharNum = gbCurC->char_number  - MAX_TEXT;
		if(gbTopCharNum < 0)
		{
			gbTopCharNum = 0;
		}
	}
	else
	{
		gbTopCharNum = 0;
	}
	
	return GOOD;
}
int Insert_Enter()
{
	stChar * NewC;
	stChar * NextC;
	stLine * stTmpL;
	int NxtLineCnt;

	NewC = (stChar *)malloc(sizeof(stChar));
	if( 0 == NewC )	return FAIL;
	
	NewC->character = '\n';
	NewC->next = 0;

	gbTopCharNum = 0;
// 맨 앞에 엔터
//화면에 다 찬 문장뒤에 엔터
	if( 0 == gbCurC)
	{
		NewC->char_number = 1;
		NewC->prev = 0;

		MakeNewLine(gbCurL, gbCurL->char_point, gbCurL->line_count);
		gbCurL->char_point = NewC;
		gbCurL->line_count = 1;
		gbCurL = gbCurL->down;

		return GOOD;
	}
//중간 & 맨 뒤에서 엔터
	else
	{
		// 다음 라인 정보
		NextC = gbCurC->next;
		NxtLineCnt = gbCurL->line_count - gbCurC->char_number;
		// 현재 라인 정보 정리
		NewC->char_number = (gbCurC->char_number) + 1;
		NewC->prev = gbCurC;
		gbCurC->next = NewC;
		gbCurL->line_count = NewC->char_number;
		
		// 엔터 뒷문자들을 담을 새 라인 생성
		gbCurL = MakeNewLine(gbCurL, NextC, NxtLineCnt);
		gbCurC = 0;

		// 새 라인에 들어갈 문자들의 char_number를 조절
		if( 0 == NextC ) 	return GOOD;
		NextC->char_number = 1;
		NextC->prev = 0;
		
		CHAR_ALIGN(NextC);
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
