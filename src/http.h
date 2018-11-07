/*
 * Copyright (c) 2018 int16h <int16h@openbsd.space>
 * Copyright (c) 2016 Kristaps Dzonsons <kristaps@bsd.lv>
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

#ifndef HTTP_H
#define HTTP_H

extern char* get_path_ca();

struct	source {
	int	 family; /* 4 (PF_INET) or 6 (PF_INET6) */
	char	*ip; /* IPV4 or IPV6 address */
};

struct	http;

typedef enum Method {UNSUPPORTED, GET, HEAD} Method;

typedef struct Header {
    char *name;
    char *value;
    struct Header *next;
} Header;

typedef struct http_request {
        enum Method method;
        char *url;
        char *version;
        struct Header *headers;
        char *body;
} http_request;

/*
 * Write and read callbacks to allow HTTP and HTTPS.
 * Both of these return the number of bytes read (or written) or -1 on
 * failure.
 * 0 bytes read means that the connection has closed.
 */
typedef	ssize_t (*writefp)(const void *, size_t, const struct http *);
typedef	ssize_t (*readfp)(char *, size_t, const struct http *);

/*
 * HTTP/S header pair.
 * There's also a cooked-up pair, "Status", with the status code.
 * Both strings are NUL-terminated.
 */
struct	httphead {
	const char	*key;
	const char	*val;
};

/*
 * Grab all information from a transfer.
 * DO NOT free any parts of this, and editing the parts (e.g., changing
 * the underlying strings) will persist; so in short, don't.
 * All of these values will be set upon http_get() success.
 */
struct	httpget {
	struct httpxfer	*xfer; /* underlying transfer */
	struct http	*http; /* underlying connection */
	int		 code; /* return code */
	struct httphead	*head; /* headers */
	size_t		 headsz; /* number of headers */
	char		*headpart; /* header buffer */
	size_t		 headpartsz; /* size of headpart */
	char		*bodypart; /* body buffer */
	size_t		 bodypartsz; /* size of bodypart */
};

int		 http_init(void);

/* Convenience functions. */
struct httpget	*http_get(const char *, const char *, short, const char *);
void		 http_get_free(struct httpget *);
/* void		http_get(const char *, const char *, short, const char *); */


/* Allocation and release. */
struct http	*http_alloc(const char *, const char *, short, const char *);
void		 http_free(struct http *);
struct httpxfer	*http_open(const struct http *);
void		 http_close(struct httpxfer *);
void		 http_disconnect(struct http *);

/* Access. */
char		*http_head_read(const struct http *,
			struct httpxfer *, size_t *);
struct httphead	*http_head_parse(const struct http *,
			struct httpxfer *, size_t *);
char		*http_body_read(const struct http *,
			struct httpxfer *, size_t *);
int		 http_head_status(const struct http *,
			struct httphead *, size_t);
struct httphead	*http_head_get(const char *,
			struct httphead *, size_t);
struct Request *parse_request(const char *raw);
void free_header(struct Header *h);
void free_request(struct Request *req);

#endif /* HTTP_H */
