#ifndef __MAPPING_H_

#define __MAPPING_H_

#define N 30

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define ERROR_SYS -1

struct PI{
	int int_ID;
	float flt_rate;
};

struct PO{
	int int_ID;
	int int_fanin;
};

struct Nodes{
	int int_ID;
	float flt_rate;
	int int_fanin[2];
};

struct DAG{
	int int_nbreNodes;
	int int_input;
	int int_output;
	struct PI* PIs;
	struct PO* POs;
	struct Nodes* Nodes;
};

struct LUT{
	int int_output;
	int int_nbreInputs;
	int* int_inputs;
};


// The following function load the DAG structure from a file with the name of the file in parameter
struct DAG loadingFile (char str_file[N]);

// Return the 2 parents of a node with its ID
int* parents(struct DAG DAG_input, int int_node);

// Test if a list contains a specific element with its ID
_Bool containsElt(int* tint_Inputnodes, int int_nbreInputnodes, int int_elt);

// Remove a element from a list
void removeElt(int* tint_list, int int_nbreList, int int_elt);

// Add a element in a list
void addElt(int* tint_list, int int_nbreList, int int_elt);

// Test if a list contains only the input of a DAG
_Bool finish (int* tint_Inputnodes, int int_nbreInputnodes, int* tint_inputs, int int_nbreInputs);

// Compute the optimization to try to reduce the depth and the rate of a DAG
struct LUT* optiMap(struct DAG DAG_input, int int_sizeLUT);

// Write the optimised solution in a file
void writingFile (char str_file[N], int int_nbreLUT, struct LUT* LUTS);

#endif