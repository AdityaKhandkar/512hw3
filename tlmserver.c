/* Internet domain, connection-oriented SERVER   */

#include "local.h"
int main ( void )  {
  int            orig_sock,  /* Original socket descriptor in server */
                 new_sock,   /* New socket descriptor from connect   */
                 clnt_len;   /* Length of client address             */
  struct sockaddr_in
                 clnt_adr,   /* Internet address of client & server  */
                 serv_adr;
  int            len, i;     /* Misc counters, etc.                  */

  if ((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("generate error");
    exit(1);
  }
  
  memset ( &serv_adr, 0, sizeof(serv_adr) );  /* Clear structure     */
  serv_adr.sin_family       = AF_INET;        /* Set address type    */
  serv_adr.sin_addr.s_addr  = htonl(INADDR_ANY); /* Any interface    */
  serv_adr.sin_port         = htons(PORT);    /* Use our fake port   */
                                              /* Now BIND            */
  if (bind( orig_sock, (struct sockaddr *) &serv_adr,  
            sizeof(serv_adr)) <0) {
    perror("bind error");
  close(orig_sock);
  exit(2);
  }
  if (listen(orig_sock, 5) <0) {              /* LISTEN              */
    perror("listen error");
    exit(3);
  }
  do {
  clnt_len = sizeof(clnt_adr);
    if ((new_sock = accept ( orig_sock, (struct sockaddr *) &clnt_adr,
                             &clnt_len)) <0)  {      /* ACCEPT       */
    perror("accept error");
    close(orig_sock);
    exit(4);
  }
  if ( fork( ) == 0)  {                      /* In CHILD process     */
    char temp;
    int bound;
	int score = 0;
	char pts[1000];
    while ( (len=read(new_sock, buf, BUFSIZ)) > 0) {
		char substr[]="eer";
		if(strstr(buf, "Set\n")){
		    char response1[] = "What are you setting the substring to?\n";
		    //write(new_sock, response1, 20);
			strncpy(buf, "OK\n", len);
			write(new_sock, buf, len);
//----------------------------------------------------------------------------------------------
			//len=read(new_sock, buf, BUFSIZ);
			//strncpy(substr, buf, len);
			//printf(substr);
			//strncpy(buf, "OK\n", len);
			//write(new_sock, buf, len);
//----------------------------------------------------------------------------------------------
			score = 0;
			
			
		}
		else if(strstr(buf, "Submit\n")){
		    char response2[] = "What is your submission?\n";
		    //write(new_sock, response2, 20);
			strncpy(buf, "OK\n", len);
			write(new_sock, buf, len);
//----------------------------------------------------------------------------------------------
			len=read(new_sock, buf, BUFSIZ);
			printf("goodday");
			if(strstr(buf, substr)){
				char buff2[255];
   				int isWord;
				isWord = 0;
   				FILE *fp = fopen("/../../usr/share/dict/american-english", "r");
				while( fgets(buff2, 255, (FILE*)fp)!=NULL){
					if (!strcmp(buff2, buf)){
						isWord =1;
						score = score+1;
						sprintf(pts, "%d\n", score);
						strncpy(buf, pts, len);
						write(new_sock, buf, len);
						break;
						}
				}
				fclose(fp);
				if (!isWord){
						score = score-1;
						sprintf(pts, "%d\n", score);
						strncpy(buf, pts, len);
						write(new_sock, buf, len);
				}
			}	
			else	{
				score = score-1;
				sprintf(pts, "%d\n", score);
				strncpy(buf, pts, len);
				write(new_sock, buf, len);
			}
//----------------------------------------------------------------------------------------------
		}
		else if(strstr(buf, "List\n")){
			char word_in_list[255];
		    	char response2[] = "Here is the list:\n";
		    //write(new_sock, response2, 20);
   			FILE *fp = fopen("/../../usr/share/dict/american-english", "r");
			while( fgets(word_in_list, 255, (FILE*)fp)!=NULL){
				if (!strcmp(word_in_list, buf)){
				    write(new_sock, buf, 10);
				}
			}
			fclose(fp);
		}
		else if(strstr(buf, substr)){
			char buff2[255];
   			int isWord;
			isWord = 0;
   			FILE *fp = fopen("/../../usr/share/dict/american-english", "r");
			while( fgets(buff2, 255, (FILE*)fp)!=NULL){
				if (!strcmp(buff2, buf)){
					isWord =1;
					score = score+1;
					sprintf(pts, "%d\n", score);
					strncpy(buf, pts, len);
					write(new_sock, buf, len);
					break;
					}
			}
			fclose(fp);
			if (!isWord){
					score = score-1;
					sprintf(pts, "%d\n", score);
					strncpy(buf, pts, len);
					write(new_sock, buf, len);
			}
		}
		else{
			score = score-1;
			sprintf(pts, "%d\n", score);
			strncpy(buf, pts, len);
			write(new_sock, buf, len);
		}
      if ( buf[0] == '.' ) break;            /* are we done yet?     */
    }
    close(new_sock);                         /* In CHILD process     */
    exit( 0);
  } else close(new_sock);                    /* In PARENT process    */
  } while( 1 );                              /* FOREVER              */
}
