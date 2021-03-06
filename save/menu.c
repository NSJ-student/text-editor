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
int Menu_Save(MENU_ITEM now);
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
	MENU_ITEM now = MFILE;

	Menu_CursorOn(now);

	while( (asc = getch()) != ESC )
	{
		if( asc == 0 )
		{
			cmd = getch();

			if( (cmd == L_ARROW)&&(now>MFILE) )
			{
				Menu_CursorOff(now);
				now--;
				Menu_CursorOn(now);
			}
			else if( (cmd == R_ARROW)&&(now<MEXIT) )
			{
				Menu_CursorOff(now);
				now++;
				Menu_CursorOn(now);
			}
			else if(cmd == ALT_X)	return END;
		}
		else if(asc == ENTER )
		{
			switch(now)
			{
				case MFILE: 	if( END == File() )
							{
								return END;
							}break;
				case EDIT: 	break;
				case MEXIT: 	return END;
			}
			Move_Cursor(POS_MENU_X(now), POS_MENU_Y);
		}
	}

	Menu_CursorOff(now);

	return GO;
}

int File()
{
	char cmd = 0;
	char asc = 0;
	int result = END;
	void * scr;
	FILE_ITEM now = NEW;

	scr = malloc(160*25);
	if(scr == 0)
	{
		Dialog_Error("[ Screen Error ]");
		return END;
	}
	
	gettext(1, 1, 80, 25, scr);
	Draw_FileItem();
	File_CursorOn(now);

	while( (asc = getch()) != ESC )
	{
		if( asc == 0 )
		{
			cmd = getch();

			if( (cmd == U_ARROW)&&(now>NEW) )
			{
				File_CursorOff(now);
				now--;
				File_CursorOn(now);
			}
			else if( (cmd == D_ARROW)&&(now<FEXIT) )
			{
				File_CursorOff(now);
				now++;
				File_CursorOn(now);
			}
			else if(cmd == ALT_X)	return END;
		}
		else if(asc == ENTER )
		{	
			switch(now)
			{
				case NEW: 	result = Menu_New();			break;
				case LOAD: 	result = Menu_Load();			break;
				case SAVE: 	result = Menu_Save( SAVE);  	break;
				case SAVEAS:	result = Menu_Save(SAVEAS); 	break;
				case FEXIT: 	return END;
			}
			break;
		}
	}
	puttext(1, 1, 80, 25, scr);
	free(scr);
	
	if(result == GO )
	{
		if((now != SAVE)&&(now != SAVEAS))	Clr_Text();

		if( now == LOAD)	Draw_Scroll(gbTopL);
		
		Draw_FileName();
	}
	
	return GO;
}



int Menu_New()
{	
	char * path;
	
	if(save_to_new== YES) 
	{
		path = Dialog_New(set_path);
		
		if(path == 0)		// path = 0 => cancel
			return END;			// cancel
		else				// don't save & save
		{
			if(path != -1)	// path = -1 => don't save
				if( GOOD != Save_File(path))	
					return END;	// save error
		}
	}	
	
	// Save or Don't Save  => page clear
	set_path = NO;
	save_to_new = NO;
	FreeAll();
	Init();
	SetNoName();

	return GO;	
}

int Menu_Save(MENU_ITEM now)
{
	char * path;
	
	if((now == SAVEAS)||(set_path == NO) )
	{	
		path = Dialog(SAVEAS);
	}
	else
		path = Dialog(SAVE);
	
	if(path != 0) 		// path = 0 => cancel & esc
	{
		if( GOOD == Save_File(path)) 
		{
			set_path = YES;
			save_to_new = NO;
			SetFileName();
			return GO;
		}
		else
			set_path = NO;
	}
	else
		set_path = NO;

	return END;
}

int Menu_Load()
{			
	char * path;
	
	path = Dialog(LOAD);
	
	if(path != 0)
	{
		if( GOOD == Load_File(path) )
		{
			set_path = YES;
			save_to_new = NO;

			SetFileName();
			return GO;
		}
		else
		{
			set_path = NO;
			SetNoName();
			
			return GO;
		}
	}
	else
		set_path = NO;

	return END;
}
