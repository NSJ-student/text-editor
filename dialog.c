#include "default.h"
#include "dialog.h"

static char gbDPath[DIALOG_PATH_W-2];
char FileName[18];

void SetFileName()
{
	int head = 0;
	int i;

	for(i = 7; gbDPath[i] != 0; i++, head++)
	{
		FileName[head] = gbDPath[i];
	}
	FileName[head] = 0;
}

void SetNoName()
{
	int head = 0;
	int i;
	char * NON = "NO  NAME";

	for(i = 0; NON[i] != 0; i++, head++)
	{
		FileName[head] = NON[i];
	}
	FileName[head] = 0;
}

void Dialog_Error(char * msg)
{
	char asc;
	
	Draw_ErrorBox(msg);

	while( 1 )
	{
		asc = getch();
		if( (asc == ENTER)||(asc == ESC) )	break;
	}
}

char * Dialog_New(const BOOL set_path)
{
	int idx = 0;
	char asc = 0;
	char cmd = 0;
	CITEM cNow = C_SAVE;
	int path_length = 0;

	Get_File_Path(gbDPath);
	if(set_path == NO)
	{
		cNow = C_WINDOW;
		idx = Draw_DialogNew(gbDPath);
		path_length = idx;
	}
	else
	{
		Draw_DialogNew_Saved(gbDPath);
		Draw_DialogFocusNew(cNow, 0);
	}
	
	Cursor_On();
	while( asc != ESC)
	{
		asc = getch();		// �Է� ����
		if( asc == 0 )
		{
			cmd = getch();

			switch(cmd)
			{
				case R_ARROW: 	if(cNow != C_WINDOW) 
								{
									if(cNow == C_SAVE) 		cNow = C_DONT;
									else if(cNow == C_DONT) 	cNow = C_CANCEL;
								}
								else if( gbDPath[idx] != 0) idx++;
								break;
				case L_ARROW: 	if(cNow != C_WINDOW) 
								{
									if(cNow == C_DONT) 		cNow = C_SAVE;
									else if(cNow == C_CANCEL) 	cNow = C_DONT;
								}
								else if(idx> 0)			idx--;
								break;
				case D_ARROW:	if(cNow == C_WINDOW) 		cNow = C_SAVE; break;
				case U_ARROW: 	if(set_path == NO) 			cNow = C_WINDOW; break;
				default: 			continue;
			}
			
			if(cNow == C_WINDOW)
			{
				Cursor_On();
			}
			else
			{
				Cursor_Off();
			}
			Draw_DialogFocusNew(cNow, idx);
		}
		else if(asc == TAB)
		{
			cNow++;
			if(cNow > C_WINDOW)
			{
				cNow = C_SAVE;
			}
			Draw_DialogFocusNew(cNow, idx);
		}
		else if(asc == ENTER)
		{
			if((cNow == C_SAVE)||(cNow == C_WINDOW))
			{
				Cursor_Off();
				return gbDPath;
			}
			if(cNow == C_DONT)
			{
				Cursor_Off();
				return -1;
			}
			if(cNow == C_CANCEL)
			{
				Cursor_Off();
				return 0;
			}
		}
		else if(cNow == C_WINDOW)
		{
			if(asc == BACK_SPACE)
			{
				if(idx > 0)
				{
					path_length--;
					idx--;
					Draw_Array_Move_Left(gbDPath, idx);
					Move_Cursor(idx+POS_PATH_X, POS_PATH_Y);
				}
			}
			else
			{
				if(path_length < DIALOG_PATH_LEN+7)
				{
					Draw_Array_Move_Right(gbDPath, idx);
					gbDPath[idx] = asc;
					VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, asc);
					Move_Cursor(idx+POS_PATH_X+1, POS_PATH_Y);
					
					idx++;
					path_length++;
				}
			}
		}
	}

	Cursor_Off();
	return 0;
}


