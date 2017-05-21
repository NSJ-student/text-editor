#ifndef __DRAW_TEXT_H__
#define __DRAW_TEXT_H__


// 텍스트창 문자&커서
void Draw_Text_Input(stLine * Line, stChar * Char);
void Draw_Text_Enter();
void Draw_Text_Remove();

void Draw_Text_RestLineClear(stLine * Line, stChar * Char, int offset);
void Draw_Scroll(stLine * pTop);

void Draw_Text_PgUp();
void Draw_Text_PgDn();
void Draw_Text_End();
void Draw_Text_LMove();
void Draw_Text_RMove();
void Draw_Text_UMove();
void Draw_Text_DMove();
void Draw_Text_Cursor();

void Draw_Text_All();

#endif /* __DRAW_TEXT_H__ */
