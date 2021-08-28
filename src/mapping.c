#include "mapping.h"

struct DAG loadingFile(char str_file[N]) {
	FILE* pfil_fic;
	int int_return;
	char str_aag[4];
	char str_line[50];
	char* str_ptr;
	int i;
	struct DAG DAG;
	const char str_separator[2] = " ";

	pfil_fic = fopen (str_file, "r");
	if (pfil_fic == NULL) {
		fprintf(stderr, "The file cannot be load : %s\n", strerror (errno));
	}

	if (fgets(str_line, 50, pfil_fic) != NULL) {
		strcpy(str_aag, strtok(str_line, str_separator));
		DAG.int_nbreNodes = strtol(strtok(NULL, str_separator), &str_ptr, 10);
		DAG.int_input = strtol(strtok(NULL, str_separator), &str_ptr, 10);
		DAG.int_output = strtol(strtok(NULL, str_separator), &str_ptr, 10);
	}
	else {
		printf("Ouielse\n");
		fprintf(stderr, "Error when reading of the file: %s\n", strerror (errno));
		int_return = fclose (pfil_fic);
		if (int_return == EOF) {
			fprintf (stderr, "Error when closing the file: %s\n", strerror (errno));
		}
	}


	DAG.PIs = malloc((DAG.int_input)*sizeof(struct PI));

	for (i=0; i<(DAG.int_input); i++){
		if (fgets(str_line, 50, pfil_fic) != NULL) {
			DAG.PIs[i].int_ID = strtol(strtok(str_line, str_separator), &str_ptr, 10);
			DAG.PIs[i].flt_rate = strtold(strtok(NULL, str_separator), &str_ptr);
		}
		else {
			fprintf(stderr, "Error when reading of the file: %s\n", strerror (errno));
			int_return = fclose (pfil_fic);
			if (int_return == EOF) {
				fprintf (stderr, "Error when closing the file: %s\n", strerror (errno));
			}
		}
	}

	DAG.POs = malloc((DAG.int_output)*sizeof(struct PO));

	for (i=0; i<(DAG.int_output); i++){
		if (fgets(str_line, 50, pfil_fic) != NULL) {
			DAG.POs[i].int_ID = strtol(strtok(str_line, str_separator), &str_ptr, 10);
			DAG.POs[i].int_fanin = strtol(strtok(NULL, str_separator), &str_ptr, 10);
		}
		else {
			fprintf(stderr, "Error when reading of the file: %s\n", strerror (errno));
			int_return = fclose (pfil_fic);
			if (int_return == EOF) {
				fprintf (stderr, "Error when closing the file: %s\n", strerror (errno));
			}
		}
	}

	DAG.Nodes = malloc((DAG.int_nbreNodes)*sizeof(struct Nodes));

	for (i=0; i<(DAG.int_nbreNodes-DAG.int_input-DAG.int_output); i++){
		if (fgets(str_line, 50, pfil_fic) != NULL) {
			DAG.Nodes[i].int_ID = strtol(strtok(str_line, str_separator), &str_ptr, 10);
			DAG.Nodes[i].flt_rate = strtold(strtok(NULL, str_separator), &str_ptr);
			DAG.Nodes[i].int_fanin[0] = strtol(strtok(NULL, str_separator), &str_ptr, 10);
			DAG.Nodes[i].int_fanin[1] = strtol(strtok(NULL, str_separator), &str_ptr, 10);
		}
		else {
			fprintf(stderr, "Error when reading of the file: %s\n", strerror (errno));
			int_return = fclose (pfil_fic);
			if (int_return == EOF) {
				fprintf (stderr, "Error when closing the file: %s\n", strerror (errno));
			}
		}
	}

	int_return = fclose (pfil_fic);
	if (int_return == EOF) {
		fprintf (stderr, "Error when closing the file: %s\n", strerror(errno));
	}

	printf("Reading successful!\n");
	return (DAG);

}

