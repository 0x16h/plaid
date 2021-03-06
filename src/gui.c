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

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "http.h"
#include "util.h"

static const char *event_names[] = {
   "",
   "",
   "KeyPress",
   "KeyRelease",
   "ButtonPress",
   "ButtonRelease",
   "MotionNotify",
   "EnterNotify",
   "LeaveNotify",
   "FocusIn",
   "FocusOut",
   "KeymapNotify",
   "Expose",
   "GraphicsExpose",
   "NoExpose",
   "VisibilityNotify",
   "CreateNotify",
   "DestroyNotify",
   "UnmapNotify",
   "MapNotify",
   "MapRequest",
   "ReparentNotify",
   "ConfigureNotify",
   "ConfigureRequest",
   "GravityNotify",
   "ResizeRequest",
   "CirculateNotify",
   "CirculateRequest",
   "PropertyNotify",
   "SelectionClear",
   "SelectionRequest",
   "SelectionNotify",
   "ColormapNotify",
   "ClientMessage",
   "MappingNotify"
};

extern int
gui_init(char *htfile)
{
	printf("gui_init(%s)\n", htfile);
	Display* display = XOpenDisplay(NULL);
	if (display == NULL) {
		return 1;
	}
	int screen = DefaultScreen(display);
	Font font;
/*	font = XLoadFont(display, "6x9"); */
	font = XLoadFont(display, 
		"-adobe-times-*-r-*-*-14-*-*-*-*-*-*-*");
/*		"-misc-dejavu sans-*-r-*-*-*-*-100-100-*-*-*-uni"); */
	GC gc = DefaultGC(display, screen);
	XSetFont(display, gc, font);
	Window parent_window = DefaultRootWindow(display);

	int x = 0;
	int y = 0;

	unsigned int width = 1024;
	unsigned int height = 600;
	unsigned int border_width = 1;
	unsigned int border_color = BlackPixel(display, screen);
	unsigned int background_color = WhitePixel(display, screen);

	Window main_window = XCreateSimpleWindow(display, parent_window,
		x,
		y,
		width,
		height,
		border_width,
		border_color,
		background_color);

	long event_mask = ExposureMask
		| KeyPressMask
		| KeyReleaseMask
		| ButtonPressMask
		| ButtonReleaseMask
		| FocusChangeMask
		;

	XSelectInput(display, main_window, event_mask);
	XMapWindow(display, main_window);
	XStoreName(display, main_window, "Welcome to plaid");
	Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, main_window, &wm_delete, 1);

	tinfoilhat();
	char msg[1024] = "";
	char key[32];


	int ch, lines = 0;
	FILE *fp;
	fp = fopen(htfile, "r");
	do {
		ch = fgetc(fp);
		if (ch == '\n')
			lines++;
	} while (ch != EOF);

	rewind(fp);
	char *contents[lines];
	int i = 0;
	size_t len = 0;
	
	for (i = 0; i < lines; i++)
	{
		contents[i] = NULL;
		len = 0;
		getline(&contents[i], &len, fp);
	}

	fclose(fp);

/*
FILE *fp = fopen(htfile, "rb");
fseek(fp, 0, SEEK_END);
long fsize = ftell(fp);
fseek(fp, 0, SEEK_SET);  //same as rewind(f);
char *nhtml = malloc(fsize + 1);
fread(nhtml, fsize, 1, fp);
fclose(fp);
nhtml[fsize] = 0;
char *cleanhtml = stripHTMLTags(nhtml, sizeof(nhtml));
printf("HTML: %s\n", nhtml[1]);
printf("HTML: %s", &cleanhtml);
*/
	/* printf("%.12s\n",htfile); */
/*	printf("number of lines: %d\n", lines);*/
/*	printf("first: %s\n", contents[0]); */
/*	printf("last: %s\n", contents[lines-1]); */

    /* Event loop */
	for (;;) {
	/* Get next event from queue */
		XEvent event;
		XNextEvent(display, &event);

		/* printf("event: %s\n", event_names[event.type]); */

		if (event.type == KeyPress) {
			int len = XLookupString(&event.xkey,
				key, sizeof(key) - 1, 0, 0);
			key[len] = 0;

			if (strlen(msg) > 50)
				msg[0] = 0;
		}

		if (event.type == ButtonPress) {

		}

		if (event.type == KeyPress || event.type == Expose) {
		/*	printf("keypress\n"); */
			int y = 20;
			char *pos;
			for (i = 0; i < lines; i++) {
			 if ((pos=strchr(contents[i], '\n')) != NULL)
			 	contents[i][strlen(contents[i])-1] = 0;
			if ((pos=strchr(contents[i], '\r')) != NULL)
				contents[i][strlen(contents[i])-1] = 0;
				XDrawString(display, main_window,
					gc, 10, y, contents[i], strlen(contents[i]));
				y = y+20;
			

/*
namelen = strlen(name); 
strncpy(copy, name, namelen - 3);
 add a final null terminator 
copy[namelen - 3] = 0;
*/
			}

        	}

		/* Close button */
		if (event.type == ClientMessage) {
			if (event.xclient.data.l[0] == wm_delete) {
				break;
			}
		}
    	}

	XCloseDisplay(display);
	return 0;
}
