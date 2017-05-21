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

	stUpC = (gbCurL->up)->char_point;	// ������ �� �� ����
	
	if( stUpC->character == '\n' )		// �� ���ο� ���͸� �����ϸ�
	{
		free(stUpC);
		stTmpL = gbCurL->up;
		
		// �� ���� ����
		DELETE_ULINE(gbCurL, stTmpL);
		
		if(stTmpL == gbHead)
		{
			gbHead = gbCurL;
			gbTopL = gbCurL;
		}
		
		// line_number ����
		stTmpL = gbCurL;
		LINE_ALIGN(stTmpL);
		
		gbTopCharNum = 0;
		return GOOD;
	}
	
	gbCurC = gbCurL->char_point;	// ���� ���� ù ����
	while( 0 != stUpC->next )	 
	{
		stUpC = stUpC->next;	// ������ ������ ���ڷ� �̵�
	}
	PrevC = stUpC->prev;
	DELETE_PCHAR(gbCurC, stUpC);	// ���� �հ� ���� ���ڸ� �մ´�

	gbCurC = PrevC;	// Ŀ���� �� ĭ ������
	gbCurL = gbCurL->up;			// ������ �� ĭ ����
	gbCurL->line_count = gbCurL->line_count + Cur_LineCount - 1;

	// ���� ��ȣ ����
	CHAR_ALIGN(PrevC);
	// ���� ����
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

// ---------------------------------		���� & ����
	(CurL->line_count)--;
	
	if( 0 == stTmpC->prev )	CurL->char_point = stTmpC->next;		// �� ���ڿ� �� ���ڸ� ����
	else 					(stTmpC->prev)->next = stTmpC->next;	
	
	if( 0 != stTmpC->next )	(stTmpC->next)->prev = stTmpC->prev;	// �� ���ڸ� �� ���ڶ� ����

	gbCurC = stTmpC->prev;
	free(stTmpC);		// ���� ��ġ�� ����

//---------------------------------- 	�غ�

	// ���� �� �ؽ�Ʈ�� ������ ����
	if(( CurL->line_number == 1 )&&(CurL->line_count == 0))	return GOOD;		

	if( gbCurC == 0)						// ���� �� Ŀ���� �� ��
	{
		(CurL->char_point)->char_number = 1;	// �� �ձ����� char_number�� 1�� �ʱ�ȭ
		stTmpC = (CurL->char_point)->next;		// stTmpC�� ���� ��ġ 2 �� 
	}
	else		stTmpC = gbCurC;			// ���� �� Ŀ���� �� ���� �ƴ�
	
//----------------------------------	char_number ����
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
// �� �տ� ����
//ȭ�鿡 �� �� ����ڿ� ����
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
//�߰� & �� �ڿ��� ����
	else
	{
		// ���� ���� ����
		NextC = gbCurC->next;
		NxtLineCnt = gbCurL->line_count - gbCurC->char_number;
		// ���� ���� ���� ����
		NewC->char_number = (gbCurC->char_number) + 1;
		NewC->prev = gbCurC;
		gbCurC->next = NewC;
		gbCurL->line_count = NewC->char_number;
		
		// ���� �޹��ڵ��� ���� �� ���� ����
		gbCurL = MakeNewLine(gbCurL, NextC, NxtLineCnt);
		gbCurC = 0;

		// �� ���ο� �� ���ڵ��� char_number�� ����
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

	// line_number ����
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
