/*本文件中的函数用于邮件选项相关消息的响应*/

#include<gtk/gtk.h>
#include<string.h>
#include"../include/WidgetStruct.h"
#include "../../function/include/localfunc.h"
#include "../../function/include/DataStruct.h"
#include"../../function/include/mysocket.h"

//extern GtkTextBuffer *buffer;
GtkWidget *addlinkwindow;
gpointer linkpanel;
#define MAILENUM 100
char *value;
char* emailId;
extern char g_userId[25];
FileName filename;
extern GtkWidget *clist;
char maileTextbuffer[1024];


GtkWidget *updateWindow;
char linkManName[50];
char linkManID[25];
int itemId = -1;

//extern GtkTextBuffer *buffer;
void realize_text (GtkWidget *text, gpointer data);
/*************test******************************
显示邮件内容
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊*/
void selection_made( GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer* data )
{

	gchar *text="123456";

   
    stMail mail[MAILENUM];
    int num;
   // g_print("%s\n",value);
	char strhead[20];
    if(0 == strcmp(value,"收件箱"))
	{
        sprintf(strhead,"./data/%s/recv/",g_userId);
	}
	else if(0 == strcmp(value,"发件箱"))
	{
		sprintf(strhead,"./data/%s/send/",g_userId);
	}
	else if(0 == strcmp(value,"草稿"))
	{
	 	sprintf(strhead,"./data/%s/draft/",g_userId);
	}
	num=GetLocalMail (mail,strhead);


/* 取得存储在被选中的行和列的单元格上的文本
* 当鼠标点击时，我们用t e x t参数接收一个指针*/
    
	gtk_clist_get_text(GTK_CLIST(clist), row, 0, &emailId);
	gtk_clist_get_text(GTK_CLIST(clist), row, 0, &filename.emailId);
	gtk_clist_get_text(GTK_CLIST(clist), row, 1, &filename.target);
	gtk_clist_get_text(GTK_CLIST(clist), row, 2, &filename.title);
	gtk_clist_get_text(GTK_CLIST(clist), row, 3, &filename.time);

   int i=0;
    for(;i<num;i++)
    {
        //g_print("%s emailId=%s",mail[i].acEmailID,emailId);
       if(strcmp(mail[i].acEmailID,emailId)==0)
		{ //g_print("%s emailId=%s",mail[i].acEmailID,emailId);
			
            maileTextbuffer[0]='\0';
            char tmp[100];
            char str[100];
       		sprintf(str,"%s%s|%s|%s|%s|.txt",strhead,mail[i].acEmailID,mail[i].acTitle,mail[i].acSendTargetID,mail[i].acEmailTime);
			//g_print("%s\n",str);
            FILE* fp=NULL;
            fp=fopen(str,"r");
			if(fp==NULL)
			{
				g_print("open erro\n");
			}
			else
            {
				int readlength=0;
				while ((readlength =fread (tmp, sizeof (char), 100, fp)) > 0)
				{
							tmp[readlength]='\0';
				    		//g_print("tmp=%s",tmp);
							strcat(maileTextbuffer,tmp);
				}
			}
		}
	}
//设置文本内容
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(data), maileTextbuffer, -1);
    memset(maileTextbuffer,'\0',1024);

	return;
}



/*************test******************************
显示邮件列表
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊*/

void show_receive(stMail* pemail,int num ,gpointer data)
{
	int indx;
/* 字符串数组，用于加到l i s t中，4行2列*/
	gtk_clist_clear( (GtkCList *) data);

     gchar* buffer[4];
     int ii=0;
     for(;ii<4;ii++)
     {
         buffer[ii]=(gchar*)malloc(sizeof(gchar)*20);
     }
/*下面将文本真正加到l i s t中。对每行添加一次*/
	for ( indx=0 ; indx < num ; indx++ )
	{
        strcpy(buffer[0],pemail[indx].acEmailID);
		strcpy(buffer[1],pemail[indx].acSendTargetID);
		strcpy(buffer[2],pemail[indx].acTitle);
		strcpy(buffer[3],pemail[indx].acEmailTime);
        //g_print("%s\n",buffer[1]);
	   gtk_clist_append( (GtkCList *) data, buffer);
     }
//gtk_clist_set_background((GtkCList *)data,1,"green");

	return;
}

