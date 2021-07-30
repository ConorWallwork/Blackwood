#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/syslimits.h>

/* ----------------------- PARSING A FILE OF TEAM REULTS  ----------------------- */

/* LEG TIMES ARE STORED AS A STRING ARRAY. THIS IS THE INDEX */
#define NUMLEGS	5
#define RUN    	0
#define PADDLE  1
#define SWIM    2
#define HORSE   3
#define CYCLE   4

typedef struct 
{
	int team_no;
	char* times[ NUMLEGS ];
	bool hasHorse;
	char* filename;	/* the file this team came from */
	bool isNuts;
	
//	FIRST AND LAST NAMES OF COMPETITORS. ONLY IF THEY'RE NUTS */
	char* fnames[NUMLEGS]; 
	char* snames[NUMLEGS];
	
	
} TEAM;

// WE MAY ACCESS TEAMS BY YEAR
typedef struct
{
	int base; /* index of first team for this year in teams array */
	int y_nteams; /* number of teams in this year */
	char* year;
} YEAR;


extern void print_team(TEAM team); /* Debugging tool */

#define RESULTS_DIRECTORY       "RESULTS_FILES"
extern YEAR ngetTeams(char* times_file, char* teams_file); /* Get teams and record nuts */
extern void getTeams(char* times_file); /* Get teams, don't worry about nuts */

extern TEAM getTeam(char* line);
extern char* getStat(char* line, int start, int finish, bool isnumeric);

extern TEAM* 	teams;
extern int 	nteams;

	
extern YEAR* years;
extern int nyears;
	
extern void print_year(YEAR);


/* ----------------------- TIMECODE CONVERSIONS ----------------------- */

extern int timecode_to_seconds(char* timecode);
extern char* seconds_to_timecode(int seconds);

/* ----------------------- CREATING THE HISTOGRAMS ----------------------- */


typedef struct
{
        int lower; 	// LOWER BOUND FOR THIS INTERVAL, IN SECONDS
        int nracers;	// NUMBER OF COMPETITORS IN THIS INTERVAL
	bool gotNuts;
	
	TEAM** nuts;	// ARRAY OF TEAM POINTERS, NOT A MATRIX
	int nNuts;
} INTERVAL;

typedef struct
{
	TEAM* fastest; /* pointer to team with fastest competitor for this graph */
        int leg;
        int lower;	/* lowest interval, in seconds */
        int upper;	/* highest interval, in seconds */
        int interval;	/* length of each interval */
        int nIntervals;
        INTERVAL* bars;
} HISTOGRAM;

extern HISTOGRAM createHistogram(int leg, int lower, int upper, int interval);
extern void	 fillBars( HISTOGRAM* graph);	

extern void	print_graph(HISTOGRAM* graph);

/* ----------------- GETTING NUTS FROM A TEAM LIST FILE  -------------------- */

typedef struct
{
	bool isNuts;
	char* fnames[NUMLEGS];
	char* snames[NUMLEGS];
} TEAM_LIST;

extern TEAM_LIST* getNuts(char* teams_file);
extern bool isNuts(char* team_name);
	




