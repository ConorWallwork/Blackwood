#include "blackwood.h"

#define MAX_TEAMS	250
#define TNAME_START	5
#define TNAME_MAX	30
	
#define RUNNER_START	54
#define	NAMELEN		21

char noname[BUFSIZ] = "NOT STATED";
char empty[1] = "";
	
void printTeamList(TEAM_LIST team)
{
	if( team.isNuts )
	{
		for( int i = 0; i < NUMLEGS; i++)
		{
			printf("%s %s\n", team.fnames[i], team.snames[i]);
		}
	}
}

TEAM_LIST* getNuts(char* teams_file)
{
	TEAM_LIST* team_lists = calloc(MAX_TEAMS, sizeof(TEAM_LIST));
	
	asprintf(&teams_file, "%s/%s", RESULTS_DIRECTORY, teams_file);
	FILE* fp = fopen(teams_file, "r");

	if( fp == NULL)
	{
		perror("Error opening teams file");
		exit(EXIT_FAILURE);
	}	
	
	int team_no;
	char line[ BUFSIZ ];
	while( fgets(line, sizeof(line), fp) != NULL )
	{
		team_no = atoi( getStat(line, 0, 3, false));	
		
//		TEAM LISTS ARE INDEXED BY TEAM NUMBER FOR SIMPLICITY. DO AWAY WITH INDIRECTION
		TEAM_LIST* thisteam = &team_lists[team_no];
		
		char* team_name = getStat(line, TNAME_START, TNAME_START + TNAME_MAX, false);
		
		if( ( thisteam->isNuts = isNuts(team_name)) )
		{
			char* this_racer;
			for(int i = 0; i < NUMLEGS; i++)
			{
				int name_start = RUNNER_START + i*NAMELEN;
				this_racer = getStat(line, name_start, name_start + NAMELEN - 1, false);
				
				thisteam->fnames[i] = strtok(this_racer, " ");
				thisteam->snames[i] = strtok(NULL, " ");
				
				if(thisteam->fnames[i] == NULL)
				{
//					THE UNLIKELY CASE THAT THE NAME IS BLANK
					thisteam->fnames[i] = noname;
					thisteam->snames[i] = empty;
				}
				
				
			}
		}
		
	}
	return team_lists;
}		
			
bool isNuts(char* team_name)
{
	if( strstr(team_name, "NUT") == NULL)
	{
		return false;
	}

	if( strstr(team_name, "MINUTE") != NULL)
	{
		return false;
	}	
	
	return true;	
}