char * Dialog(MENU_ITEM mNow)
{
	int idx = 0;
	char asc = 0;
	char cmd = 0;
	SITEM sNow = S_WINDOW;
	int path_length = 0;

	Get_File_Path(gbDPath);
	// �̹� ��ΰ� ������ ���¿��� save�� ������ �׳� ��θ� ��ȯ
	if(mNow == SAVE)
		return gbDPath;

	// ��θ� �Է¹ޱ����� dialog�� �׸�
	if(mNow == SAVEAS)
		idx = Draw_Dialog_Save(gbDPath);
	if(mNow == LOAD)
		idx = Draw_Dialog_Load(gbDPath);

	path_length = idx;

	Cursor_On();
	while( asc != ESC)
	{
		asc = getch();		// �Է� ����
		if( asc == 0 )
		{
			cmd = getch();

			switch(cmd)	// ����Ű�� �̿��ؼ� focus �̵� & �Է�â���� �ε��� ��ġ �̵�
			{
				case R_ARROW: 	if(sNow != S_WINDOW) 	sNow = S_CANCEL; 
								else if( gbDPath[idx] != 0)		idx++;
								break;
				case L_ARROW: 	if(sNow != S_WINDOW) 	sNow = S_OK; 
								else if(idx> 7)			idx--;
								break;
				case D_ARROW: 	if(sNow == S_WINDOW) 	sNow = S_OK; 	break;
				case U_ARROW: 	sNow = S_WINDOW; 				break;
				default: 			continue;
			}
			
			if(sNow == C_WINDOW)
			{
				Cursor_On();
			}
			else
			{
				Cursor_Off();
			}
			Draw_DialogFocus(sNow, idx);
		}
		else if(asc == TAB)	// ���� �̿��ؼ� focus�� �̵�
		{
			sNow++;
			if(sNow > S_CANCEL)
			{
				sNow = S_WINDOW;
			}
			Draw_DialogFocus(sNow, idx);
		}
		else if(asc == ENTER)
		{
			if((sNow == S_OK)||(sNow == S_WINDOW))	// focus�� �Է�â�̳� OK�� ��� ��ȯ
			{
				Cursor_Off();
				return gbDPath;
			}
			if(sNow == S_CANCEL)		// focus�� cancel�̸� 0 ��ȯ
			{
				Cursor_Off();
				return 0;
			}
		}
		else if(sNow == S_WINDOW)
		{
			if(asc == BACK_SPACE)
			{
				if(idx > 7)
				{
					path_length--;			// ��� ���� 1 ����
					idx--;					// �ε��� ��ġ 1 ����
					Draw_Array_Move_Left(gbDPath, idx);				// ��� �迭�� �·� �� ĭ ����
					Move_Cursor(idx+POS_PATH_X, POS_PATH_Y);	// �ε��� ��ġ ǥ��
				}
			}
			else
			{
				if(path_length < DIALOG_PATH_LEN+7)	// ��ΰ� �ִ밪 ���� ������
				{
					Draw_Array_Move_Right(gbDPath, idx);	// ��� �迭�� ��� �� ĭ �ٹδ�
					gbDPath[idx] = asc;						// �Է� ���ڸ� ���� �ε����� �ִ´�
					VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, asc);		// �Է� ���ڸ� ȭ�鿡 ���
					Move_Cursor(idx+POS_PATH_X+1, POS_PATH_Y);		// �ε��� ��ġ ǥ��
					
					idx++;				// �ε��� ��ġ 1 ����
					path_length++;		// ��� ���� 1 ����
				}
			}
		}
	}

	Cursor_Off();
	return 0;
}

void Draw_Array_Move_Left(char path[], int idx)
{
	if(*(path+idx+1) == '\0') 
	{
		*(path+idx) = '\0';
		VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, 0);
		return;
	}
	
	while( *(path+idx) != '\0' )
	{
		*(path+idx) = *(path+idx+1);
		VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, *(path+idx));
		idx++;
	}
}
void Draw_Array_Move_Right(char path [ ], int idx)
{
	int iSeeker = idx;

	for(; *(path +iSeeker) != '\0'; iSeeker++);
	
	if(iSeeker == idx) 
	{
		*(path+iSeeker+1) = '\0';
		return;
	}

	*(path+iSeeker+1) = '\0';
	while(iSeeker-1 >= idx)
	{
		*(path+iSeeker) = *(path+iSeeker-1);
		VGA_CHAR(iSeeker+POS_PATH_X, POS_PATH_Y, *(path+iSeeker));
		iSeeker--;
	}
}

