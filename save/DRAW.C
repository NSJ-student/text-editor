#include <stdio.h>
#include <string.h>
#include "default.h"
#include "draw.h"
#include "cursor.h"

const char * Menu_item[3] = { "File", "Edit", "Exit" };
const char * File_item[5] = {"   New   ", "  Load   ", "  Save   ", " Save as ", "  Exit   "};
const char * NoName =  "     NO  NAME     ";

int Put_Str(int x, int y, const char * str)
{
	while( *str != 0 )
	{
		VGA_CHAR(x, y, *str);
		str++;
		x++;
	}

	return x;
}
void Draw_DialogNew_Saved(char * path)
{
	int pos;
	
	Clr_Dialog();
	Draw_Rect_D(POS_DIALOG_X, POS_DIALOG_Y, DIALOG_W, DIALOG_H);
	Draw_Rect(POS_DIALOG_X+1, POS_DIALOG_Y+1, DIALOG_PATH_W, DIALOG_PATH_H);
	Put_Str(POS_DIALOG_X+2, POS_DIALOG_Y, " Create ");
	Put_Str(POS_NEW_OK_X, POS_BTN_Y, "  Save  ");
	Put_Str(POS_NEW_DONT_X, POS_BTN_Y, " Don't Save ");
	Put_Str(POS_NEW_CANCEL_X, POS_BTN_Y, " Cancel ");
	pos = Put_Str(POS_PATH_X, POS_PATH_Y, " Want to save in [ ");
	pos = Put_Str(pos, POS_PATH_Y, path);
	Put_Str(pos, POS_PATH_Y, " ]?");
}

int Draw_DialogNew(char * path)
{
	int idx = 0;

	Clr_Dialog();
	Draw_Rect_D(POS_DIALOG_X, POS_DIALOG_Y, DIALOG_W, DIALOG_H);
	Draw_Rect(POS_DIALOG_X+1, POS_DIALOG_Y+1, DIALOG_PATH_W, DIALOG_PATH_H);
	Put_Str(POS_DIALOG_X+2, POS_DIALOG_Y, " Create ");
	Put_Str(POS_NEW_OK_X, POS_BTN_Y, "  Save  ");
	Put_Str(POS_NEW_DONT_X, POS_BTN_Y, " Don't Save ");
	Put_Str(POS_NEW_CANCEL_X, POS_BTN_Y, " Cancel ");
	idx = Put_Str(POS_PATH_X, POS_PATH_Y, path);
	Move_Cursor(idx, POS_PATH_Y); 

	return idx-POS_PATH_X;
}

