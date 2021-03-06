/**
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <iostream>

#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>
#include <string.h>  /* String function definitions */
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */

int main(int argc, char* argv[])
{
	int i, n, fd = 0, arg_number = 1;

	struct termios tio;
	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;


	fd = open("/dev/ttyUSB0", O_RDWR | O_SYNC);

	tcsetattr(fd, TCSANOW, &tio);

	n = write(fd, "AT\r\n", 4);
	sleep(1);
	n = write(fd, "AT+CMGF=1\r\n", 11);
	sleep(1);
	n = write(fd, "AT+CSMS=1\r\n", 11);
	sleep(1);

	if(argc == 4)
	{
		puts("We have 4 arguments!\n");

		if(atoi(argv[arg_number++]) == 0)
			n = write(fd, "AT+CSMP=17,167,0,0\r\n", 20);
		else
			n = write(fd, "AT+CSMP=17,167,0,16\r\n", 21);
	} else
		n = write(fd, "AT+CSMP=17,167,0,0\r\n", 20);

	sleep(1);
	n = write(fd, "AT+CSCA=\"+4540390999\"\r\n", 23); // Telmore
	//n = write(fd, "AT+CSCA=\"+4531300000\"\r\n", 23); // 3
	sleep(1);
	char phone_number[22];

	for(i = 0; i < 22; i++) {
		phone_number[i] = 0;
	}

	sprintf(phone_number, "%s%s%s", "AT+CMGS=\"+45", argv[arg_number++], "\"\r");

	n = write(fd, phone_number, 22);
	sleep(1);
	char text_message[160];

	for (i = 0; i < 160; i++) {
		text_message[i] = 0;
	}
	sprintf(text_message, "%s%s", "MicroModem Alert: ", argv[arg_number++]);
	n = write(fd, text_message, sizeof(text_message));
	sleep(1);
	n = write(fd, "\x1A", 1);
	close(fd);

}
