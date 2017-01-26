#include "SctpSocket.h"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
#include <cstdlib>
#include <cstdio>
#include <corecrt_io.h>

static int receive_cb(struct socket *sock, union sctp_sockstore addr, void *data,
	size_t datalen, struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
	if (data == nullptr) {
		done = 1;
		usrsctp_close(sock);
	}
	else {
#ifdef _WIN32
		_write(_fileno(stdout), data, (unsigned int)datalen);
#else
		if (write(fileno(stdout), data, datalen) < 0) {
			perror("write");
		}
#endif
		free(data);
	}
	return (1);
}

//#endif