int* parents (struct DAG DAG_input, int int_node) {
	int i;

	i = 0;
	while (i<DAG_input.int_nbreNodes && DAG_input.Nodes[i].int_ID!=int_node) {
		i++;
	}

	return DAG_input.Nodes[i].int_fanin;
}

_Bool containsElt(int* tint_InputNodes, int int_nbreInputNodes, int int_elt) {
	int i;

	i = 0;
	while (i<int_nbreInputNodes && tint_InputNodes[i]!=int_elt) {
		i++;
	}

	return (i!=int_nbreInputNodes);
}

void removeElt(int* tint_list, int int_nbreList, int int_elt) {
	int i;
	int j;

	i=0;
	j=i;
	while (i<int_nbreList) {
		if (tint_list[i] == int_elt) {
			j++;
		}
		tint_list[i] = tint_list[j];
		i++;
		j++;
	}

	tint_list = realloc(tint_list, int_nbreList-1);
}

void addElt(int* tint_list, int int_nbreList, int int_elt) {
	tint_list = realloc(tint_list, (int_nbreList+1)*sizeof(int));
	tint_list[int_nbreList] = int_elt;
}

_Bool finish (int* tint_InputNodes, int int_nbreInputNodes, int* tint_inputs, int int_nbreInputs) {
	int i;

	i = 0;
	while (i<int_nbreInputNodes && (containsElt(tint_inputs, int_nbreInputs, tint_InputNodes[i]))) {
		i++;
	}
	return (i==int_nbreInputNodes);
}

struct LUT* optiMap(struct DAG DAG_input, int int_sizeLUT) {
	struct LUT* LUTS;
	struct LUT LUT;
	int int_nbreLUTS = 0;
	int i;

	LUTS = malloc(DAG_input.int_nbreNodes*sizeof(struct LUT));

	for (i=0; i<DAG_input.int_nbreNodes-DAG_input.int_input-DAG_input.int_output; i++) {
		LUT.int_output = DAG_input.Nodes[i].int_ID;
		LUT.int_nbreInputs = 2;
		LUT.int_inputs = DAG_input.Nodes[i].int_fanin;
		LUTS[int_nbreLUTS] = LUT;
		int_nbreLUTS++;
	}

	return LUTS;


}

void writingFile(char str_file[N], int int_nbreLUT, struct LUT* LUTS) {
	FILE* pfil_fic;
	int int_return;
	char* str_string;
	char str_id[10];
	int i;
	int j;

	pfil_fic = fopen (str_file, "w+");
	if (pfil_fic == NULL) {
		fprintf (stderr, "File opening error : %s\n", strerror (errno));
	}
	str_string = malloc((1+int_nbreLUT)*50*sizeof(char));
	sprintf(str_string, "%s", "");
	for (i=0; i<int_nbreLUT; i++) {
		sprintf(str_id, "%d", LUTS[i].int_output);
		strcat(str_string, str_id);
		for (j=0; j<LUTS[i].int_nbreInputs; j++) {
			sprintf(str_id, " %d", LUTS[i].int_inputs[j]);
			strcat(str_string, str_id);
		}
		strcat(str_string, "\n");
	}
	int_return = fwrite(str_string, sizeof (char),  strlen(str_string), pfil_fic);
	if (int_return != strlen(str_string)) {
		fprintf (stderr, "File writing error : %s\n", strerror (errno));
		int_return = fclose (pfil_fic);
		if (int_return == EOF) {
			fprintf (stderr, "Closing file error : %s\n", strerror (errno));
		}	
	}

	int_return = fclose (pfil_fic);
	if (int_return == EOF) {
		fprintf (stderr, "Closing file error : %s\n", strerror (errno));
	}

	printf("Writing successful!\n");
}

