#include "blackwood.h"

#define FILENAME "test"
#define TIMECODE teams[1].horse_time /* Debugging tool for timecode/second conversions */

/* ------------ SWIM DEFAULTS ------------- */
#define S_SLOWEST 	1800 	// 30 minutes
#define S_FASTEST	480 	// 8 minutes
#define S_INTERVAL	10

/* ------------ CYCLE DEFAULTS------------- */
#define C_SLOWEST 	5400 	// 1h30 minutes
#define C_FASTEST	1500	// 25 minutes
#define C_INTERVAL	30

/* ------------- RUN DEFAULTS -------------- */
#define R_INTERVAL 	30
#define R_FASTEST	2100 // 35 minutes
#define R_SLOWEST	5900

/* ------------- PADDLE DEFAULTS -------------- */
#define P_INTERVAL	40
#define P_FASTEST	2100
#define P_SLOWEST	7800

/* ------------- HORSE DEFAULTS -------------- */
#define H_FASTEST	1200
#define H_SLOWEST	5400
#define H_INTERVAL	30

/* --------- FILES ---------------- */

#define RESULTS_FILE		"_times"
#define TEAMS_FILE		"_teams"
#define OLDEST_TEAMS		2011
#define NEWEST_TEAMS		2017
#define OLDEST_TIMES		2007


int main(int argc, char* argv[])
{
	// if( argc != 3)
	// {
	// 	printf("Enter a file name\n");
	// 	exit(EXIT_FAILURE);
	// }
	// char* timesfile = argv[1];
	// char* teamsfile = argv[2];

	// years = malloc(sizeof(YEAR));
	// //GENERATE TEAMS FROM NAMED RESULTS/TEAMS FILE
	// years[nyears] = ngetTeams( timesfile, teamsfile );
	// nyears++;

	//GENERATE TEAMS FROM BUILT-IN RESULTS/TEAMS FILES
	for( int i = OLDEST_TEAMS; i <= NEWEST_TEAMS; i++)
	{
		years = realloc(years, sizeof(YEAR)*(nyears + 1) );

		char thistimesfile[PATH_MAX];
		char thisteamsfile[PATH_MAX];
		sprintf(thistimesfile, "%i%s", i, RESULTS_FILE);
		sprintf(thisteamsfile, "%i%s", i, TEAMS_FILE);
//printf("%s	%s\n", thisteamsfile, thistimesfile);
		years[nyears] = ngetTeams(thistimesfile, thisteamsfile);

		//print_year(years[nyears]);

		nyears++;
	}

	//GENERATE TEAMS FROM REST OF THE RESULTS FILES
	for( int i = OLDEST_TIMES; i < OLDEST_TEAMS; i++)
	{
		char thistimesfile[PATH_MAX];
		sprintf(thistimesfile, "%s/%i%s", RESULTS_DIRECTORY, i, RESULTS_FILE);
		getTeams(thistimesfile);
	}

	/*for( int i = 0; i < nteams; i++)
	{
		print_team( teams[i] );
	}*/

	HISTOGRAM run = createHistogram( RUN, R_SLOWEST, R_FASTEST, R_INTERVAL);
	print_graph(&run);

	HISTOGRAM paddle = createHistogram( PADDLE, P_SLOWEST, P_FASTEST, P_INTERVAL);
	print_graph(&paddle);

	HISTOGRAM swim = createHistogram( SWIM, S_SLOWEST, S_FASTEST, S_INTERVAL);
	print_graph(&swim);

	HISTOGRAM horse = createHistogram( HORSE, H_SLOWEST, H_FASTEST, H_INTERVAL);
	print_graph(&horse);

	HISTOGRAM cycle = createHistogram( CYCLE, C_SLOWEST, C_FASTEST, C_INTERVAL );
	print_graph(&cycle);


	return EXIT_SUCCESS;
}
