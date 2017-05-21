#ifndef __DIALOG_H__
#define __DIALOG_H__

void SetNoName();
void SetFileName();

char * Dialog(MENU_ITEM aNow);
void Dialog_Error(char * msg);
char * Dialog_New(const BOOL set_path);
void Draw_Array_Move_Left(char path[], int idx);
void Draw_Array_Move_Right(char path [ ], int idx);

#endif /* __DIALOG_H__ */