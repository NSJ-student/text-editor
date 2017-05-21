#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include "default.h"
#include "draw.h"
#include "file.h"

static char gbPath[DIALOG_PATH_W-2];

void Get_File_Path(char * caDialog_Path)
{
	unsigned int uiCnt = 0;
	
	for(uiCnt = 0; gbPath[uiCnt] != 0; uiCnt++)
	{
		caDialog_Path[uiCnt] = gbPath[uiCnt];
	}
	caDialog_Path[uiCnt] = 0;

	return;
}
void Set_File_Path(char * caDialog_Path)
{
	unsigned int uiCnt = 0;
	
	for(uiCnt = 0; caDialog_Path[uiCnt] != 0; uiCnt++)
	{
		gbPath[uiCnt] = caDialog_Path[uiCnt];
	}
	gbPath[uiCnt] = 0;

	return;
}

void Test_Ext(char * path)
{
	unsigned int uiCnt;
	unsigned int uiExt;
	char cExt[] = ".txt";
	
	for(uiCnt = 1; path[uiCnt] != 0; uiCnt++)
	{
		if(path[uiCnt] == '.')
		{
			break;
		}
		if( uiCnt == 13)
		{
			break;
		}
	}
	
	for(uiExt = 0; cExt[uiExt] != 0; uiExt++)
	{
		path[uiCnt+uiExt] = cExt[uiExt];
	}
	path[uiCnt+uiExt] = 0;

	return;
}

int Save_File(char * path)
{
	int fd;
	stLine * TmpL = gbHead;
	stChar * TmpC;
	int x = 0;
	int y = 0;

	Test_Ext(path);
	fd = creat(path, S_IREAD|S_IWRITE);
	if(fd == -1)
	{
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

	Set_File_Path(path);
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
				return ERROR;
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
					return ERROR;
				}
			}
		}
		else if((ch >= ' ')&&(ch <= '~'))	
		{
			if(FAIL == Insert(ch))
			{
				FreeAll();
				Init();
				Dialog_Error("[ Insert Fail ]");
				return ERROR;
			}
		}
	}

	gbCurL = gbHead;
	gbCurC = 0;
	gbTopL = gbHead;
	gbTopCharNum= 0;

	Set_File_Path(path);
	close(fd);
	return GOOD;
}
