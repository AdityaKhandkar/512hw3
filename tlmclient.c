/* Internet domain, connection-oriented CLIENT   */

#include "local.h"

int validateInput(char buf[], int len);
int validateOption(char buf[], int len);
int validateSubstr(char buf[], int len);

int main ( int argc, char *argv[]) {

  int             orig_sock, /* Original socket descriptor in client */
                  len;       /* Length of server address             */
  struct sockaddr_in
                   serv_adr; /* Internet address of server           */
  struct hostent    *host;   /* The host (server)                    */

  if (argc != 2 ) {          /* Expect name of host on command line  */
    fprintf(stderr, "usage: %s server\n", argv[0]);
    exit(1);
  }
  host = gethostbyname(argv[1]);              /* Get host info       */
  if (host == (struct hostent *) NULL ) {
    perror("gethostbyname ");
    exit(2);
  }
  memset(&serv_adr, 0, sizeof( serv_adr));    /* Clear the structure */
  serv_adr.sin_family = AF_INET;              /* Set address type    */
  memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);  /* adr  */
  serv_adr.sin_port  = htons( PORT );         /* Use our fake port   */

  if ((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { /* SOCKET */
    perror("generate error");
    exit(3);
  }

	/* CONNECT */
  if (connect( orig_sock, (struct sockaddr *)&serv_adr,
              sizeof(serv_adr)) < 0) {
    perror("connect error");
    exit(4);
  }

  // do {
  //   write(fileno(stdout), "> ", 3);                    /* Prompt user */
  //   if ((len=read(fileno(stdin), buf, BUFSIZ) ) > 0) { /* Get input   */
  //     //buf = tolower(buf);
      
  //   char localbuf[len];
  //   for(int i = 0; i < len; i++) {
  //     localbuf[i] = buf[i];
  //   }

  //   while(validate(localbuf, len) != 1) {
  //     write(fileno(stdout), "> ", 3);
  //     len=read(fileno(stdin), buf, BUFSIZ);
  //     strcpy(localbuf, buf);
  //   }

  //   // printf("%s\n", buf);
  //   write(orig_sock, localbuf, len);                      /* Write to sck*/
  //   if ((len=read(orig_sock, buf, len)) > 0)          /* If returned */
  //     write(fileno(stdout), buf, len);               /* Display it  */
  //   }
  // } while( buf[0] != '.');

  char message_start_game[] = "Please enter a 3 letter set\n";
  char message_continue[] = "\nEnter one of the follwing to continue or Quit to quit\n";
  char message_validate_substr[] = "Please enter a 3 letter set only (no numbers/special characters)\n";
  char message_make_submission[] = "Make your submission\n";
  char message_make_submission_again[] = "Please enter your submission again\n";
  char message_only_alphabets[] = "Please enter only alphabetic characters\n";
  char message_list_words[] = "Here is the list of words that matches ";
  char message_set_replace[] = "Enter a 3 letter set to replace the current set\n";
  char message_score[] = "Your score is: ";
  char message_default[] = "Please only enter one of the 4 options\n";
  char substr[3];

  write(fileno(stdout), message_start_game, strlen(message_start_game));
  write(fileno(stdout), "> ", 3);
  len = read(fileno(stdin), buf, BUFSIZ);

  while(validateSubstr(buf, len) == -1) {
		write(fileno(stdout), message_validate_substr, strlen(message_validate_substr));
		write(fileno(stdout), "> ", 3);
		len = read(fileno(stdin), buf, BUFSIZ);
	}

  strncpy(substr, buf, 3);
  write(orig_sock, tolower(buf), 3); // Send the substring to the server

  write(fileno(stdout), message_continue, strlen(message_continue));

  // len = read(fileno(stdin), buf, BUFSIZ);

  // Keep playing the game until they quit
  while(len > 0 && strcmp(buf, quit) != 0) {
	  write(fileno(stdout), "\n", 1);
	  write(fileno(stdout), submit, strlen(submit));
	  write(fileno(stdout), "\n", 1);
	  write(fileno(stdout), list, strlen(list));
	  write(fileno(stdout), "\n", 1);
	  write(fileno(stdout), set, strlen(set));
	  write(fileno(stdout), "\n", 1);
	  write(fileno(stdout), quit, strlen(quit));
	  write(fileno(stdout), "\n\n", 2);
	  write(fileno(stdout), "> ", 3);


  	len = read(fileno(stdin), buf, BUFSIZ);

  	// int v = validateOption(buf, len);

  	switch(validateOption(buf, len)) {
  		case 1: { // Submit

  			write(orig_sock, (char *)"1", 1);
  			if((len = read(orig_sock, buf, BUFSIZ)) > 0)
	  			write(fileno(stdout), message_make_submission, strlen(message_make_submission));
  				// write(fileno(stdout), buf, len);
  				
  			write(fileno(stdout), ">>> ", 5);

				len = read(fileno(stdin), buf, BUFSIZ);
				
				// Validate that the string they entered is only alphabets
				while(len > 0 && validateInput(buf, len) == -1) {
	  			write(fileno(stdout), message_only_alphabets, strlen(message_only_alphabets));
	  			write(fileno(stdout), message_make_submission_again, strlen(message_make_submission_again));
	  			write(fileno(stdout), ">>> ", 5);
					len = read(fileno(stdin), buf, BUFSIZ);
				}

				// Send the word to the server
				write(orig_sock, buf, len);

				// Add code to display the reply from the server.
				len = read(orig_sock, buf, BUFSIZ);

				write(fileno(stdout), buf, len);

  		} break;

  		case 2: { // List

				write(fileno(stdout), message_list_words, strlen(message_list_words));
				write(fileno(stdout), substr, strlen(substr));
				write(fileno(stdout), ":\n", 2);

  			write(orig_sock, (char *)"2", 1);

  			// Listing all the words that match the substring
  			while((len = read(orig_sock, buf, BUFSIZ)) > 0) {
  				write(fileno(stdout), buf, strlen(buf));
  				write(fileno(stdout), "\n", 1);
  			}
  		} break;

  		case 3: { // Set
  			write(orig_sock, (char *)"3", 1);
  			write(fileno(stdout), message_set_replace, strlen(message_set_replace));
				write(fileno(stdout), ">>> ", 5);	
  			len = read(fileno(stdin), buf, BUFSIZ);
  			while(validateSubstr(buf, len) == -1) {
					write(fileno(stdout), message_validate_substr, strlen(message_validate_substr));
					write(fileno(stdout), ">>> ", 5);
					len = read(fileno(stdin), buf, BUFSIZ);
				}
				strncpy(substr, buf, 3);
  			write(orig_sock, buf, BUFSIZ);
  		} break;

  		case 4: { // Quit
  			write(fileno(stdout), message_score, strlen(message_score));
  			write(orig_sock, (char *)"4", 1);

  			exit(0);
  		} break;

  		default: write(fileno(stdout), message_default, strlen(message_default));
  	}
  }
    close(orig_sock);
    exit(0);
} 

int validateInput(char buf[], int len) {

  char temp[len - 1];

  for(int i = 0; i < len - 1; i++) {
    temp[i] = buf[i];
  }

  // printf("temp: %s, buf: %s", temp, buf);

  for(int i = 0; i < len - 1; i++) {
    if(!(buf[i] >= 97 && buf[i] <= 122) && !(buf[i] >= 65 && buf[i] <= 90)) {
    	memset(temp, 0, sizeof(temp));
      return -1;
    }
  }

  memset(temp, 0, sizeof(temp));
  return 1;
}

int validateOption(char buf[], int len) {
	char temp[len - 1];

  for(int i = 0; i < len - 1; i++) {
    temp[i] = buf[i];
  }

  // printf("temp: %s, buf: %s", temp, buf);

  if(strcmp(temp, submit) == 0) return 1;
  else if(strcmp(temp, list) == 0) return 2;
  else if(strcmp(temp, set) == 0) return 3;
  else if(strcmp(temp, quit) == 0) return 4;
  else return -1;
}

int validateSubstr(char buf[], int len) {

	if(len - 1 != 3) return -1;

	// printf("In validateSubstr, buf: %s, strlen(buf): %d\n", buf, len - 1);

	for(int i = 0; i < len - 1; i++) {
    if(!(buf[i] >= 97 && buf[i] <= 122) && !(buf[i] >= 65 && buf[i] <= 90)) {
      return -1;
    }
  }

  return 1;
}