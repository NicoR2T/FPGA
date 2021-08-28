/*!
\file main.c
\author Nicolas Tirel
\version 1.0
\date 01/03/2020
\brief 
*/

/* Inclusion des librairies */
#include "mapping.h"

/*!
\fn int main (int argc, char** argv)
\author Nicolas Tirel
\version 1.0
\date 01/03/2020
\brief 
\param argc number of arguments
\param argv arguments
\return 0 if all is correct
*/

int main (int argc, char** argv) {
	char* endptr;
	int int_LUTSize = strtol(argv[2], &endptr, 10);
	struct LUT* LUTs;
	struct DAG DAG;
	clock_t start = clock();
	clock_t end;

	if (int_LUTSize <= 2) {
		fprintf (stderr, "Size of LUT is too low : %s\n", strerror(errno));
	}

	if (argc==4) {
		DAG = loadingFile(argv[1]);

		if (int_LUTSize > DAG.int_input) {
			fprintf(stderr, "Size of LUT can't be higher than the number of inputs : %s\n", strerror(errno));
		}
		else {
			LUTs = optiMap(DAG, int_LUTSize);

			writingFile(argv[3], DAG.int_nbreNodes-DAG.int_input-DAG.int_output, LUTs);
			free(DAG.PIs);
			free(DAG.POs);
			free(DAG.Nodes);
			free(LUTs);
		}
	}
	else {
		fprintf (stderr, "Number of argument is insufficient : %s\n", strerror(errno));
	}

	end = clock();
	printf("Running time: %f ms\n", (double)(end-start) / CLOCKS_PER_SEC * 1000);

	return (0);	
}