void show_contact( gpointer data )
{
	linkpanel = data;
	extern char g_userId[25];
	char temp[50];
	int index = 0;
	int num = 0;
	int length = 0;
	int i ;
	int j = 0;
	int k = 0;
	memset(temp, '\0', 50);
    char* buffer[4];
    int ii=0;
    for(;ii<4;ii++)
    {
        buffer[ii]=(gchar*)malloc(sizeof(char) * 50);
    }
/* 字符串数组，用于加到l i s t中，4行2列*/
	gtk_clist_clear( (GtkCList *) data);
////////////////////write here //////////////////////////////////////
	char *pacContact[50];

	GetContactInfo(g_userId, pacContact, &num);
/*下面将文本真正加到l i s t中。对每行添加一次*/
	for (; index < num ; ++index )
	{	
		k = 0;
		length = strlen(pacContact[index]);
		//g_print("该条记录长度%d %s\n",length, pacContact[index]);	
		for (i = 0; i < length; ++i)
		{	
			if (pacContact[index][i] == '|')
			{
				strcpy(buffer[k], temp);
				++k;
				memset(temp, '\0', 50);
				j = 0;
			}
			else
			{
				temp[j] = pacContact[index][i];
				++j;
			}
		}
		gtk_clist_append( (GtkCList *) data, buffer);
	}
//gtk_clist_set_background((GtkCList *)data,1,"green");

	return;
}


void show_send( gpointer data )
{

//先清空列表
	gtk_clist_clear( (GtkCList *) data);
////////////////////write here //////////////////////////////////////


	return;
}

void show_draft( gpointer data )
{

//先清空列表
	gtk_clist_clear( (GtkCList *) data);
////////////////////write here //////////////////////////////////////


return;
}


/*邮件搜索响应*/
void search_email(GtkWidget *search_button, gpointer data)
{
	 gtk_clist_clear( (GtkCList *) clist);
	const gchar* keyword = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
/////////////////////////////////write here ///////////////////////////////////
	//g_print("%s\n",keyword);
    stMail searchMail[100];
    int num=0;
    int i=0;
    char str[50];
    while(i<3)
	{
        if(0==i)
        {
            sprintf(str,"./data/%s/recv/",g_userId);
			num=SearchMail (keyword,NULL,str,searchMail);
            memset(str,'\0',strlen(str));
        }
        else if(1==i)
  		{
			sprintf(str,"./data/%s/send/",g_userId);
			num=SearchMail (keyword,NULL,str,searchMail);
			memset(str,'\0',strlen(str));
		}
		else if(2==i)
  		{
			sprintf(str,"./data/%s/draft/",g_userId);
			num=SearchMail (keyword,NULL,str,searchMail);
			memset(str,'\0',strlen(str));
		}
		gchar* buffer[4];
		 int ii=0;
		 int indx;
		 for(;ii<4;ii++)
		 {
		     buffer[ii]=(gchar*)malloc(sizeof(gchar)*20);
		 }
     	/*下面将文本真正加到l i s t中。对每行添加一次*/
		for ( indx=0 ; indx < num ; indx++ )
		{
		    strcpy(buffer[0],searchMail[indx].acEmailID);
			strcpy(buffer[1],searchMail[indx].acSendTargetID);
			strcpy(buffer[2],searchMail[indx].acTitle);
			strcpy(buffer[3],searchMail[indx].acEmailTime);
		    
		   gtk_clist_append( (GtkCList *) clist, buffer);
     	}
      i++;
	}
}


/************************************************
功能：作为点击邮箱选项的消息相应
返回值：void
参数：GtkWidget *widget, gpointer 
创建日期：2012-9-8
作者：张建涛
************************************************/
void on_changed(GtkWidget *widget, gpointer data) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
    stMail email[MAILENUM];
	enum{COLUMN = 0,NUM_COLS};
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
	{
		
		gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
		if(0 == strcmp(value,"收件箱"))
		{
			//在邮件列表显示收件箱邮件
            char str[50]; 
			//extern char g_userId[25];
			sprintf(str,"./data/%s/recv/",g_userId);
           
          //  sprintf(str,"./data/%s/recv/",g_userId);
           int num=GetLocalMail (email,str);
          //  g_print("%d\n",num);
			show_receive(email,num,data);
		}
		if(0 == strcmp(value,"发件箱"))
		{
			//在邮件列表显示收件箱邮件
            char str[50]; 
			//extern char g_userId[25];
			sprintf(str,"./data/%s/send/",g_userId);
           
          //  sprintf(str,"./data/%s/recv/",g_userId);
           int num=GetLocalMail (email,str);
          //  g_print("%d\n",num);
			show_receive(email,num,data);
		}
		if(0 == strcmp(value,"草稿"))
		{
			//在邮件列表显示收件箱邮件
            char str[50]; 
			//extern char g_userId[25];
			sprintf(str,"./data/%s/draft/",g_userId);
           
          //  sprintf(str,"./data/%s/recv/",g_userId);
           int num=GetLocalMail (email,str);
          //  g_print("%d\n",num);
			show_receive(email,num,data);

		}
	}
}