//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************

	// FIRST IMPLEMENTATION TRY

	/*
	int i;
	int j;
	int* tint_InputNodes;
	int int_nbreInputNodes = 0;
	int* tint_OK;
	int int_nbreOK = 0;
	int* tint_NOK;
	int int_nbreNOK = 0;
	int* tint_toTest;
	int int_nbretoTest = 0;
	int tint_inputs[DAG_input.int_input];
	int tint_parents[2];

	int node_OK;
	int node_NOK;

	struct LUT* LUTs;
	struct LUT LUT;
	int int_nbreLuts = 0;

	for (i=0; i<DAG_input.int_input; i++) {
		tint_inputs[i] = DAG_input.PIs[i].int_ID;
	}

	tint_toTest = malloc(int_nbretoTest*sizeof(int));
	for (i=0; i<DAG_input.int_output; i++) {
		addElt(tint_toTest, int_nbretoTest, DAG_input.POs[i].int_fanin);
		int_nbretoTest++;
	}

	tint_InputNodes = malloc(int_nbreInputNodes*sizeof(int));
	tint_OK = malloc(int_nbreOK*sizeof(int));
	tint_NOK = malloc(int_nbreNOK*sizeof(int));

	LUTs = malloc(DAG_input.int_nbreNodes*sizeof(struct LUT));

	while (int_nbretoTest!=0) {
		int_nbreInputNodes = 0;
		tint_InputNodes = malloc(int_nbreInputNodes*sizeof(int));
		addElt(tint_InputNodes, int_nbreInputNodes, tint_toTest[0]);
		int_nbreInputNodes++;
		while (!(finish(tint_InputNodes, int_nbreInputNodes, tint_inputs, DAG_input.int_input))) {
			for (j=0; j<int_nbreInputNodes; j++) {
				if (!(containsElt(tint_inputs, DAG_input.int_input, tint_InputNodes[j]))) {
					tint_parents[0] = parents(DAG_input, tint_InputNodes[j])[0];
					tint_parents[1] = parents(DAG_input, tint_InputNodes[j])[1];
					if (!containsElt(tint_InputNodes, int_nbreInputNodes, tint_parents[0])) {
						tint_InputNodes[j] = tint_parents[0];
						if (!containsElt(tint_InputNodes, int_nbreInputNodes, tint_parents[1])) {
							addElt(tint_InputNodes, int_nbreInputNodes, tint_parents[1]);
							int_nbreInputNodes++;
						}
					}
					else {
						if (containsElt(tint_InputNodes, int_nbreInputNodes, tint_parents[1])) {
							removeElt(tint_InputNodes, int_nbreInputNodes, tint_InputNodes[j]);
							int_nbreInputNodes--;
						}
						else {
							tint_InputNodes[j] = tint_parents[1];
						}
					}
				j--;
				}
			} 
		}

		if (int_nbreInputNodes<=int_sizeLUT) {
			LUT.int_output = tint_toTest[0];
			LUT.int_nbreInputs = int_nbreInputNodes;
			LUT.int_inputs = tint_InputNodes;
			LUTs[int_nbreLuts] = LUT;
			int_nbreLuts++;
			addElt(tint_OK, int_nbreOK, tint_toTest[0]);
			int_nbreOK++;
			removeElt(tint_toTest, int_nbretoTest, tint_toTest[0]);
			int_nbretoTest--;
		}

		else {
			addElt(tint_NOK, int_nbreNOK, tint_toTest[0]);
			int_nbreNOK++;
			tint_parents[0] = parents(DAG_input, tint_toTest[0])[0];
			tint_parents[1] = parents(DAG_input, tint_toTest[0])[1];
			if (!containsElt(tint_toTest, int_nbretoTest, tint_parents[0]) && !containsElt(tint_InputNodes, int_nbreInputNodes, tint_parents[0])) {
				addElt(tint_toTest, int_nbretoTest, tint_parents[0]);
				int_nbretoTest++;
			}
			if (!containsElt(tint_toTest, int_nbretoTest, tint_parents[1]) && !containsElt(tint_InputNodes, int_nbreInputNodes, tint_parents[1])) {
				addElt(tint_toTest, int_nbretoTest, tint_parents[1]);
				int_nbretoTest++;
			}
			removeElt(tint_toTest, int_nbretoTest, tint_toTest[0]);
			int_nbretoTest--;
		}
	}

	while (int_nbreNOK!=0) {
		tint_parents[0] = parents(DAG_input, tint_NOK[0])[0];
		tint_parents[1] = parents(DAG_input, tint_NOK[0])[1];
		if ((containsElt(tint_OK, int_nbreOK, tint_parents[0]) && containsElt(tint_OK, int_nbreOK, tint_parents[1]))
			|| (!containsElt(tint_OK, int_nbreOK, tint_parents[0]) && !containsElt(tint_OK, int_nbreOK, tint_parents[1]))) {
			removeElt(tint_NOK, int_nbreNOK, tint_NOK[0]);
			int_nbreNOK--;
		}
		else {
			if (containsElt(tint_OK, int_nbreOK, tint_parents[0])) {
				node_OK = tint_parents[0];
				node_NOK = tint_parents[1];
			}
			else {
				node_NOK = tint_parents[0];
				node_OK = tint_parents[1];
			}
			int_nbreInputNodes = 0;
			tint_InputNodes = malloc(int_nbreInputNodes*sizeof(int));
			addElt(tint_InputNodes, int_nbreInputNodes, node_OK);
			int_nbreInputNodes++;
			addElt(tint_InputNodes, int_nbreInputNodes, parents(DAG_input, node_NOK)[0]);
			int_nbreInputNodes++;
			addElt(tint_InputNodes, int_nbreInputNodes, parents(DAG_input, node_NOK)[1]);
			LUT.int_output = tint_NOK[0];
			LUT.int_nbreInputs = int_sizeLUT;
			LUT.int_inputs = tint_InputNodes;
			LUTs[int_nbreLuts] = LUT;
			int_nbreLuts++;
			removeElt(tint_NOK, int_nbreNOK, tint_NOK[0]);
			int_nbreNOK--;
		}
	}

	printf("------%d\n", int_nbreLuts);
	for (i=0; i<int_nbreLuts; i++) {
		printf("%d, %d: %d %d %d\n", LUTs[i].int_output, LUTs[i].int_nbreInputs, LUTs[i].int_inputs[0], LUTs[i].int_inputs[1], LUTs[i].int_inputs[2]);
	}

	free(tint_OK);
	free(tint_NOK);
	//free(tint_InputNodes);
	//free(tint_toTest);
	return LUTs;

	*/

