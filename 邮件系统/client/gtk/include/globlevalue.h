#ifndef GLOBLEVALUE_H
#define GLOBLEVALUE_H

//************************全觉变量的定义*****************************************************
GtkWidget *Send_scrolled_win;              /*定义发送滚动窗口*/
GtkWidget *Send_textview;                  /*定义发送文本区*/
GtkTextBuffer *Send_buffer;                /*定义发送文本缓冲区*/

GtkWidget *addfile_entry;
GtkToolItem *font,*font_color;

gchar *text_info;////////////////文本框的内容全部放在 text 里面
enum{COLUMN = 0,NUM_COLS};



#endif