/*消息响应函数*/
//添加  修改  删除 联系人相关

void add_button_clicked(GtkWidget *widget, LinkmanWidget* data) 
{
	//在此处添加消息响应
	char *pacContact[128][3];
	char acContactName[50];
	char acCOntactId[25];
	char iItemId = 1;
	extern char g_userId[25];
	char buffer[2][50];
	memset(acContactName, '\0', 50);
	memset(acCOntactId, '\0', 25);
	
	int iAddResult;
	const gchar* a = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->LinkName));
	strcpy(acContactName,a);
	const gchar* b = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->LinkId));	
	strcpy(acCOntactId,b);
	
	iAddResult= AddContact (g_userId,acCOntactId, acContactName, iItemId);
	//iDeleteResult = DeleteContact(g_userId,acCOntactId);//删除联系人
	//iAddResult = ReviseContact(g_userId,"asdfasdf", acCOntactId, acContactName);
	//iAddResult = GetContactInfo(g_userId, pacContact);
	/* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 linkpanel
	gtk_entry_set_text(GTK_ENTRY(data->LinkName),"");
	gtk_entry_set_text(GTK_ENTRY(data->LinkId),"");*/
	printf("IADDRESULT %d\n", iAddResult);
	if (iAddResult == 0)
	{
		gtk_widget_destroy(GTK_WIDGET(addlinkwindow));
		show_contact(linkpanel);
		g_print("tian jia cheng gong \n");
	}
	else
	{
		g_print("tian jia shi bai\n");
	}
////////////////////write here //////////////////////////////////////
}

/************************************************
功能：创建联系人添加窗口，作为点击添加按钮的消息相应
返回值：int
参数：gpoint
创建日期：2012-9-8
作者：张建涛
************************************************/
int add_linkman(GtkWidget *widget,gpointer data)
{
	//GtkWidget *window;
	GtkWidget *name_label,*num_label;
	//GtkWidget *name_entry,*num_entry;
	GtkWidget *add_button;
	//GtkWidget *clear_button;
	GtkWidget *hbox1,*hbox2,*hbox3;
	GtkWidget *vbox;
//用于获取联系人信息的结构体
	LinkmanWidget info;
	
	
	gtk_init(0,0);

	addlinkwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(addlinkwindow),"添加联系人");
	gtk_window_set_position(GTK_WINDOW(addlinkwindow),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(addlinkwindow),10);

	g_signal_connect(G_OBJECT(addlinkwindow),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//设置窗口不可伸缩
	gtk_window_set_policy(GTK_WINDOW(addlinkwindow),FALSE,FALSE,TRUE);

	name_label = gtk_label_new(" 姓   名: ");
	num_label = gtk_label_new(" 帐   号: ");

	info.LinkName = gtk_entry_new();
	info.LinkId = gtk_entry_new();
	
	add_button = gtk_button_new_with_label("  确定  ");
	
//消息响应
	gtk_signal_connect(GTK_OBJECT(add_button),
	"clicked",GTK_SIGNAL_FUNC(add_button_clicked),&info);

	
	hbox1 = gtk_hbox_new(FALSE,1);
	hbox2 = gtk_hbox_new(FALSE,1);
	hbox3 = gtk_hbox_new(FALSE,0);
	vbox = gtk_vbox_new(FALSE,5);

	

	gtk_box_pack_start(GTK_BOX(hbox1),name_label,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox1),info.LinkName,TRUE,TRUE,0);
	
	gtk_box_pack_start(GTK_BOX(hbox2),num_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox2),info.LinkId,TRUE,TRUE,0);
	
	gtk_box_pack_end(GTK_BOX(hbox3),add_button,FALSE,TRUE,0);
	//gtk_box_pack_end(GTK_BOX(hbox3),clear_button,FALSE,TRUE,10);



	gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,FALSE,5);
	
	gtk_container_add(GTK_CONTAINER(addlinkwindow),vbox);
	 gtk_widget_show_all(addlinkwindow);
	if(NULL!=data)
	{
		gtk_window_set_title(GTK_WINDOW(addlinkwindow),"修改联系人");
	}
	gtk_main();
	return 0;

}

int delete_linkman()
{
	int iDeleteResult;
	extern char g_userId[25];
	if (-1 == itemId)
	{
		printf("您没有选中联系人！！\n");
		return;
	}
	printf("itemId = %d\n", itemId);
	printf("g_userId = %s", g_userId);
	iDeleteResult = DeleteContact(g_userId, itemId);//删除联系人
	if (iDeleteResult == 0)
	{
		show_contact(linkpanel);
		g_print("tian jia cheng gong \n");
	}
	else
	{
		g_print("tian jia shi bai\n");
	}
}

