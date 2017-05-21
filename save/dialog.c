#include "default.h"
#include "dialog.h"

static char path[DIALOG_PATH_W-2]= DEFAULT_PATH;
char FileName[18];

void SetFileName()
{
	int head = 0;
	int i;

	for(i = 7; path[i] != 0; i++, head++)
	{
		FileName[head] = path[i];
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
	CITEM now = C_SAVE;
	int path_length = 0;


	if(set_path == NO)
	{
		now = C_WINDOW;
		idx = Draw_DialogNew(path);
		path_length = idx;
	}
	else
	{
		Draw_DialogNew_Saved(path);
		Draw_DialogFocusNew(now, 0);
	}

	asc = getch();
	while( asc != ESC)
	{
		if( asc == 0 )
		{
			cmd = getch();

			switch(cmd)
			{
				case R_ARROW: 	if(now != C_WINDOW) 
								{
									if(now == C_SAVE) 		now = C_DONT;
									else if(now == C_DONT) 	now = C_CANCEL;
								}
								else if( path[idx] != 0) idx++;
								break;
				case L_ARROW: 	if(now != C_WINDOW) 
								{
									if(now == C_DONT) 		now = C_SAVE;
									else if(now == C_CANCEL) 	now = C_DONT;
								}
								else if(idx> 0)			idx--;
								break;
				case D_ARROW:	if(now == C_WINDOW) 		now = C_SAVE; break;
				case U_ARROW: 	if(set_path == NO) 			now = C_WINDOW; break;
				default: 			continue;
			}
			Draw_DialogFocusNew(now, idx);
		}
		else if(asc == ENTER)
		{
			if((now == C_SAVE)||(now == C_WINDOW))
			{
				Cursor_On();
				return path;
			}
			if(now == C_DONT)
			{
				Cursor_On();
				return -1;
			}
			if(now == C_CANCEL)
			{
				Cursor_On();
				return 0;
			}
		}
		else if(now == C_WINDOW)
		{
			if(asc == BACK_SPACE)
			{
				if(idx > 0)
				{
					path_length--;
					idx--;
					Draw_Array_Move_Left(path, idx);
					Move_Cursor(idx+POS_PATH_X, POS_PATH_Y);
				}
			}
			else
			{
				if(path_length < DIALOG_PATH_W-3)
				{
					Draw_Array_Move_Right(path, idx);
					path[idx] = asc;
					VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, asc);
					Move_Cursor(idx+POS_PATH_X+1, POS_PATH_Y);
					
					idx++;
					path_length++;
				}
			}
		}
		asc = getch();
	}

	Cursor_On();
	return 0;
}


char * Dialog(MENU_ITEM aNow)
{
	int idx = 0;
	char asc = 0;
	char cmd = 0;
	SITEM now = S_WINDOW;
	int path_length = 0;

	if(aNow == SAVE)
		return path;

	if(aNow == SAVEAS)
		idx = Draw_Dialog_Save(path);
	if(aNow == LOAD)
		idx = Draw_Dialog_Load(path);

	path_length = idx;
	
	asc = getch();
	while( asc != ESC)
	{
		if( asc == 0 )
		{
			cmd = getch();

			switch(cmd)
			{
				case R_ARROW: 	if(now != S_WINDOW) 	now = S_CANCEL; 
								else if( path[idx] != 0)	idx++;
								break;
				case L_ARROW: 	if(now != S_WINDOW) 	now = S_OK; 
								else if(idx> 7)			idx--;
								break;
				case D_ARROW: 	if(now == S_WINDOW) now = S_OK; 	break;
				case U_ARROW: 	now = S_WINDOW; 				break;
				default: 			continue;
			}
			Draw_DialogFocus(now, idx);
		}
		else if(asc == ENTER)
		{
			if((now == S_OK)||(now == S_WINDOW))
			{
				Cursor_On();
				return path;
			}
			if(now == S_CANCEL)
			{
				Cursor_On();
				return 0;
			}
		}
		else if(now == S_WINDOW)
		{
			if(asc == BACK_SPACE)
			{
				if(idx > 7)
				{
					path_length--;
					idx--;
					Draw_Array_Move_Left(path, idx);
					Move_Cursor(idx+POS_PATH_X, POS_PATH_Y);
				}
			}
			else
			{
				if(path_length < DIALOG_PATH_W-3)
				{
					Draw_Array_Move_Right(path, idx);
					path[idx] = asc;
					VGA_CHAR(idx+POS_PATH_X, POS_PATH_Y, asc);
					Move_Cursor(idx+POS_PATH_X+1, POS_PATH_Y);
					
					idx++;
					path_length++;
				}
			}
		}
		asc = getch();
	}

	Cursor_On();
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

