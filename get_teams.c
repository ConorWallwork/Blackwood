#include "blackwood.h"

/* ------------------------------------------------------------------- */
/* THIS SECTION IS RESPONSIBLE FOR PROCESSING BLACKWOOD MARATHON
** RESULTS FILES. IT ASSUMES THE FILE CONSISTS ONLY OF LINES OF THE FORM:

**    1   63 OP   00:38:58    2    2  00:47:36    6    2  00:10:23    1    2  00:31:15    2    1  00:33:12    1    1  02:41:24    1
**  (POS NO. CAT  RUN        LP   OP  PADDLE     LP   OP  SWIM       LP   OP  HORSE      LP   OP  CYCLE      LP   OP  OVERALL    OP)

** THE TEAM NUMBER AND INDIVIDUAL LEG TIMES FOR EACH TEAM ARE RECORDED
** IN A TEAM STRUCTURE AS DEFINED IN BLACKWOOD.H. THE VARIABLE 'TEAMS'
** POINTS TO A DYNAMICALLY ALLOCATED ARRAY OF THESE TEAMS.
** ------------------------------------------------------------------- */

/* POSITION OF IMPORTANT INFO  WHEN EACH LINE IS CONSIDERED AS WHITE SPACE SEPARATED TOKENS */

#define TEAM_POS_TOK	0 /* ONLY TEAMS WITH HORSES HAVE A POSITION, OTHERS HAVE 'NE' */
#define TEAM_NO_TOK     1
#define RUN_TOK         3
#define PADDLE_TOK      6
#define SWIM_TOK        9
#define HORSE_TOK       12
#define CYCLE_TOK       15
#define CYCLE_TOK_NH	17

/* POSITION OF IMPORTANT INFO WHEN EACH LINE IS CONSIDERED AS AN ARRAY OF CHARACTERS */

#define TEAM_NO_START	6
#define TEAM_NO_FINISH	8
#define RUN_START	15	/* RUN TIME STARTS AT 15TH CHARACTER OF EACH LINE */
#define TIME_LEN	8	/* TIMES ARE 8 CHARACTERS LONG */
#define NEXT_TIME	20	/* TIME FOR NEXT LEG IS 20 CHARACTERS DOWN THE ARRAY */

#define DNF 	"ff:ff:ff" /* TIME WE GIVE FOR RACERS THAT DNF OR DNS */
#define IRON	400

#define FORLEGS	for(int i = 0; i < NUMLEGS; i++)

/* DEBUGGING TOOL. PRINTS ALL INFO ON ONE LINE */
void print_team(TEAM team)
{
	printf("%i	", team.team_no);
	FORLEGS
	{
		printf("%s ", team.times[i]);
	}

	printf("%.2s ", team.filename+2);

	if( team.isNuts )
	{
		FORLEGS
                {
                	printf("%s %s\t", team.fnames[i], team.snames[i]);
                }
	}

	printf("\n");
}

void print_year(YEAR year)
{
	printf("%s\n", year.year);
	printf("%i teams\n", year.y_nteams);

	for(int i = 0; i < year.y_nteams; i++)
	{
		print_team(teams[i + year.base]);
	}
}


