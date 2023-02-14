# include <iostream>
# include <sys/types.h>          /* See NOTES */
# include <sys/socket.h>

int main()
{
	int server_fd = socket(domain, type, protocol);
	return 0;
}
