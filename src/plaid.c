/*
 * Copyright (c) 2018 int16h <int16h@openbsd.space>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <sys/socket.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "http.h"
#include "util.h"

static void usage()
{
	fprintf(stderr, "usage: %s [-u url]\n", getprogname()); 
	exit(1);
}

int main(int argc, char *argv[])
{
char *url = "", *host = "", *cafile = get_path_ca(), *path = "/";
int i, ch, staplefd = -1, infd = -1, nonce = 1;
http_request *request = NULL;
size_t rescount, httphsz = 0; 
size_t postsz;
/* struct source addr; */
char    *ip;
struct httphead	*httph = NULL;
struct httpget *hget;
ssize_t written, w;
short port;
/*	if (pledge("stdio inet rpath dns", NULL) == -1)
		err(1, "pledge");
*/
/*	if (unveil("/tmp", "w") == -1)
		err(1, "unveil");
	if (unveil(cafile, "r") == -1)
		err(1, "unveil");	
*/
if(argc < 2){
	usage();
}
if(argv[2])
	url=argv[2];

printf("URL: %s\t", url);
printf("CA: %s\n", cafile);

if ((host = url2host(url, &port, &path)) == NULL)
	errx(1, "url2host failed");
if (*path == '\0')
	path = "/";

printf("Host: %s\tPort: %d\tPath: %s\n", host, port, path);

/* ip = lookup_host(host); */
ip = lookup_host(host);
	printf("IP: %s\n", ip);
/*		hget = http_get(sources, rescount, host, port, path,
		    request->body, postsz);
*/
		hget = http_get(ip, host, port, path);
		if (hget == NULL)
			errx(1, "http_get");
		free(ip);
}
