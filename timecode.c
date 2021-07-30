#include "blackwood.h"

#define UNIT_SEPARATOR ":"

/* TAKES A TIMECODE OF THE FORM:
** HH:MM:SS 
** AND CONVERTS IT TO TOTAL SECONDS */
int timecode_to_seconds( char* timecode )
{
	if( !isdigit(timecode[0] ) )
	{	return 7200;	}
	
	
	int total_seconds = 0;

//	SEPARATE TIMECODE INTO HOURS, MINS, SECS
	char* token = strtok(strdup(timecode), UNIT_SEPARATOR);

	int hours = atoi(token);
	total_seconds += 3600*hours;	
	
	token = strtok(NULL, UNIT_SEPARATOR);
	int minutes = atoi(token);
	total_seconds += 60*minutes;
	
	token = strtok(NULL, UNIT_SEPARATOR);
	int seconds = atoi(token);
	total_seconds += seconds;
	
	return total_seconds;
}

/* TAKES TOTAL SECONDS AND CONVERTS IT TO
** A TIMECODE OF THE FORM DEFINED ABOVE */
char* seconds_to_timecode( int seconds )
{
	char timecode[BUFSIZ];

	int t_hours = seconds / 3600;
	int t_minutes = ( seconds % 3600 ) / 60;
	int t_seconds = ( ( seconds % 3600 ) % 60 );
	
	snprintf(timecode, sizeof(timecode), "%02i:%02i:%02i", t_hours, t_minutes, t_seconds);	

	return strdup(timecode);
}
	
	