void update_button_clicked(GtkWidget *widget, LinkmanWidget* data)
{
	//在此处添加消息响应
	char *pacContact[128][3];
	char acContactName[50];
	char acContactId[25];
	char iItemId = 1;
	extern char g_userId[25];
	char buffer[2][50];
	memset(acContactName, '\0', 50);
	memset(acContactId, '\0', 25);
	
	int iUpdateResult;
	const gchar* a = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->LinkName));
	strcpy(acContactName,a);
	const gchar* b = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->LinkId));	
	strcpy(acContactId,b);
	
	//iAddResult= AddContact (g_userId,acCOntactId, acContactName, iItemId);
	//iDeleteResult = DeleteContact(g_userId,acCOntactId);//删除联系人
	iUpdateResult = ReviseContact(g_userId,itemId, acContactId, acContactName);
	//iAddResult = GetContactInfo(g_userId, pacContact);
	printf("IADDRESULT %d\n", iUpdateResult);
	if (iUpdateResult == 0)
	{
		gtk_widget_destroy(GTK_WIDGET(updateWindow));
		show_contact(linkpanel);
		g_print("tian jia cheng gong \n");
	}
	else
	{
		g_print("tian jia shi bai\n");
	}
////////////////////write here //////////////////////////////////////
}


int update_linkman(GtkWidget *widget,gpointer data)
{
	if (-1 == itemId)
	{
		printf("您没有选中联系人！！\n");
		return;
	}
	//GtkWidget *window;
	GtkWidget *name_label,*num_label;
	//GtkWidget *name_entry,*num_entry;
	GtkWidget *add_button;
	//GtkWidget *clear_button;
	GtkWidget *hbox1,*hbox2,*hbox3;
	GtkWidget *vbox;
//用于获取联系人信息的结构体
	LinkmanWidget info;
	
	gtk_init(0,0);

	updateWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(updateWindow),"修改联系人");
	gtk_window_set_position(GTK_WINDOW(updateWindow),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(updateWindow),10);

	g_signal_connect(G_OBJECT(updateWindow),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//设置窗口不可伸缩
	gtk_window_set_policy(GTK_WINDOW(updateWindow),FALSE,FALSE,TRUE);

	name_label = gtk_label_new(" 姓   名: ");
	num_label = gtk_label_new(" 帐   号: ");

	info.LinkName = gtk_entry_new();
	info.LinkId = gtk_entry_new();
	
	add_button = gtk_button_new_with_label("  确定  ");
	gtk_entry_set_text(GTK_ENTRY(info.LinkName),linkManName);
	gtk_entry_set_text(GTK_ENTRY(info.LinkId),linkManID);
//消息响应
	gtk_signal_connect(GTK_OBJECT(add_button),
	"clicked",GTK_SIGNAL_FUNC(update_button_clicked),&info);
	
	
	hbox1 = gtk_hbox_new(FALSE,1);
	hbox2 = gtk_hbox_new(FALSE,1);
	hbox3 = gtk_hbox_new(FALSE,0);
	vbox = gtk_vbox_new(FALSE,5);

	

	gtk_box_pack_start(GTK_BOX(hbox1),name_label,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox1),info.LinkName,TRUE,TRUE,0);
	
	gtk_box_pack_start(GTK_BOX(hbox2),num_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox2),info.LinkId,TRUE,TRUE,0);
	
	gtk_box_pack_end(GTK_BOX(hbox3),add_button,FALSE,TRUE,0);
	//gtk_box_pack_end(GTK_BOX(hbox3),clear_button,FALSE,TRUE,10);



	gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,FALSE,5);
	
	gtk_container_add(GTK_CONTAINER(updateWindow),vbox);
	gtk_widget_show_all(updateWindow);
	
	gtk_main();
	return 0;

}
void selection_linkman(GtkWidget *clist1,gint row,gint column,GdkEventButton *event, gchar* data)
{
	char *itemid;
	char *linmanname;
	char *linmanid;
	char itID[10];

	memset(linkManName,'\0', 50);
	memset(linkManName,'\0', 25);
	memset(itID,'\0', 10);
	gtk_clist_get_text(GTK_CLIST(clist1), row, 0, &linmanname);
	gtk_clist_get_text(GTK_CLIST(clist1), row, 1, &linmanid);
	gtk_clist_get_text(GTK_CLIST(clist1), row, 3, &itemid);
	strcpy(linkManName, linmanname);
	strcpy(linkManID, linmanid);
	strcpy(itID, itemid);
	itemId = atoi(itID);
	g_print("%s\n",linkManName);
	g_print("%s\n",linkManID);
	g_print("%s\n",itID);
	g_print("%d\n",itemId);

}


