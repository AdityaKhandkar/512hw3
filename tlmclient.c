/* Internet domain, connection-oriented CLIENT   */

#include "local.h"

int validate(char buf[], int len);

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

/* CONNECT                                                           */
  if (connect( orig_sock, (struct sockaddr *)&serv_adr,
              sizeof(serv_adr)) < 0) {
    perror("connect error");
    exit(4);
  }
  do {
    write(fileno(stdout), "> ", 3);                    /* Prompt user */
    if ((len=read(fileno(stdin), buf, BUFSIZ) ) > 0) { /* Get input   */
      //buf = tolower(buf);
      
      char localbuf[len];
      for(int i = 0; i < len; i++) {
        localbuf[i] = buf[i];
      }

      while(validate(localbuf, len) != 1) {
        write(fileno(stdout), "> ", 3);
        len=read(fileno(stdin), buf, BUFSIZ);
        strcpy(localbuf, buf);
      }

      // printf("%s\n", buf);
      write(orig_sock, localbuf, len);                      /* Write to sck*/
      if ((len=read(orig_sock, buf, len)) > 0)          /* If returned */
        write(fileno(stdout), buf, len);               /* Display it  */
      }
    } while( buf[0] != '.');

    close(orig_sock);
    exit(0);
} 

int validate(char buf[], int len) {

  char submit[] = "Submit";
  char list[] = "List";
  char quit[] = "Quit";
  char set[] = "Set";
  char temp[len - 1];

  for(int i = 0; i < len - 1; i++) {
    temp[i] = buf[i];
  }

  printf("buf: %s, submit: %s, list: %s, set: %s\n", temp, submit, list, set);

  if(strcmp(temp, submit) == 0 || strcmp(temp, list) == 0 || strcmp(temp, set) == 0) {
    return 1;
  }

  if(strcmp(temp, quit) == 0) {
    exit(0);
  }

  for(int i = 0; i < len - 1; i++) {
    if(!(buf[i] >= 97 && buf[i] <= 122) && !(buf[i] >= 65 && buf[i] <= 90)) {
      printf("Please enter only alphabetic characters.\n");
      return -1;
    }
  }

  return 1;
}
