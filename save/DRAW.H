#ifndef __DRAW_H__
#define __DRAW_H__

// 그리기 기본 함수
void Draw_HLine(int y);
void Draw_Rect(int xi, int yi, int w, int h);
void Draw_Rect_D(int xi, int yi, int w, int h);

void Draw_CursorOn(int x, int y, int len);
void Draw_CursorOff(int x, int y, int len);

void Clr_Text();
void Clr_Dialog();
void Clr_Line(int x, int y, int len);

// Dialog 관련
void Draw_ErrorBox(char * msg);
int Draw_Dialog_Save(char * path);
int Draw_Dialog_Load(char * path);
int Draw_DialogNew(char * path);
void Draw_DialogNew_Saved(char * path);
void Draw_DialogFocus(SITEM now, int cur);
void Draw_DialogFocusNew(CITEM now, int cur);

// 메뉴 관련
void Menu_CursorOn(MENU_ITEM now);
void Menu_CursorOff(MENU_ITEM now);
void File_CursorOn(FILE_ITEM now);
void File_CursorOff(FILE_ITEM now);

// 창 그리기
void Draw_MainWindow();
void Draw_FileItem();
void Draw_FileName();

// 해당 좌표에 문자열 출력
int Put_Str(int x, int y, const char * str);

void Draw_Int(int y, int a);

#endif /* __DRAW_H__ */
