#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include "default.h"
#include "draw.h"
#include "file.h"

int Save_File(char * path)
{
	int fd;
	stLine * TmpL = gbHead;
	stChar * TmpC;
	int x = 0;
	int y = 0;

	fd = creat(path, S_IREAD|S_IWRITE);
	if(fd == -1)
	{
		Dialog_Error("[ Create Fail ]");
		return FAIL;
	}
	
	while( 0 != TmpL )
	{
		TmpC = TmpL->char_point;
		while(TmpC != 0)
		{
			write(fd, &(TmpC->character), 1);
			TmpC = TmpC->next;
			x++;
		}
		y++;
		TmpL = TmpL->down;
	}

	close(fd);
	return GOOD;
}

int Load_File(char * path)
{
	int fd;
	char ch;
	volatile int ret = 1;
	stLine * tmpL;
	
	fd = open(path, O_RDWR|O_TEXT);
	if(fd == -1)
	{
		Dialog_Error("[ Open Fail ]");
		return FAIL;
	}
	FreeAll();
	Init();

	lseek(fd, 0, SEEK_SET);

	while( ret != 0)
	{
		ret = read(fd, &ch, 1);

		if(ret == 0) break;
		
		if(ch == '\n' )		
		{
			if(FAIL == Insert_Enter())
			{
				FreeAll();
				Init();
				Dialog_Error("[ Enter Fail ]");
				return FAIL;
			}
		}
		else if(ch == TAB)	
		{
			for(ret = 0; ret<TAB_CNT; ret++)
			{
				if(FAIL == Insert(' '))
				{
					FreeAll();
					Init();
					Dialog_Error("[ Insert Fail ]");
					return FAIL;
				}
			}
		}
		else if(ch >= ' ')	
		{
			if(FAIL == Insert(ch))
			{
				FreeAll();
				Init();
				Dialog_Error("[ Insert Fail ]");
				return FAIL;
			}
		}
	}

	tmpL = gbCurL;
	for(ret = 0; (ret<TEXT_H-1)&&(tmpL->up != 0); ret++)
	{
		tmpL = tmpL->up;
	}
	gbTopL = tmpL;

	close(fd);
	return GOOD;
}