int Draw_Dialog_Save(char * path)
{
	int idx = 0;

	Clr_Dialog();
	Draw_Rect_D(POS_DIALOG_X, POS_DIALOG_Y, DIALOG_W, DIALOG_H);
	Draw_Rect(POS_DIALOG_X+1, POS_DIALOG_Y+1, DIALOG_PATH_W, DIALOG_PATH_H);
	Put_Str(POS_DIALOG_X+2, POS_DIALOG_Y, " File ");
	Put_Str(POS_OK_X, POS_BTN_Y, "  Save  ");
	Put_Str(POS_CANCEL_X, POS_BTN_Y, " Cancel ");
	idx = Put_Str(POS_PATH_X, POS_PATH_Y, path);
	Move_Cursor(idx, POS_PATH_Y); 

	return idx-POS_PATH_X;
}
int Draw_Dialog_Load(char * path)
{
	int idx = 0;

	Clr_Dialog();
	Draw_Rect_D(POS_DIALOG_X, POS_DIALOG_Y, DIALOG_W, DIALOG_H);
	Draw_Rect(POS_DIALOG_X+1, POS_DIALOG_Y+1, DIALOG_PATH_W, DIALOG_PATH_H);
	Put_Str(POS_DIALOG_X+2, POS_DIALOG_Y, " File ");
	Put_Str(POS_OK_X, POS_BTN_Y, "  Load  ");
	Put_Str(POS_CANCEL_X, POS_BTN_Y, " Cancel ");
	idx = Put_Str(POS_PATH_X, POS_PATH_Y, path);
	Move_Cursor(idx, POS_PATH_Y); 

	return idx-POS_PATH_X;
}
void Draw_DialogFocusNew(CITEM now, int cur)
{
	if(now == C_SAVE)
	{
		Cursor_Off();		
		Draw_CursorOff(POS_NEW_DONT_X, POS_BTN_Y, DIALOG_BTN_LEN2);
		Draw_CursorOff(POS_NEW_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
		Draw_CursorOn(POS_NEW_OK_X, POS_BTN_Y, DIALOG_BTN_LEN);
	}
	if(now == C_CANCEL)
	{	
		Cursor_Off();	
		Draw_CursorOff(POS_NEW_DONT_X, POS_BTN_Y, DIALOG_BTN_LEN2);
		Draw_CursorOff(POS_NEW_OK_X, POS_BTN_Y,DIALOG_BTN_LEN);
		Draw_CursorOn(POS_NEW_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
	}	
	if(now == C_DONT)
	{	
		Cursor_Off();	
		Draw_CursorOn(POS_NEW_DONT_X, POS_BTN_Y, DIALOG_BTN_LEN2);
		Draw_CursorOff(POS_NEW_OK_X, POS_BTN_Y,DIALOG_BTN_LEN);
		Draw_CursorOff(POS_NEW_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
	}	
	if(now == C_WINDOW)
	{		
		Cursor_On();
		Draw_CursorOff(POS_NEW_DONT_X, POS_BTN_Y, DIALOG_BTN_LEN2);
		Draw_CursorOff(POS_NEW_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
		Draw_CursorOff(POS_NEW_OK_X, POS_BTN_Y,DIALOG_BTN_LEN);
		Move_Cursor(POS_PATH_X+cur, POS_PATH_Y);
	}
}

void Draw_DialogFocus(SITEM now, int cur)
{
	if(now == S_OK)
	{
		Cursor_Off();		
		Draw_CursorOff(POS_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
		Draw_CursorOn(POS_OK_X, POS_BTN_Y, DIALOG_BTN_LEN);
	}
	if(now == S_CANCEL)
	{	
		Cursor_Off();	
		Draw_CursorOff(POS_OK_X, POS_BTN_Y,DIALOG_BTN_LEN);
		Draw_CursorOn(POS_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
	}	
	if(now == S_WINDOW)
	{		
		Cursor_On();
		Draw_CursorOff(POS_CANCEL_X, POS_BTN_Y, DIALOG_BTN_LEN);
		Draw_CursorOff(POS_OK_X, POS_BTN_Y,DIALOG_BTN_LEN);
		Move_Cursor(POS_PATH_X+cur, POS_PATH_Y);
	}
}

void Draw_ErrorBox(char * msg)
{
	int len;
	
	Draw_Rect_D(POS_ERR_X, POS_ERR_Y, ERR_W, ERR_H);
	
	Clr_Line(POS_ERR_X+1, POS_ERR_Y+1, ERR_MSG_W);
	Clr_Line(POS_ERR_X+1, POS_ERR_Y+2, ERR_MSG_W);
	Clr_Line(POS_ERR_X+1, POS_ERR_Y+3, ERR_MSG_W);
	Clr_Line(POS_ERR_X+1, POS_ERR_Y+4, ERR_MSG_W);

	len = strlen(msg);
	Put_Str(POS_ERR_X+1+(int)((ERR_W-len)/2), POS_ERR_Y+2, msg);
	
	Put_Str(POS_ERR_X+2, POS_ERR_Y, " Error ");
	Put_Str(POS_ERR_X+((ERR_W-6)/2), 			POS_ERR_Y+ERR_H-2, "  OK  ");
	Draw_CursorOn(POS_ERR_X+((ERR_W-6)/2), 	POS_ERR_Y+ERR_H-2, 6);
}

void Draw_FileName()
{	
	int x, y;
	
	x = POS_NAME_X + 1;
	y = strlen(FileName);
	while( x < POS_NAME_X + 1 +(NAME_WIDTH - y)/2)
	{
		VGA_CHAR(x, POS_NAME_Y+1, ' ');
		x++;
	}
	x = Put_Str(x, POS_NAME_Y+1, FileName);
	while( x < POS_NAME_X + NAME_WIDTH)
	{
		VGA_CHAR(x, POS_NAME_Y+1, ' ');
		x++;
	}
}

void Draw_MainWindow(void)
{
	Draw_Rect(0, 0, WINDOW_W, WINDOW_H);	// main frame
	Draw_Rect(1, 3, WINDOW_W-2, TEXT_H+3);	// text frame
	Draw_Rect(1, 21, WINDOW_W-2, 3);		// bottom box frame
	Draw_HLine(2);
	Draw_HLine(20);
	Draw_Rect_D(POS_NAME_X, POS_NAME_Y, NAME_WIDTH+2, 3);	// name box frame
	Clr_Line(31, 3, 18);

	Put_Str(POS_FILE, 	POS_MENU_Y, Menu_item[MFILE]);
	Put_Str(POS_EDIT, 	POS_MENU_Y, Menu_item[EDIT]);
	Put_Str(POS_EXIT, 	POS_MENU_Y, Menu_item[MEXIT]);

	Put_Str(POS_NAME_X+1, POS_NAME_Y+1,NoName);
	
	Put_Str(7, 22, "F10 - Menu");
	Put_Str(25, 22, "Alt + x - Exit");
	Put_Str(47, 22, "F1 - Help");
}

void Draw_FileItem()
{
	Draw_Rect_D(POS_FILE, POS_MENU_Y+1, 11, 7);
	
	Put_Str(POS_FILE_X, POS_FILE_Y(NEW), 	File_item[NEW]);
	Put_Str(POS_FILE_X, POS_FILE_Y(LOAD), 	File_item[LOAD]);
	Put_Str(POS_FILE_X, POS_FILE_Y(SAVE), 	File_item[SAVE]);
	Put_Str(POS_FILE_X, POS_FILE_Y(SAVEAS), 	File_item[SAVEAS]);
	Put_Str(POS_FILE_X, POS_FILE_Y(FEXIT), 	File_item[FEXIT]);
}

void Draw_HLine(int y)
{
	int x = 0;

	VGA_CHAR(x, y, AA_LINE);
	for(x++; x<79; x++)
	{
		VGA_CHAR(x, y, H_LINE);
	}
	VGA_CHAR(x, y, EO_LINE);
}

void Clr_Text()
{
	int x, y;
	
	for(y=0; y<TEXT_H; y++)
	{
		for(x = 0; x<TEXT_W; x++)
		{
			VGA_CHAR(POS_TEXT_X+x, POS_TEXT_Y+y, ' ');
		}
	}
}

void Clr_Dialog()
{
	int i, j;
	for(j = 0; j<DIALOG_H-2; j++)
	{
		for(i = 0; i<DIALOG_W-2; i++)
		{
			VGA_CHAR(POS_PATH_X-1+i, POS_PATH_Y+j, ' ');
		}
	}
}
void Clr_Line(int x, int y, int len)
{
	int i;
	
	for(i = x; i<x+len; i++)
	{
		VGA_CHAR(i, y, ' ');
	}
}
void Draw_CursorOff(int x, int y, int len)
{
	for(; len>0; len--, x++)
	{
		ORG_CHAR(x, y);
	}
}

void Draw_CursorOn(int x, int y, int len)
{
	for(; len>0; len--, x++)
	{
		INV_CHAR(x, y);
	}
}

void Menu_CursorOn(MENU_ITEM now)
{
	Draw_CursorOn(POS_MENU_X(now), POS_MENU_Y, 4);
	Move_Cursor(POS_MENU_X(now), POS_MENU_Y);
}
void Menu_CursorOff(MENU_ITEM now)
{
	Draw_CursorOff(POS_MENU_X(now), POS_MENU_Y, 4);
	Move_Cursor(POS_MENU_X(now), POS_MENU_Y);
}
void File_CursorOn(FILE_ITEM now)
{
	Draw_CursorOn(POS_FILE_X, POS_FILE_Y(now), 9);
	Move_Cursor(POS_FILE_X, POS_FILE_Y(now));
}
void File_CursorOff(FILE_ITEM now)
{
	Draw_CursorOff(POS_FILE_X, POS_FILE_Y(now), 9);
	Move_Cursor(POS_FILE_X, POS_FILE_Y(now));
}
void Draw_Rect(int xi, int yi, int w, int h)
{
	int x = xi;
	int y = yi;

	VGA_CHAR(x, y, LU_LINE);
	
	for(x++; x<w-1+xi; x++)
	{
		VGA_CHAR(x, y, H_LINE);
	}

	VGA_CHAR(x, y, RU_LINE);

	for(y++; y<h-1+yi; y++)
	{
		VGA_CHAR(x, y, V_LINE);
	}

	VGA_CHAR(x, y, RD_LINE);

	for(x--; x>xi; x--)
	{
		VGA_CHAR(x, y, H_LINE);
	}

	VGA_CHAR(x, y, LD_LINE);

	for(y--; y>yi; y--) 
	{
		VGA_CHAR(x, y, V_LINE);
	}



}

void Draw_Rect_D(int xi, int yi, int w, int h)
{
	int x = xi;
	int y = yi;

	VGA_CHAR(x, y, LU_LINE_D);
	
	for(x++; x<w-1+xi; x++)
	{
		VGA_CHAR(x, y, H_LINE_D);
	}

	VGA_CHAR(x, y, RU_LINE_D);

	for(y++; y<h-1+yi; y++)
	{
		VGA_CHAR(x, y, V_LINE_D);
	}

	VGA_CHAR(x, y, RD_LINE_D);

	for(x--; x>xi; x--)
	{
		VGA_CHAR(x, y, H_LINE_D);
	}

	VGA_CHAR(x, y, LD_LINE_D);

	for(y--; y>yi; y--) 
	{
		VGA_CHAR(x, y, V_LINE_D);
	}
}

void Draw_Int(int y, int a)
{
	VGA_CHAR(0, 15+y, '0'+(int)(a/100));
	VGA_CHAR(1, 15+y, '0'+(int)((a%100)/10));
	VGA_CHAR(2, 15+y, '0'+(int)(a%10));
}
