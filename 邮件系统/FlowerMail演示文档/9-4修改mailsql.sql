/*创建用户表*/
CREATE TABLE UserTable
(
	userId varchar(25) NOT NULL PRIMARY KEY,
	nickName varchar(50) NOT NULL DEFAULT '新用户',
	passwd varchar(16) NOT NULL,
	state int,
	telephone varchar(11)
);
/*创建邮件表*/
CREATE TABLE EmailTable
(
	emailId varchar(25) NOT NULL PRIMARY KEY,
	title varchar(100) NOT NULL,
	content varchar(1024),
	type int NOT NULL,
	state int NOT NULL,
	sendTargetId varchar(500),
	copyTargetId varchar(500),
	emaiTime varchar(22),
	emailsize int,
	attachedFilePath varchar(50),
	userId varchar(25)
);
/*创建联系人表*/
CREATE TABLE ContactTable (
	itemId int NOT NULL PRIMARY KEY auto_increment, 
	userId varchar(25) NOT NULL, 
	contactName varchar(20),  
	contactId varchar(25) NOT NULL,
	teamId int NULL );
/*创建联系人组表*/
CREATE TABLE TeamTable
(
	teamId int NOT NULL PRIMARY KEY auto_increment,
	team Name VARCHAR(10)
);