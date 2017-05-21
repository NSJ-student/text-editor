#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "default.h"
#include "draw.h"
#include "draw_c.h"
#include "cursor.h"
#include "dialog.h"
#include "menu.h"
#include "file.h"

static BOOL set_path = NO;
static BOOL save_to_new = NO;

int Menu_New();
int Menu_Save(MENU_ITEM mNow);
int Menu_Load();
int File();

void TextChanged()
{
	save_to_new = YES;
}

int Menu()
{
	char cmd = 0;
	char asc = 0;
	MENU_ITEM mNow = MFILE;

	Menu_CursorOn(mNow);
	
	Cursor_Off();
	while( (asc = getch()) != ESC )
	{
		
		if( asc == 0 )
		{
			cmd = getch();

			if( (cmd == L_ARROW)&&(mNow>MFILE) )
			{
				Menu_CursorOff(mNow);
				mNow--;
				Menu_CursorOn(mNow);
			}
			else if( (cmd == R_ARROW)&&(mNow<MEXIT) )
			{
				Menu_CursorOff(mNow);
				mNow++;
				Menu_CursorOn(mNow);
			}
			else if(cmd == ALT_X)	return END;
		}
		else if(asc == ENTER )
		{
			switch(mNow)
			{
				case MFILE: 	if( END == File() )
							{
								return END;
							}break;
				case EDIT: 	break;
				case MEXIT: 	return END;
			}
			Move_Cursor(POS_MENU_X(mNow), POS_MENU_Y);
		}
	}

	Menu_CursorOff(mNow);
	Cursor_On();
	return GO;
}

int File()
{
	char cmd = 0;
	char asc = 0;
	int result = END;
	FILE_ITEM fNow = NEW;
	void * scr;
	scr = malloc(160*25);
	gettext(1, 1, 80, 25, scr);
	
	Draw_FileItem();
	File_CursorOn(fNow);

	while( (asc = getch()) != ESC )
	{
		if( asc == 0 )
		{
			cmd = getch();

			if( (cmd == U_ARROW)&&(fNow>NEW) )
			{
				File_CursorOff(fNow);
				fNow--;
				File_CursorOn(fNow);
			}
			else if( (cmd == D_ARROW)&&(fNow<FEXIT) )
			{
				File_CursorOff(fNow);
				fNow++;
				File_CursorOn(fNow);
			}
			else if(cmd == ALT_X)	
			{
				free(scr);
				return END;
			}
		}
		else if(asc == ENTER )
		{	
			switch(fNow)
			{
				case NEW: 	result = Menu_New();			break;
				case LOAD: 	result = Menu_Load();			break;
				case SAVE: 	result = Menu_Save( SAVE);  	break;
				case SAVEAS:	result = Menu_Save(SAVEAS); 	break;
				case FEXIT: 	free(scr);
							return END;
			}
			break;
		}
	}
	puttext(1, 1, 80, 25, scr);
	
	if(result == GO )
	{
		if((fNow != SAVE)&&(fNow != SAVEAS))	Clr_Text();

		if( fNow == LOAD)	Draw_Scroll(gbTopL);
		
		Draw_FileName();
	}
	
	free(scr);
	return GO;
}



int Menu_New()
{	
	char * path;
	void * scr;
	scr = malloc(160*25);

	while(1)
	{
		if(save_to_new== YES) 
		{
			gettext(1, 1, 80, 25, scr);
			path = Dialog_New(set_path);
			puttext(1, 1, 80, 25, scr);
			
			if(path == 0)		// path = 0 => cancel
			{
				free(scr);
				return END;			// cancel
			}
			else				// don't save & save
			{
				if(path != -1)	// path = -1 => don't save
				{
					if( GOOD != Save_File(path))
					{
						gettext(1, 1, 80, 25, scr);
						Dialog_Error("[ Create Fail ]");
						puttext(1, 1, 80, 25, scr);
						continue;
					}
				}
			}
		}	
		break;
	}
	
	// Save or Don't Save  => page clear
	set_path = NO;
	save_to_new = NO;
	FreeAll();
	Init();
	SetNoName();

	free(scr);
	return GO;	
}

int Menu_Save(MENU_ITEM mNow)
{
	char * path;
	void * scr;
	scr = malloc(160*25);

	while(1)
	{
		if((mNow == SAVEAS)||(set_path == NO) )
		{	
			gettext(1, 1, 80, 25, scr);
			path = Dialog(SAVEAS);
			puttext(1, 1, 80, 25, scr);
		}
		else
		{
			path = Dialog(SAVE);
		}
		
		if(path != 0) 		// path = 0 => cancel & esc
		{
			if( GOOD == Save_File(path)) 
			{
				set_path = YES;
				save_to_new = NO;
				SetFileName();
				free(scr);
				return GO;
			}
			else
			{
				gettext(1, 1, 80, 25, scr);
				Dialog_Error("[ Create Fail ]");
				puttext(1, 1, 80, 25, scr);
				set_path = NO;
				continue;
			}
		}
		else
		{
			set_path = NO;
			break;
		}
	}

	free(scr);
	return END;
}

int Menu_Load()
{			
	char * path;
	int result;
	void * scr;
	scr = malloc(160*25);

	while(1)
	{
		gettext(1, 1, 80, 25, scr);
		path = Dialog(LOAD);
		puttext(1, 1, 80, 25, scr);
		
		if(path != 0)
		{
			result = Load_File(path);
			
			if( GOOD == result )
			{
				set_path = YES;
				save_to_new = NO;

				SetFileName();
				free(scr);
				return GO;
			}
			else
			{ 
				gettext(1, 1, 80, 25, scr);
				Dialog_Error("[ Open Fail ]");
				puttext(1, 1, 80, 25, scr);
				set_path = NO;
				continue;
			}
		}
		else
		{
			set_path = NO;
			break;
		}
	}

	free(scr);
	return END;
}
