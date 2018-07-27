#ifndef MYSOCKET_H
#define MYSOCKET_H
#define FILE_BUFFER_SIZE 1024*5
int connect_socket(char* server,int);
int send_msg(int socket,char* sendBuff,int length);
int recv_msg(int socket,char* buffer,int bufflength);
int close_socket(int socket);

void writeto (int new_fd, char *buffer);
int send_file (char *file_name, int pcEmailId,int sockfd);
int send_attachedfile (char *file_name,int pcEmailId, int sockfd);
#endif
