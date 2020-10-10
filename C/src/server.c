#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>

#define PORT 8080
#define HOST_IP "127.0.0.1"

char* replace_ip(char* html_template, char* ip_addr, char* response) {

	strcpy(response,html_template);		

	//this is where we want to do the replacement
	char* ip_token;
	ip_token = strstr(response,"IP            ");
	strncpy(ip_token,ip_addr, strlen(ip_addr)); // Don't copy the terminating null byte

	return(response);

}


void handle_connection(char* html_template, int client_fd, char* ip_addr) {

	//response is same size as template
	char* response = (char*)malloc(sizeof(html_template));
	
	response = replace_ip(html_template, ip_addr, response);

	char* thistok = strtok(response, "^");

	//send the response line by line
	while(thistok) {
	
		write(client_fd, thistok, strlen(thistok)); 
		puts(thistok);
		thistok = strtok(NULL, "^");

	
	}

	free(response);



}


int main(void) {

	char html_template[] = "HTTP/1.1 200 OK\r\n^"
	"Content-Type: text/html; charset=UTF-8\r\n\r\n^"
	"<html>^"
	"<head>^"
	"</head>^"
	"<h1>Your ip address is: IP            </h1>^"
	"<p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Amet purus gravida quis blandit turpis cursus in hac. Id aliquet lectus proin nibh nisl condimentum id venenatis. Mi in nulla posuere sollicitudin aliquam. Vulputate odio ut enim blandit volutpat maecenas. Donec adipiscing tristique risus nec. Tempus iaculis urna id volutpat lacus laoreet non curabitur gravida. Dui accumsan sit amet nulla facilisi morbi tempus iaculis urna. Nec ultrices dui sapien eget mi proin sed libero. Dui ut ornare lectus sit. Dictumst quisque sagittis purus sit amet. Non odio euismod lacinia at quis. Nullam vehicula ipsum a arcu cursus. Vestibulum lectus mauris ultrices eros.^"
	"Eu mi bibendum neque egestas. Elit sed vulputate mi sit. Id diam maecenas ultricies mi. Sit amet est placerat in egestas erat imperdiet. Etiam erat velit scelerisque in dictum non. Aliquam faucibus purus in massa. Imperdiet dui accumsan sit amet nulla facilisi morbi tempus. A cras semper auctor neque vitae tempus quam pellentesque. Vel orci porta non pulvinar neque laoreet suspendisse. Varius morbi enim nunc faucibus a. Ultricies mi eget mauris pharetra. Lobortis elementum nibh tellus molestie nunc. Sit amet volutpat consequat mauris. Urna condimentum mattis pellentesque id. In hac habitasse platea dictumst. Sapien eget mi proin sed. Sit amet mauris commodo quis imperdiet massa. Dolor purus non enim praesent elementum facilisis leo vel fringilla. Vitae proin sagittis nisl rhoncus mattis rhoncus urna.^"
	"Cras adipiscing enim eu turpis. Aliquam ultrices sagittis orci a scelerisque. Enim eu turpis egestas pretium. Gravida quis blandit turpis cursus. Id diam maecenas ultricies mi eget mauris pharetra et ultrices. Habitant morbi tristique senectus et netus et malesuada fames ac. Molestie at elementum eu facilisis sed odio. Dui faucibus in ornare quam viverra orci sagittis eu volutpat. Laoreet sit amet cursus sit. Blandit volutpat maecenas volutpat blandit aliquam etiam. Id faucibus nisl tincidunt eget nullam non nisi est sit. Cursus mattis molestie a iaculis. Tellus at urna condimentum mattis pellentesque id nibh tortor id. Natoque penatibus et magnis dis parturient montes nascetur ridiculus mus. Amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor. In fermentum posuere urna nec. Ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue eget. Dolor sed viverra ipsum nunc aliquet bibendum enim facilisis. Dolor sed viverra ipsum nunc aliquet bibendum enim.^"
	"Nullam vehicula ipsum a arcu cursus vitae congue mauris. Libero nunc consequat interdum varius sit. Aliquet risus feugiat in ante metus dictum. Amet mattis vulputate enim nulla aliquet porttitor lacus luctus. Suspendisse ultrices gravida dictum fusce ut. Aenean et tortor at risus viverra. Eget egestas purus viverra accumsan in nisl nisi scelerisque. Id diam maecenas ultricies mi eget mauris. Orci eu lobortis elementum nibh tellus molestie nunc non. Risus nec feugiat in fermentum posuere urna nec tincidunt. Nunc aliquet bibendum enim facilisis gravida neque convallis a cras. Vel pretium lectus quam id leo in vitae turpis massa. Enim tortor at auctor urna nunc id cursus. Consequat ac felis donec et odio pellentesque diam volutpat commodo.^"
	"Leo urna molestie at elementum eu. Neque convallis a cras semper auctor neque. Sed blandit libero volutpat sed cras ornare arcu. Euismod elementum nisi quis eleifend quam adipiscing vitae. Vestibulum lorem sed risus ultricies tristique nulla. Neque vitae tempus quam pellentesque nec nam aliquam sem et. Pellentesque sit amet porttitor eget dolor morbi non arcu. Sit amet mauris commodo quis imperdiet. Duis at tellus at urna condimentum mattis. Vitae proin sagittis nisl rhoncus mattis rhoncus urna neque viverra. A iaculis at erat pellentesque adipiscing commodo elit at. Maecenas pharetra convallis posuere morbi leo. </p>^"
	"</html>^";

/*
	char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
"<style>body { background-color: #111 }"
"h1 { font-size:4cm; text-align: center; color: black;"
" text-shadow: 0 0 2mm red}</style></head>"
"<body><h1>Goodbye, world!</h1></body></html>\r\n";
 */

	int one = 1, client_fd;
	struct sockaddr_in svr_addr, cli_addr;
	socklen_t sin_len = sizeof(cli_addr);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		err(1, "ERROR: Unabel to open socket");
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(PORT);

	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
		close(sock);
		err(1, "ERROR: Unable to bind address");
	}

	listen(sock, 10);

	for (;;) {
		client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
		if (client_fd == -1) {
			perror("Can't accept");
			continue;
		}

		if (!fork()) {

			//print ip
			struct sockaddr_in* p_addr =  &cli_addr;
			char ip_str[INET_ADDRSTRLEN];
			inet_ntop( AF_INET, &p_addr->sin_addr, ip_str, INET_ADDRSTRLEN );
			printf("INFO: Connection from %s\n", ip_str);

			//render page	
			handle_connection(html_template, client_fd, ip_str);

			puts("INFO: Served page");	

			close(client_fd);	
			exit(0);
		}

		close(client_fd);	

	}


}