/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAXPOSDECIMAL 2147483647
#define MINNEGDECIMAL -2147483648
#define MAXOFFSET 32767
#define MINOFFSET -32768

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int find_opcode(char *);
int check_registers(char *);
int check_offset(char *);
int handle_fill(char *);

// array to save calculated symbolic labels
char labels[MAXLINELENGTH][7];
int labels_value[MAXLINELENGTH];

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* TODO: Phase-1 label calculation */
	int line_no = 0;

	// read every lines
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		if (label[0] != '\0') {
			// if label exceed the maximum length, exit
			if (strlen(label) > 6) {
				printf("Label exceeded maximum length!\n");
				exit(1);
			}
			// if duplicated definition occurs, exit
			for (int i = 0; i < MAXLINELENGTH; i++) {
				if (!strcmp(labels[i], label)) {
					printf("Error! Duplicated definition of labels!\n");
					exit(1);
					break;
				}
			}
			// find empty place to save the label and its numeric value.
			for (int i = 0; i < MAXLINELENGTH; i++) {
				if (!(strcmp(labels[i],""))) {
					strcpy(labels[i], label);
					labels_value[i] = line_no;
					break;
				}
			}
		}
		line_no++; 
	}
	// for (int i = 0; i < 50; i++) {
	// 	if (strlen(labels[i])>0)
	// 		printf("Label : %s, Value : %d\n", labels[i], labels_value[i]);
	// }

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */
	int line_no1 = 0;

	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		int machine_code = 0;
		int regA, regB, destReg, offsetField;
		if (!strcmp(opcode, ".fill")) {
			int value = handle_fill(arg0);
			machine_code |= value;
		}
		else {
			int opcode_bi = find_opcode(opcode);

			// store opcode on machine_code
			machine_code |= (opcode_bi << 22);

			// R-type instructions
			if (opcode_bi == 0b000 || opcode_bi == 0b001) {
				// check registers are valid
				check_registers(arg0);
				check_registers(arg1);
				check_registers(arg2);

				// switch to integer type
				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);
				sscanf(arg2, "%d", &destReg);
				
				machine_code |= (regA << 19);
				machine_code |= (regB << 16);
				machine_code |= (destReg);
			}
			// I-type instructions
			else if (opcode_bi == 0b010 || opcode_bi == 0b011 || opcode_bi == 0b100) {
				check_registers(arg0);
				check_registers(arg1);
				
				offsetField = check_offset(arg2);
				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);

				machine_code |= (regA << 19);
				machine_code |= (regB << 16);
				
				// calculate relative branching address
				if (opcode_bi == 0b100 && !isNumber(arg2)) {
					offsetField = offsetField - (line_no1+1);
				}
				// chop 32 bits integer into 16 bits
				machine_code |= (offsetField & 0xffff);
			}
			// J-type instruction
			else if (opcode_bi == 0b101) {
				check_registers(arg0);
				check_registers(arg1);

				sscanf(arg0, "%d", &regA);
				sscanf(arg1, "%d", &regB);

				machine_code |= (regA << 19);
				machine_code |= (regB << 16);
			}
			// O-type has only opcode change
		}
		// save machine code to output file
		fprintf(outFilePtr, "%d\n", machine_code);
		line_no1++;
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int find_opcode(char *opcode) {
	if (!strcmp(opcode, "add"))
		return 0b000;
	else if (!strcmp(opcode, "nor"))
		return 0b001;
	else if (!strcmp(opcode, "lw"))
		return 0b010;
	else if (!strcmp(opcode, "sw"))
		return 0b011;
	else if (!strcmp(opcode, "beq"))
		return 0b100;
	else if (!strcmp(opcode, "jalr"))
		return 0b101;
	else if (!strcmp(opcode, "halt"))
		return 0b110;
	else if (!strcmp(opcode, "noop"))
		return 0b111;
	else {
		printf("Error! Unrecoginzed opcode %s!\n", opcode);
		exit(1);
	}
}

int check_registers(char *reg) {
	// if register is non-integer argument.
	if (!isNumber(reg)) {
		printf("Error! Non-integer register arguments!\n");
		exit(1);
	}
	int num;
	sscanf(reg, "%d", &num);

	// if register is outside the range [0,7]
	if (num < 0 || num > 7) {
		printf("Error! Registers outside the range [0,7]!\n");
		exit(1);
	}
	return 1;
}

int check_offset(char *offset) {
	int value;
	// if offsetField is numeric value
	if (isNumber(offset)) {
		sscanf(offset, "%d", &value);
		if (value > MAXOFFSET || value < MINOFFSET) {
			printf("Error! OffsetFields does not fit in 16 bits!\n");
			exit(1);
		}
		return value;
	}
	// if offsetField is symbolic name
	else {
		for(int i = 0; i < MAXLINELENGTH; i++) {
			if (!strcmp(labels[i], offset)){
				return labels_value[i];
			}
		}
		// if the label does not defined
		printf("Error! Label : '%s' is undefined!\n", offset);
		exit(1);
	}
}

int handle_fill(char *arg) {
	int value;
	if (isNumber(arg)) {
		sscanf(arg, "%d", &value);
		if (value < MINNEGDECIMAL || value > MAXPOSDECIMAL) {
			printf("Error! Out of .fill instruction range!\n");
			exit(1);
		}
	}
	else {
		int flag = 0;
		for (int j = 0; j < MAXLINELENGTH; j++) {
			if (!strcmp(labels[j], arg)) {
				value = labels_value[j];
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("Error! Label : '%s' is undefined!\n", arg);
			exit(1);
		}
	}
	return value;
}