#define GTK_ENABLE_BROKEN
#include<gtk/gtk.h>
#include"../include/MainForm_PanelFunc.h"



//hpaned内容
/*为树构件添加成员*/
GtkTreeModel *create_and_fill_model(void)
{
	enum{COLUMN = 0,NUM_COLS};
	GtkTreeStore *treestore;
	GtkTreeIter toplevel, child;
	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel,COLUMN, "邮件", -1);
	gtk_tree_store_append(treestore, &child, &toplevel);
	gtk_tree_store_set(treestore, &child, COLUMN, "收件箱", -1);
	gtk_tree_store_append(treestore, &child, &toplevel);
	gtk_tree_store_set(treestore, &child, COLUMN, "发件箱",-1);
	gtk_tree_store_append(treestore, &child, &toplevel);
	gtk_tree_store_set(treestore, &child, COLUMN, "草稿",-1);



	return GTK_TREE_MODEL(treestore);
}
//创建Treeview
GtkWidget *create_view_and_model (void)
{
	enum{COLUMN = 0,NUM_COLS};
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkWidget *view;
	GtkTreeModel *model;
	view = gtk_tree_view_new();
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "菊花邮箱");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);
	model = create_and_fill_model();//
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model);
	return view;
}



GtkWidget *clist;
//分栏窗口

/*****************************************************
功能：创建分栏窗口包含分栏窗口内部的构件的创建
返回值：GtkWidget* hpaned 菜单栏构件
参数：void
日期：2012－9－8
作者：张建涛
*****************************************************/
GtkWidget* paned(void)
{
	GtkWidget *vpaned;
	GtkWidget *hpaned;
	
	GtkWidget *vpaned_1;//右侧的vpaned

	
	GtkWidget *scrolled_window;
	GtkWidget *clist1;
	GtkWidget *scrolled_window1;

	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *hbox_1;
	GtkWidget *vbox_1;
	GtkWidget *addfriend_button;
	GtkWidget *deletefriend_button;
	GtkWidget *remakefriend_button;
	
	GtkWidget *text;
	GtkWidget *view;

//测试
	GtkTextBuffer *buffer;



	GtkWidget *search_button;
	GtkWidget *search_entry;
	
	GtkTreeSelection *selection;

	gchar* titles[4]={"ID","发件人","主题","接收时间"};
	gchar* titles_1[2]={"联系人","账号"};
	
	hpaned = gtk_hpaned_new();
	vpaned = gtk_vpaned_new();
	vpaned_1 = gtk_vpaned_new();
	
	hbox=gtk_hbox_new(FALSE,0);
	hbox_1=gtk_hbox_new(FALSE,0);
	vbox=gtk_vbox_new(FALSE,0);
	vbox_1=gtk_vbox_new(FALSE,0);
	
//分栏窗口
	scrolled_window=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (	scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
	clist = gtk_clist_new_with_titles(4,titles);

	gtk_clist_set_column_visibility (GTK_CLIST(clist),0,FALSE);	

	gtk_clist_set_column_width (GTK_CLIST(clist), 0, 10);
	gtk_clist_set_column_width (GTK_CLIST(clist), 1, 100);
	gtk_clist_set_column_width (GTK_CLIST(clist), 2, 300);
	
	
	gtk_container_add(GTK_CONTAINER(scrolled_window), clist);

//在主窗口的左侧创建联系人列表

	scrolled_window1=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (	scrolled_window1),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
	clist1 = gtk_clist_new_with_titles(4,titles_1);
	gtk_clist_set_column_visibility (GTK_CLIST(clist1),2,FALSE);
	gtk_clist_set_column_visibility (GTK_CLIST(clist1),3,FALSE);
	gtk_clist_set_column_width (GTK_CLIST(clist1), 0, 80);
	//gtk_signal_connect(GTK_OBJECT(clist), "select_row",G_CALLBACK(selection_made),text);
	gtk_container_add(GTK_CONTAINER(scrolled_window1), clist1);
	addfriend_button=gtk_button_new_with_label("添加联系人");	
	deletefriend_button=gtk_button_new_with_label("删除联系人");
	remakefriend_button=gtk_button_new_with_label("修改联系人");
//点击添加联系人按钮是的消息响应
		//显示联系人	
	show_contact(clist1);
	//获取焦点
	gtk_signal_connect(GTK_OBJECT(clist1), "select_row",G_CALLBACK(selection_linkman),NULL);
	//添加联系人
	gtk_signal_connect(GTK_OBJECT(addfriend_button), "clicked",G_CALLBACK(add_linkman),NULL);
	//更改联系人
	gtk_signal_connect(GTK_OBJECT(remakefriend_button), "clicked",G_CALLBACK(update_linkman),NULL);
	//删除联系人
	gtk_signal_connect(GTK_OBJECT(deletefriend_button), "clicked",G_CALLBACK(delete_linkman),NULL);
	
//点击修改联系人按钮是的消息响应
//为了和addfriendbutton 区分开，给回调函数传递一个非0参数
	
//邮件文本text相关
	text=gtk_text_view_new ();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text),FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));
	//gtk_text_buffer_set_text (buffer, "Hello, this is some text", -1);
//选中邮件的响应
	gtk_signal_connect(GTK_OBJECT(clist), "select_row",G_CALLBACK(selection_made),buffer);

	view=create_view_and_model();
//树视图消息响应
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	g_signal_connect(selection, "changed", G_CALLBACK(on_changed),clist);

	gtk_paned_add1(GTK_PANED(hpaned), vpaned_1);
	gtk_paned_add2(GTK_PANED(hpaned), vbox);	
	
	gtk_paned_add1(GTK_PANED(vpaned_1), view);
	gtk_paned_add2(GTK_PANED(vpaned_1), vbox_1);	
	
	gtk_box_pack_start(GTK_BOX(vbox_1),scrolled_window1,TRUE,TRUE,0);
	gtk_box_pack_end(GTK_BOX(vbox_1),hbox_1,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox_1),addfriend_button,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox_1),remakefriend_button,FALSE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(hbox_1),deletefriend_button,FALSE,FALSE,0);

	gtk_paned_add1(GTK_PANED(vpaned), scrolled_window);//把scrolled_window添加到vpaned1
	gtk_paned_add2(GTK_PANED(vpaned),text);//添加text到vpaned2

	//添加邮件搜索栏
	search_entry=gtk_entry_new();
	search_button=gtk_button_new_with_label("搜索");
	gtk_box_pack_end(GTK_BOX(hbox),search_button,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox),search_entry,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(search_button),"clicked",G_CALLBACK(search_email),search_entry);	
	
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),vpaned,TRUE,TRUE,0);
	
	return hpaned;
}