/* EACH LINE IN THE RESULTS FILE HAS THE FORM ABOVE.
** getTeams STORES THE RELEVANT INFORMATION FOR EACH
** IN A TEAM STRUCTURE AND CREATES AN ARRAY OF TEAMS */
YEAR ngetTeams(char* filename, char* teams_file)
{
//	CREATE AN OBJECT FOR THIS YEAR
	YEAR thisyear;
//	GET THE YEAR AS A STRING. EG "2018"
	asprintf(&thisyear.year, "%.4s", filename);

//	base HOLDS INDEX OF FIRST TEAM FROM THIS YEAR
	thisyear.base = nteams;

	TEAM_LIST* team_lists = getNuts(teams_file);

// PUT FILENAME ON HEAP
	filename = strdup( filename );

//	OPEN FILE AND CHECK FOR ERRORS

	asprintf(&filename, "%s/%s", RESULTS_DIRECTORY, filename);
	FILE* fp = fopen(filename, "r");
	if( fp == NULL )
	{
printf("%s\n", filename);
		perror("Opening teams file");
		exit(EXIT_FAILURE);
	}

//	RECORD NUMBER OF TEAMS IN THE FILE
	int ynteams = nteams;

	if( nteams == 0)
	{
//		INITIALISE TEAMS ARRAY FOR CALLS TO REALLOC
		teams = malloc(sizeof(TEAM));
	}
	else
	{
//		TEAMS HAS ALREADY BEEN MANUALLY ALLOCATED
//		NOTHING TO BE DONE HERE
	}

	char line[ BUFSIZ ];
	while( fgets(line, sizeof(line), fp) != NULL )
	{
//		MAKE SPACE FOR THIS TEAM
		teams = realloc(teams, ( nteams + 1 )*( sizeof(TEAM) ));

//		SO WE DON'T KEEP WRITING TEAMS[NTEAMS]
		TEAM* thisteam = &teams[ nteams ];

		*thisteam  = getTeam(line);
		thisteam->filename = thisyear.year;

		int num = thisteam->team_no;

//		IF ITS NOT AN IRON PERSON AND THEY'RE NUTS WE NEED NAMES OF TEAM MEMBERS.
//		BECAUSE THE TEAM LISTS ARE INDEXED BY TEAM NUMBER WE CAN EASILY ACCESS THEM
		if( num < IRON && team_lists[ num ].isNuts )
		{
			thisteam->isNuts = true;

			FORLEGS
			{
				thisteam->fnames[i] = team_lists[num].fnames[i];
				thisteam->snames[i] = team_lists[num].snames[i];
			}
		}
		nteams++;
	}
	fclose(fp);

	ynteams = nteams - ynteams;
	thisyear.y_nteams = ynteams;

	printf("%i teams in %s\n", ynteams, filename);

	return thisyear;

}

TEAM getTeam(char* line)
{
	TEAM thisteam;

	char team_no[ 5 ];
	sscanf(line, " %*s %s", team_no);
	thisteam.team_no = atoi(team_no);

	FORLEGS
	{
//		EACH TIME STARTS AT A MULTIPLE OF 20 AFTER RUN TIME
		int time_start = RUN_START + i*NEXT_TIME;
		thisteam.times[i] = getStat(line, time_start, time_start + TIME_LEN - 1, true);
	}
	thisteam.hasHorse = isdigit(thisteam.times[HORSE][0]);

	return thisteam;
}


/* RETURNS THE PART OF THE CHARACTER ARRAY line FROM
** start TO finish INCLUSIVELY AS A MANUALLY ALLOCATED
** STRING */
char* getStat(char* line, int start, int finish, bool isnumeric)
{
//	DNF OR DNS
	if( isnumeric && !isdigit( line[start] ) )
	{	return strdup( DNF );	}


	char this_stat[BUFSIZ];

	for(int i = start; i <= finish; i++)
	{
		this_stat[ i - start ] = line[ i ];
	}

	this_stat[ finish - start + 1 ] = '\0';
	return strdup( this_stat );
}

/* NO TEAMS FILE. NO NUTS */
void getTeams(char* filename)
{

//	TEAMS WILL POINT TO FILENAME, SO PUT IT ON HEAP
	filename = strdup( filename );

//	OPEN FILE AND CHECK FOR ERRORS
	FILE* fp = fopen(filename, "r");
	if( fp == NULL )
	{
		perror("Opening teams file");
		exit(EXIT_FAILURE);
	}

//	RECORD NUMBER OF TEAMS IN THE FILE
	int fnteams = nteams;

	if( nteams == 0)
	{
//		INITIALISE TEAMS ARRAY FOR CALLS TO REALLOC
		teams = malloc(sizeof(TEAM));
	}
	else
	{
//		TEAMS HAS ALREADY BEEN MANUALLY ALLOCATED
//		NOTHING TO BE DONE HERE
	}

	char line[ BUFSIZ ];
	while( fgets(line, sizeof(line), fp) != NULL )
	{
//		MAKE SPACE FOR THIS TEAM
		teams = realloc(teams, ( nteams + 1 )*( sizeof(TEAM) ));

		TEAM* thisteam = &teams[ nteams ];

		*thisteam  = getTeam(line);
		thisteam->filename = filename;

		nteams++;
	}
	fclose(fp);

	fnteams = nteams - fnteams;

	printf("%i teams in %s\n", fnteams, filename);
}