//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************


	// SECOND IMPLEMENTATION TRY

	/*
	int i;
	int j;
	int k;
	int x;
	int tint_listInput[DAG_input.int_input];
	int tint_listOutput[DAG_input.int_output];
	int* tint_nodesInput;
	int int_nbreNodesInput = 0;
	int* tint_nodesNInput;
	int int_nbreNodesNInput = 0;
	int p1 = 0;
	int p2 = 0;
	int impair = 0;

	int* tint_mergeNodes;
	int int_nbreMergeNodes = 0;
	int* tint_mergeInputs;
	int int_nbreMergeInputs = 0;

	tint_mergeInputs = malloc(int_nbreMergeInputs*sizeof(int));
	tint_mergeNodes = malloc(int_nbreMergeNodes*sizeof(int));

	struct LUT* LUTS;
	struct LUT LUT;
	int int_nbreLUTS = 0;

	LUTS = malloc(DAG_input.int_nbreNodes*sizeof(struct LUT));

	tint_nodesInput = malloc(int_nbreNodesInput*sizeof(int));
	tint_nodesNInput = malloc(int_nbreNodesNInput*sizeof(int));

	int int_nbreInputLUT = 0;
	int* tint_inputLUT = malloc(int_nbreInputLUT*sizeof(int));

	int int_nbreL = 0;
	int* tint_bigL = malloc(int_nbreL*sizeof(int));

	int int_duplicateLUT = 0;
	int* tint_listLUT = malloc(int_duplicateLUT*sizeof(int));


	for (i=0; i<DAG_input.int_input; i++) {
		tint_listInput[i] = DAG_input.PIs[i].int_ID;
		printf("Input: %d\n", tint_listInput[i]);
	}

	for (i=0; i<DAG_input.int_output; i++) {
		tint_listOutput[i] = DAG_input.POs[i].int_fanin;
		printf("Output: %d\n", tint_listOutput[i]);
	}

	for (i=0; i<DAG_input.int_output; i++) {
		
		p1 = parents(DAG_input, tint_listOutput[i])[0];
		p2 = parents(DAG_input, tint_listOutput[i])[1];
		x=0;
		addElt(tint_nodesNInput, int_nbreNodesNInput, p1);
		int_nbreNodesNInput++;
		addElt(tint_nodesNInput, int_nbreNodesNInput, p2);
		int_nbreNodesNInput++;
		for (j=0; j<int_nbreNodesNInput; j++) {
			p1 = parents(DAG_input, tint_nodesNInput[j])[0];
			p2 = parents(DAG_input, tint_nodesNInput[j])[1];
			if (containsElt(tint_listInput, DAG_input.int_input, p1) && !containsElt(tint_nodesInput, int_nbreNodesInput, p1)) {
				addElt(tint_nodesInput, int_nbreNodesInput, p1);
				int_nbreNodesInput++;
			}
			else if(!containsElt(tint_listInput, DAG_input.int_input, p1) && !containsElt(tint_nodesNInput, int_nbreNodesNInput, p1)) {
				addElt(tint_nodesNInput, int_nbreNodesNInput, p1);
				int_nbreNodesNInput++;
			}
			if (containsElt(tint_listInput, DAG_input.int_input, p2) && !containsElt(tint_nodesInput, int_nbreNodesInput, p2)) {
				addElt(tint_nodesInput, int_nbreNodesInput, p2);
				int_nbreNodesInput++;
			}
			else if(!containsElt(tint_listInput, DAG_input.int_input, p2) && !containsElt(tint_nodesNInput, int_nbreNodesNInput, p2)) {
				addElt(tint_nodesNInput, int_nbreNodesNInput, p2);
				int_nbreNodesNInput++;
			}
		}
		
		for (j=0; j<int_nbreNodesNInput; j++) {
			printf("%d\n", tint_nodesNInput[j]);
		}
		printf("\n");


		for (j=0; j<int_nbreNodesInput; j++) {
			printf("%d\n", tint_nodesInput[j]);
		}
		printf("\n");

		if (int_nbreNodesInput<=int_sizeLUT) {
			LUT.int_output = tint_listOutput[i];
			LUT.int_nbreInputs = int_nbreNodesInput;
			LUT.int_inputs = tint_nodesInput;
			LUTS[int_nbreLUTS] = LUT;
			int_nbreLUTS++;
		}
		else {
			x = int_nbreNodesInput-int_sizeLUT;
			while(x>0){
				for (j=0; j<int_nbreNodesNInput; j++) {
					p1 = parents(DAG_input, tint_nodesNInput[j])[0];
					p2 = parents(DAG_input, tint_nodesNInput[j])[1];
					if ((containsElt(tint_listInput, DAG_input.int_input, p1) || containsElt(tint_listInput, DAG_input.int_input, p2)) && !containsElt(tint_mergeInputs, int_nbreMergeInputs, p1) && !containsElt(tint_mergeInputs, int_nbreMergeInputs, p2)) {
						addElt(tint_mergeInputs, int_nbreMergeInputs, p1);
						int_nbreMergeInputs++;
						addElt(tint_mergeInputs, int_nbreMergeInputs, p2);
						int_nbreMergeInputs++;
						printf("1 Inputs: %d, merge inputs added: %d %d\n", int_nbreMergeInputs, p1, p2);
						printf("Taille: %d\n", int_nbreMergeNodes);
						addElt(tint_mergeNodes, int_nbreMergeNodes, tint_nodesNInput[j]);
						int_nbreMergeNodes++;
						x--;
						printf("x moved: %d\n", x);
					}
					else if(containsElt(tint_mergeInputs, int_nbreMergeInputs, p1) && containsElt(tint_mergeInputs, int_nbreMergeInputs, p2)) {
						addElt(tint_mergeInputs, int_nbreMergeInputs, tint_nodesNInput[j]);
						int_nbreMergeInputs++;
						removeElt(tint_nodesNInput, int_nbreNodesNInput, tint_nodesNInput[j]);
						int_nbreNodesNInput--;
						j--;
						printf("2 Inputs: %d, merge inputs added: %d\n", int_nbreMergeInputs, tint_nodesNInput[j]);
					}
				}
				printf("x: %d\n", x);

				for (j=0; j<int_nbreNodesInput; j++) {
					if(!containsElt(tint_mergeInputs, int_nbreMergeInputs,tint_nodesInput[j])){
						addElt(tint_mergeInputs, int_nbreMergeInputs, tint_nodesInput[j]);
						int_nbreMergeInputs++;
						impair = tint_nodesInput[j];
					}
				}

				// Ajouter le node fusion dans merge nodes et supprimer les deux nodes inputs dans input pour réduire le X
				for (j=0; j<int_nbreMergeInputs; j++) {
					printf("MInput: %d\n", tint_mergeInputs[j]);
				}
				printf("%d, %d\n", int_nbreMergeInputs, int_nbreMergeNodes);
				for (j=0; j<int_nbreMergeNodes; j++) {
					printf("MNodes: %d\n", tint_mergeNodes[j]);
					//addElt(tint_mergeInputs, int_nbreMergeInputs, tint_mergeNodes[j]);
					tint_mergeInputs[int_nbreMergeInputs] = tint_mergeNodes[j];
					printf("MNodes: %d\n", tint_mergeNodes[j]);
					printf("Add: %d\n", tint_mergeInputs[int_nbreMergeInputs]);	
					int_nbreMergeInputs++;
					//removeElt(tint_mergeNodes, int_nbreMergeNodes, tint_mergeNodes[j]);
					//int_nbreMergeNodes--;
				}
				printf("%d, %d\n", int_nbreMergeInputs, int_nbreMergeNodes);
				for (j=0; j<int_nbreMergeInputs; j++) {
					printf("MInput: %d\n", tint_mergeInputs[j]);
				}
			}


			for (j=0; j<int_nbreMergeInputs; j++) {
				int_nbreInputLUT = 0;
				printf("sku ? %d\n\n", tint_mergeInputs[j]);
				if (!containsElt(tint_listInput, DAG_input.int_input, tint_mergeInputs[j])) {
					addElt(tint_inputLUT, int_nbreInputLUT, tint_mergeInputs[j]);
					int_nbreInputLUT++;
					while (!(finish(tint_inputLUT, int_nbreInputLUT, tint_listInput, DAG_input.int_input))) {
						for (k=0; k<int_nbreInputLUT; k++) {
							printf("Merge Inputs j: %d\n", tint_mergeInputs[j]);
							if (!(containsElt(tint_listInput, DAG_input.int_input, tint_inputLUT[k]))) {
								p1 = parents(DAG_input, tint_inputLUT[k])[0];
								p2 = parents(DAG_input, tint_inputLUT[k])[1];
								printf("fiston: %d\n", tint_inputLUT[k]);
								printf("darons: %d %d\n", p1, p2);
								if (!containsElt(tint_inputLUT, int_nbreInputLUT, p1)) {
									tint_inputLUT[k] = p1;
									if (!containsElt(tint_inputLUT, int_nbreInputLUT, p2)) {
										addElt(tint_inputLUT, int_nbreInputLUT, p2);
										int_nbreInputLUT++;
									}
								}
								else {
									if (containsElt(tint_inputLUT, int_nbreInputLUT, p2)) {
										removeElt(tint_inputLUT, int_nbreInputLUT, tint_inputLUT[k]);
										int_nbreInputLUT--;
									}
									else {
										tint_inputLUT[j] = p2;
									}
								}
							}
						} 
					}
					printf("SKUSKSUSKSU\n");
					if (!containsElt(tint_listLUT, int_duplicateLUT, tint_mergeInputs[j])) {
						printf("%d\n", int_nbreLUTS);
						printf("%d\n", int_duplicateLUT);
						LUT.int_output = tint_mergeInputs[j];
						LUT.int_nbreInputs = int_nbreInputLUT;
						LUT.int_inputs = tint_inputLUT;
						LUTS[int_nbreLUTS] = LUT;
						printf("%d, %d: %d %d %d\n\n", LUTS[int_nbreLUTS].int_nbreInputs, LUTS[int_nbreLUTS].int_output, LUTS[int_nbreLUTS].int_inputs[0], LUTS[int_nbreLUTS].int_inputs[1], LUTS[int_nbreLUTS].int_inputs[2]);
						int_nbreLUTS++;
						addElt(tint_listLUT, int_duplicateLUT, tint_mergeInputs[j]);
						int_duplicateLUT++;
						//tint_inputLUT = realloc(tint_inputLUT, (int_nbreInputLUT)*sizeof(int));
					}
					addElt(tint_bigL, int_nbreL, tint_mergeInputs[j]);
					int_nbreL++;
					int_nbreInputLUT = 0;
					tint_inputLUT = malloc((int_nbreInputLUT)*sizeof(int));
				}
			}
			if(impair!=0){
				addElt(tint_bigL, int_nbreL, impair);
				int_nbreL++;
				impair = 0;
			}
			printf("%d\n", int_nbreLUTS);
			LUT.int_output = tint_listOutput[i];
			LUT.int_nbreInputs = int_nbreL;
			LUT.int_inputs = tint_bigL;
			LUTS[int_nbreLUTS] = LUT;
			printf("%d, %d: %d %d %d\n\n", LUTS[int_nbreLUTS].int_nbreInputs, LUTS[int_nbreLUTS].int_output, LUTS[int_nbreLUTS].int_inputs[0], LUTS[int_nbreLUTS].int_inputs[1], LUTS[int_nbreLUTS].int_inputs[2]);
			int_nbreLUTS++;
			int_nbreL = 0;
			tint_bigL = malloc((int_nbreL)*sizeof(int));


			int_nbreL = 0;
			//free(tint_bigL);
			int_nbreInputLUT = 0;
			//free(tint_inputLUT);	
			//tint_bigL = malloc(int_nbreL*sizeof(int));		
			tint_inputLUT = malloc(int_nbreInputLUT*sizeof(int));
		}

			



		int_nbreNodesInput = 0;
		int_nbreNodesNInput = 0;
		//free(tint_nodesInput);
		//free(tint_nodesNInput);
		tint_nodesInput = malloc(int_nbreNodesInput*sizeof(int));
		tint_nodesNInput = malloc(int_nbreNodesNInput*sizeof(int));

		//free(tint_mergeInputs);
		//free(tint_mergeNodes);
		int_nbreMergeInputs = 0;
		int_nbreMergeNodes = 0;
		tint_mergeInputs = malloc(int_nbreMergeInputs*sizeof(int));
		tint_mergeNodes = malloc(int_nbreMergeNodes*sizeof(int));

	}
	//free(tint_listLUT);

	return LUTS; */