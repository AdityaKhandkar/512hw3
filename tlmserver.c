

/* Internet domain, connection-oriented SERVER   */

#include "tlmlocal.h"
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
	  if (fork() == 0)  {                      /* In CHILD process     */
	    char temp;
	    int bound;
			int score = 0;
			char pts[1000];
			char score_ptr[100];
			char substr[3];
			len = read(new_sock, buf, BUFSIZ); // Read the substring they set for the first time.
			if(len > 0)
				strncpy(substr, buf, 3);

	    while ((len=read(new_sock, buf, BUFSIZ)) > 0) {
				memset(score_ptr, 0, sizeof(score_ptr));
			// memset(buf, 0, sizeof(buf));	
				if(strstr(buf, "3")){ // Set

	    	// char response1[] = "What are you setting the substring to?\n";
		    	write(new_sock, "ok", 2);
					memset(buf, 0, sizeof(buf));
					len=read(new_sock, buf, BUFSIZ);
					// memset(substr, 0, sizeof(substr));
					strncpy(substr, buf, 3);
					// printf("%s", substr);
					// strncpy(buf, "OK\n", len);
					// write(new_sock, buf, len);
		//----------------------------------------------------------------------------------------------
					printf("\n---\n");
					printf("Inside Set\n");
					printf("Substring: %s\n", substr);
					printf("buffer: %s",buf);
					printf("\n------------\n");
				}
				else if(strstr(buf, "1")){ // Submit
			 // char response2[] = "What is your submission?\n";
					strncpy(buf, "OK", len);
					write(new_sock, buf, len);

					memset(buf, 0, sizeof(buf));

					len=read(new_sock, buf, BUFSIZ);

					printf("\n---\n");
					printf("Inside Submit\n");
					printf("Substring: %s\n",substr);
					printf("buffer: %s", buf);

					printf("\n------------\n");

					if(strstr(buf, substr)){
						printf("hit strstr\n");
						char buff2[255];
	   				int isWord;
						isWord = 0;
	   				FILE *fp = fopen("/../../usr/share/dict/american-english", "r");
						while( fgets(buff2, 255, (FILE*)fp)!=NULL){
							// sprintf(pts, "this is buff2: %s", buff2);
							// sprintf(pts, "this is buf: %s", buf);

							if (!strcmp(buff2, buf)){
								printf("Word %s is correct\n", buf);
								isWord =1;
								score = score+1;
								printf("score is: %d\n", score);
								sprintf(score_ptr, "%d + 1 = %d", score-1, score);
								printf("score_ptr: %s\n", score_ptr);
								strcat(score_ptr, "\0");
								// int score_to_send = htons(score);
								write(new_sock, score_ptr, sizeof(score_ptr));
								// memset(pts, 0, sizeof(pts));
								break;
							}
						}
						fclose(fp);
						if (!isWord){
							printf("%s is not word\n", buf);
							score = score-1;
							printf("score is: %d\n", score);
							sprintf(score_ptr, "%d - 1 = %d\nReason: Not a word",score+1, score);
							strcat(score_ptr, "\0");
							write(new_sock, score_ptr, sizeof(score_ptr));
							memset(score_ptr, 0, sizeof(score_ptr));
						}
					}	
					else{
						score = score-1;
						printf("%s does not contain substring\n", buf);
						printf("score is: %d\n", score);
						sprintf(score_ptr, "%d - 1 = %d\nReason: Doesn't contain substring", score+1, score);
						strcat(score_ptr, "\0");	
						write(new_sock, score_ptr, sizeof(score_ptr));
						memset(score_ptr, 0, sizeof(score_ptr));
					}
					// char out[1];
					// out[0] = score; 
					printf("\nScore: %d", score);
					printf("\nscore_ptr: %s", score_ptr);
				}
				else if(strstr(buf, "2")){  //LIST
          printf("---------------------------------------\n");
          printf("-----------------ENTER LIST----------------------\n");
          printf("---------------------------------------\n");
          printf("hitting");
          char word_in_list[30];
          FILE *fp = fopen("/../../usr/share/dict/american-english", "r");
          while(fgets(word_in_list, 30, (FILE*)fp)!= NULL) {
              if (strstr(word_in_list, substr)){
                  printf("%s", word_in_list);
                  write(new_sock, word_in_list, sizeof(word_in_list));
              }
              memset(word_in_list, 0, sizeof(word_in_list));
          }
          fclose(fp);
          strncpy(word_in_list, "-", 1);
          write(new_sock, word_in_list, sizeof(word_in_list));
          memset(word_in_list, 0, sizeof(word_in_list));
//                read(new_sock, buf, BUFSIZ);

          printf("---------------------------------------\n");
          printf("-----------------EXIT LIST----------------------\n");
          printf("---------------------------------------\n");
        }
				else{ // Quit
					memset(score_ptr, 0, sizeof(score_ptr));
					sprintf(score_ptr, "%d", score);
					write(new_sock, score_ptr, strlen(score_ptr));
				}

		      if ( buf[0] == '.' ) break;            /* are we done yet?     */
		    }
	    close(new_sock);                         /* In CHILD process     */
	    exit( 0);
	  } else close(new_sock);                    /* In PARENT process    */
  } while( 1 );                              /* FOREVER              */
}
