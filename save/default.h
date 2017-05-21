#ifndef __DEFAULT_H__
#define __DEFAULT_H__


#define DEFAULT_PATH	".\\save\\"


#pragma pack(1)

typedef struct _char_tag {
	char character;
	int char_number;
	struct _char_tag * prev;
	struct _char_tag * next;
} stChar;

typedef struct _line_tag {
	int line_number;
	int line_count;
	struct _line_tag * down;
	struct _line_tag * up;
	struct _char_tag * char_point;
} stLine;

#pragma pack(4)

extern stLine * gbHead;			// 문자가 저장된 공간 맨 앞을 가리키는 포인터
extern stLine * gbCurL;			// 현재 커서가 위치한 공간을 가리키는 포인터
extern stLine * gbTopL;
extern stChar * gbCurC;			// 현재 커서가 위치한 공간을 가리키는 포인터
extern char FileName[18];

#define TAB_CNT	4

#define GO	0
#define END	-1

#define FAIL		-1
#define GOOD	0

#define UP		1
#define DOWN	0

typedef enum { NEW_IN, INPUT, REMOVE, CURSOR, R_MOVE, L_MOVE } 	ACT; 
typedef enum { MFILE, EDIT, MEXIT } 						MENU_ITEM;
typedef enum { NEW, LOAD, SAVE, SAVEAS, FEXIT }			FILE_ITEM;
typedef enum { S_WINDOW, S_OK, S_CANCEL } 				SITEM;
typedef enum { C_SAVE, C_DONT, C_CANCEL, C_WINDOW } 	CITEM;
typedef enum { YES, NO } 								BOOL;

// ------------------------------------ DRAW 관련

// Extended ASCII - LINE 
#define H_LINE			196
#define V_LINE			179
#define LU_LINE			218
#define RU_LINE			191
#define LD_LINE			192
#define RD_LINE			217
#define EO_LINE			180
#define AA_LINE			195

// Extended ASCII - DOUBLE LINE 
#define H_LINE_D		205
#define V_LINE_D			186
#define LU_LINE_D		201
#define RU_LINE_D		187
#define LD_LINE_D		200
#define RD_LINE_D		188
#define EO_LINE_D		185
#define AA_LINE_D		204

// Name box component 

#define POS_NAME_X		30
#define POS_NAME_Y 		2
#define NAME_WIDTH		18

// Error box
#define POS_ERR_X		29
#define POS_ERR_Y		7
#define ERR_MSG_W		18
#define ERR_W			20
#define ERR_H			6

// Menu component x-axis
#define POS_MENU_X(x)	(3+12*(x))
#define POS_MENU_Y		1	// menu bar y-axis

#define POS_FILE			3+12*0
#define POS_EDIT		3+12*1
#define POS_EXIT		3+12*2

#define POS_FILE_X		4
#define POS_FILE_Y(x)	(3+x)

#define POS_NEW		3 + 0
#define POS_LOAD		3 + 1
#define POS_SAVE		3 + 2
#define POS_SAVEAS		3 + 3
#define POS_FEXIT		3 + 4

// init cursor position
#define POS_TEXT_X		2	// text line initial x-axis
#define POS_TEXT_Y		5	// text line initial y-axis	

// Save&Load Dialog 관련
#define DIALOG_W			66			// dialog width
#define DIALOG_H			6			// dialog height
#define DIALOG_BTN_LEN		8			// Save&Load&Cancel button length
#define DIALOG_BTN_LEN2	12			// Don't Save button length
#define DIALOG_PATH_W		DIALOG_W-2	// path window width
#define DIALOG_PATH_H		3			// path window height

#define POS_DIALOG_X			7		// dialog left-up  x-axis
#define POS_DIALOG_Y			7		// dialog left-up y-axis
#define POS_PATH_X				9		// path window left-up y-axis
#define POS_PATH_Y				9		// path window left-up y-axis


// Save&Load 관련 다이얼로그 좌표
#define DIALOG_BLANK_W			((int)((DIALOG_W-2-DIALOG_BTN_LEN*2)/3))
																	// Save&Load x-axis
