/*
 * cload - curses implementation of 'xload'
 * Copyright 2011 Duane Wessels <wessels@packet-pushers.com>
 */

/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <err.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <curses.h>

#define NHIST 400
unsigned int Interval = 3;
double History[NHIST];
WINDOW *w;

void
display()
{
	double MaxLoad = 1.0;
	int x, y;
	int WIDTH;
	int HEIGHT;
	if (!w)
		w = initscr();
	getmaxyx(w, HEIGHT, WIDTH);
	if (WIDTH > NHIST)
		WIDTH = NHIST;
	for (x = 0; x < WIDTH; x++)
		if (MaxLoad < History[x])
			MaxLoad = ceil(History[x]);
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			move(HEIGHT-1-y, WIDTH-1-x);
			((double) HEIGHT * History[x] / MaxLoad > y) ?  standout() : standend();
			addch(' ');
		}
	}
	/* draw a line at the top */
	standend();
	for (x = 0; x < WIDTH; x++) {
		move(0, x);
		addch('_');
	}
	move(0, WIDTH - 5);
	printw(" %d ", (int) MaxLoad);
	curs_set(0);
	refresh();
}

int
main(int argc, char *argv[])
{
	memset(History, 0, sizeof(History));
	if (argc > 1)
		Interval = atoi(argv[1]);
	for (;;) {
		int i;
		for (i=NHIST-1; i > 0; i--)
			History[i] = History[i-1];
		if (getloadavg(&History[0], 1) < 0)
			err(1, "getloadavg");
		display();
		sleep(Interval);
	}
	endwin();
	return 0;
}
