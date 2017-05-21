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
		gbCurL->line_count += 1;
		
		NewC = NewC->next;
		CHAR_ALIGN(NewC)
	}
	return GOOD;
}