#define POS_OK_X				(POS_DIALOG_X -1 + DIALOG_BLANK_W)		
																	// Cancel x-axis
#define POS_CANCEL_X			(POS_OK_X +	DIALOG_BTN_LEN + DIALOG_BLANK_W)


#define POS_BTN_Y				(POS_DIALOG_Y+DIALOG_H-2)			// button y-axis	
// New 관련 다이얼로그 좌표
#define DIALOG_BLANK_W2		((int)((DIALOG_W-2-DIALOG_BTN_LEN*2-DIALOG_BTN_LEN2)/4))
																	// Save x-axis
#define POS_NEW_OK_X			(POS_DIALOG_X -1 + DIALOG_BLANK_W2)		
																	// Don't Save x-axis
#define POS_NEW_DONT_X		(POS_NEW_OK_X +	DIALOG_BTN_LEN + DIALOG_BLANK_W2)		
																	// Cancel x-axis
#define POS_NEW_CANCEL_X		(POS_NEW_DONT_X + DIALOG_BTN_LEN2 + DIALOG_BLANK_W2)

// 전체 윈도우 크기
#define WINDOW_W		80
#define WINDOW_H		25

#define TEXT_W			76
#define TEXT_H			14

#define MAX_TEXT		75

/* ASCII */
#define ESC				27
#define ENTER			13
#define BACK_SPACE		8
#define SPACE			32
#define TAB				9

/* KEY VALYE */
#define L_ARROW			75
#define R_ARROW			77
#define U_ARROW			72
#define D_ARROW			80
#define F1				59
#define F10				68
#define ALT_X			45
#define DELETE			83
#define HOME_KEY		71
#define END_KEY			79
#define PAGE_UP			73
#define PAGE_DOWN		81


//-------------------------		매크로 함수		----------------------------

#define VGA_CHAR(x, y, ch)	(*((char far *)(0xb8000000 + (x)*2 + (y)*0xa0))) = (ch); \
							(*((char far *)(0xb8000000 + ((x)*2+1) + (y)*0xa0))) = 0x07
#define INV_CHAR(x, y)		(*((char far *)(0xb8000000 + ((x)*2+1) + (y)*0xa0))) = 0x70
#define ORG_CHAR(x, y)		(*((char far *)(0xb8000000 + ((x)*2+1) + (y)*0xa0))) = 0x07

//#define VGA_ATTR(x, y, at)	((*((char far *)(0xb8000000 + ((x)*2+1) + (y)*0xa0))) = (at))
#define DELETE_ULINE(CurLine, FreeLine)		\
		if(CurLine != 0) (CurLine)->up = ((FreeLine)->up); 	\
		if((FreeLine)->up != 0) ((FreeLine)->up)->down = CurLine;\
		free(FreeLine)
#define DELETE_DLINE(CurLine, FreeLine)		\
		if(CurLine != 0) (CurLine)->down = ((FreeLine)->down);\
		if((FreeLine)->down != 0) ((FreeLine)->down)->up = CurLine;	\
		free(FreeLine)
#define DELETE_NCHAR(CurChar, FreeChar)		\
		if(CurChar != 0) 	(CurChar)->next = ((FreeChar)->next); 	\
		if((FreeChar)->next != 0) ((FreeChar)->next)->prev = CurChar;\
		free(FreeChar)
#define DELETE_PCHAR(CurChar, FreeChar)		\
		if(CurChar != 0) 	(CurChar)->prev = ((FreeChar)->prev);\
		if((FreeChar)->prev != 0) ((FreeChar)->prev)->next = CurChar;	\
		free(FreeChar)
										
#define LINE_ALIGN(stTmpL)		while(stTmpL != 0)	\
								{	\
									stTmpL->line_number = (stTmpL->up)->line_number + 1;	\
									stTmpL = stTmpL->down;	\
								}

#define CHAR_ALIGN(stTmpC)		while(stTmpC != 0)	\
								{	\
									stTmpC->char_number = (stTmpC->prev)->char_number + 1;	\
									stTmpC = stTmpC->next;	\
								}

#endif	/* __DEFAULT_H__ */
