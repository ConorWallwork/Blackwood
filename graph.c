#include "blackwood.h"

/* RETURNS A HISTROGRAM OBJECT WITH INFORMATION FROM THE TEAMS ARRAY 
** leg - THE LEG YOU WANT TO GRAPH TIMES OF
** slowest - THE SLOWEST TIME ( MAX TIME ) TO BE DISPLAYED ON THE GRAPH
** fastest - THE FASTEST TIME ( MIN TIME ) TO BE DISPLAYED ON THE GRAPH,
** interval - THE 'CLASS INTERVAL' OF THIS HISTOGRAM */
HISTOGRAM createHistogram(int leg, int slowest, int fastest, int interval)
{
	HISTOGRAM thisgraph;
	
	thisgraph.leg = leg;
	thisgraph.upper = slowest;
	thisgraph.lower = fastest;
	thisgraph.interval = interval;
	thisgraph.nIntervals = ( slowest - fastest ) / interval;
	
	INTERVAL* intervals = malloc( sizeof(INTERVAL) * thisgraph.nIntervals );
	
	for( int i = 0; i < thisgraph.nIntervals; i++)
	{
		intervals[i].lower = fastest + i*interval;
		intervals[i].nracers = 0;
		intervals[i].gotNuts = false;
		intervals[i].nNuts = 0;
		
		
	}
	thisgraph.bars = intervals;
	
	fillBars(&thisgraph);
	
	return thisgraph;
}

/* COUNT THE 'Y VALUE' FOR EACH INTERVAL/BAR ON THE
** HISTOGRAM BY TRAVERSING THE TEAMS ARRAY */
void fillBars( HISTOGRAM* graph)
{
	TEAM* fastest = &teams[0];
	int fastest_time = 7200; // TWO HOURS. SAFE BET
	
//	GO THROUGH EACH TEAM. FOR LEG THAT WE WANT, INCREMENT THE
//	COUNTER FOR THE INTERVAL THAT THAT TIME FALLS IN
	for( int i = 0; i < nteams; i++)
	{
		char* thistime = teams[i].times[ graph->leg ];

//		OPEN THIS LINE IF WE ONLY WANT TO COUNT TEAMS WITH HORSES
		//if( !teams[i].hasHorse ) { continue; }
		
//		THIS IS AN ASTERISK'ED TIME. SKIP IT
		if( !isdigit(thistime[0]) ) { continue; }
		
//		CONVERT THIS TIME TO SECONDS
		int seconds = timecode_to_seconds(thistime);
		
		if( seconds < fastest_time )
		{
			fastest = &teams[i];
			fastest_time = seconds;
		}
		
//		FIND THE INTERVAL IT LIES IN
		int interval_no = ( seconds - graph->lower) / graph->interval;

//		DO AWAY WITH INDIRECTION		
		INTERVAL* thisinterval = &graph->bars[interval_no];
		thisinterval->nracers++;
	
		if( teams[i].isNuts )
		{
//print_team(teams[i]);
			thisinterval->gotNuts = true;
			if( thisinterval->nNuts == 0)
			{
//				MANUALLY ALLOCATED SPACE FOR FUTURE CALLS TO REALLOC
				thisinterval->nuts = malloc(sizeof(TEAM*));
			}
			
//			MAKE WAY FOR THIS TEAM OF NUTS
			thisinterval->nuts = realloc( thisinterval->nuts, (thisinterval->nNuts+1)*sizeof(TEAM) );

//			ADD THIS TEAM TO THE INTERVAL'S ARRAY OF NUT TEAMS
			thisinterval->nuts[thisinterval->nNuts] = &teams[i];
			thisinterval->nNuts++;
		}
			
	}
	
	graph->fastest = fastest;

}		

void print_graph(HISTOGRAM* graph)
{
	int totalracers = 0;
	int totaltime = 0;
	
	switch( graph->leg )
	{
		case RUN:
		{
			printf("	DISTRIBUTION OF RUN TIMES\n\n");
			break;
		}
		case PADDLE:
		{
			printf("	DISTRIBUTION OF PADDLE TIMES\n\n");
			break;
		}
		case SWIM:
		{
			printf("	DISTRIBUTION OF SWIM TIMES\n\n");
			break;
		}
		case HORSE:
		{
			printf("	DISTRIBUTION OF HORSE TIMES\n\n");
			break;
		}
		case CYCLE:
		{
			printf("	DISTRIBUTION OF CYCLE TIMES\n\n");
			break;
		}
	}
	
	printf("%i teams\n", nteams);
	printf("fastest: %i	%s	%s\n", graph->fastest->team_no, graph->fastest->times[graph->leg], graph->fastest->filename);
		
	//printf("             5    10\n");
	for( int i = 0; i < graph->nIntervals; i++)
	{
//		DO AWAY WITH INDIRECTION
		INTERVAL* thisinterval = &graph->bars[i];
	

//		PRINT THE TIME FOR THIS INTERVAL
		printf("%s ", seconds_to_timecode(thisinterval->lower) );
		
		int nracers = thisinterval->nracers;
		totalracers += nracers;
		totaltime += nracers*thisinterval->lower;
		
		for( int c = 0; c < nracers; c++)
		{
			printf("*");
		}
		
		if( thisinterval->gotNuts )
		{
			printf("    ");
			for( int i = 0; i < thisinterval->nNuts; i++)
			{
				char racer[BUFSIZ];
				sprintf(racer, "%.1s %s %s %.2s\t", thisinterval->nuts[i]->fnames[graph->leg],
						          thisinterval->nuts[i]->snames[graph->leg],
						          thisinterval->nuts[i]->times[graph->leg],
						          thisinterval->nuts[i]->filename+2);
				printf("%.30s", racer);
			}
		} 
		
		printf("\n");
	}
	printf("%i racers\n", totalracers);
	//printf("average time %s\n", seconds_to_timecode( totaltime / totalracers ) );
}	
	
	
