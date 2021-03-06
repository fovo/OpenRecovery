/*
 * Copyright (C) 2010 Skrilax_CZ
 * Open Recovery Interactive menu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include "imenu.h"

static interactive_menu_struct* interactive_menu;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		fprintf(stderr, "Too few arguments (requires at least 2 to have sense).\n");
		return 1;
	}
	
	int imenu_fd;	
		
	if((imenu_fd = open(INTERACTIVE_MENU_SHM, O_RDWR,
				             666)) < 0 ) 
  	return 1;
  	
  ftruncate(imenu_fd, sizeof(interactive_menu_struct));
	if ((interactive_menu = ((interactive_menu_struct*) mmap(0, sizeof(interactive_menu_struct), (PROT_READ | PROT_WRITE),
		                 MAP_SHARED, imenu_fd, 0))) == MAP_FAILED)
		return 1;
		
	strncpy(interactive_menu->header, argv[1], 49);
	
	int c;
	for (c = 2; c < argc && c < 21; c++)
		strncpy(interactive_menu->items[c-2], argv[c], 49);
	
	interactive_menu->items[c][0] = '\0'; 
	
	//reset the return trigger
	interactive_menu->out_trigger = 0;
	
	//trigger it
	interactive_menu->in_trigger = 1; 
	int response;
	while (!(response = interactive_menu->out_trigger))
		usleep(5000);
	
	printf("%d\n", response);
	close(imenu_fd);	
	return 0;
}

