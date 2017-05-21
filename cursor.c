#include "default.h"
#include "cursor.h"
#include <dos.h>

void Move_Cursor(int x, int y)
{
	union REGS regs;
	regs.h.ah = 2;
	regs.h.dh = y;
	regs.h.dl = x;
	regs.h.bh = 0;
	int86(0x10, &regs, &regs);
}

void Cursor_Off()
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0x20;
	regs.h.cl = 0;
	int86(0x10, &regs, &regs);
}

void Cursor_On()
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0x0B;
	regs.h.cl = 0x0C;
	int86(0x10, &regs, &regs);
}