#include <gtk/gtk.h>
#include "../include/EditMailForm_Func.h"
#include"../include/globlevalue.h"
#include"../../function/include/DataStruct.h"
#include"../../function/include/mysocket.h"
#include<string.h>


/////**********************************************************************
/////	函数名称	：	GtkTreeModel *create_and_fill_model(void)
/////	
/////	函数功能	：	创  建  树项并添加树项成员   
/////									
/////   返 回 值	：	GTK_TREE_MODEL(treestore)
/////**********************************************************************
 GtkTreeModel *create_and_fill_model1(void)
{
	extern char g_userId[25];
	char temp[50];
	char acNameAndID[76];
	memset(acNameAndID,'\0', 76);
	int index = 0;
	int num = 0;
	int length = 0;
	int i ;
	int j = 0;
	int k = 0;
	memset(temp, '\0', 50);
    char* buffer[2];
    int ii=0;
    for(;ii<2;ii++)
    {
        buffer[ii]=(gchar*)malloc(sizeof(char) * 50);
    }
	char *pacContact[50];
/* 字符串数组，用于加到l i s t中，4行2列*/
////////////////////write here //////////////////////////////////////

	GetContactInfo(g_userId, pacContact, &num);
	GtkTreeStore *treestore;
	GtkTreeIter toplevel, child;

	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, COLUMN, "联系人",-1);

	GetContactInfo(g_userId, pacContact, &num);
	for (; index < num ; ++index )
	{	
		k = 0;
		length = strlen(pacContact[index]);
		memset(buffer[0], '\0', 50);
		memset(buffer[1], '\0', 50);
		//g_print("该条记录长度%d %s\n",length, pacContact[index]);	
		for (i = 0; i < length; ++i)
		{	
			memset(acNameAndID,'\0', 76);
			if (pacContact[index][i] == '|')
			{
				strcpy(buffer[k], temp);
				++k;
				if (k == 2)
				{
					sprintf(acNameAndID, "%s(%s)", buffer[0], buffer[1]);
					j = 0;
					break;
				}
				memset(temp, '\0', 50);
				j = 0;
			}
			else
			{
				temp[j] = pacContact[index][i];
				++j;
			}
		}
		gtk_tree_store_append(treestore, &child, &toplevel);
		gtk_tree_store_set(treestore, &child, COLUMN, acNameAndID,-1);
	}	
	return GTK_TREE_MODEL(treestore);
}
 
/////**********************************************************************
/////	函数名称	：	GtkWidget *create_view_and_model (void)
/////	
/////	函数功能	：	创  建  树视图     Treeview
/////									
/////   返 回 值	：	view
/////**********************************************************************
GtkWidget *create_view_and_model1 (void)
{
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkWidget *view;
	GtkTreeModel *model;
	view = gtk_tree_view_new();
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);
	model = create_and_fill_model1();//
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model);
	return view;
}
