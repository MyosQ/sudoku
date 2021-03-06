#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 9
#define BOXSIZE 3
#define NUMBER_OF_SUDOKUS 1

int ok(char** arr, int row, int col, int k);
int su_solver(char** arr, int row, int col);
void err_sys(char* msg);

int main(int argc, char** argv){
	FILE* fd;
	int a, i = 0, k = 0, solved, sudNum = 0;
	char ** arr;
	clock_t start, end;
	start = clock();

	if(argc != 2){
		printf("Usage: <program> <textfile>\n");
		exit(EXIT_FAILURE);
	}

	/* Open file */
	if((fd = fopen(argv[1], "r")) == NULL)
		err_sys("Can't open file");

	/* Allocate for 2-d array */
	if ((arr = calloc(SIZE+1, sizeof(char*))) == NULL)
		err_sys("Allocation error\0");
	for(a = 0; a < SIZE+1; a++)
		if((arr[a] = calloc(SIZE+1, sizeof(char))) == NULL)
			err_sys("Allocation error\0");

	/* The 'main' loop. One SuDoku at a time. */
	while(sudNum < NUMBER_OF_SUDOKUS){
		k = 0;
		i = 0;

		/* Skip first line */
		while((a = getc(fd)) != '\n');

		/* Read into 2-d array */
		while(k < 9 && a != EOF){
			while((a = getc(fd)) != '\n' && a != EOF){
				arr[k][i] = a;
				i++;
			}
			i = 0;
			k++;
		}

		/* Solving part */
		if ((solved = su_solver(arr, 0, 0)) < 0){
			printf("Sudoku #%d: No solution\n", sudNum+1);
		}

		else{
			printf("Sudoku #%d: Solved!\n", sudNum+1);
			for (a = 0; a < SIZE; a++) {
				fputs(arr[a], stdout);
				putc('\n', stdout);
			}
			putc('\n', stdout);
		}
		sudNum++;
	}
	/*--- End of Su Doku's ---*/

	/* Print time taken */
	end = clock();
	printf("time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

	/* Freeing */

	fclose(fd);
	for(a = 0; a < 10; a++){
		free(arr[a]);
	}
	free(arr);
	return 0;
}

/* The valid-checker.
 Returns 0 if number works, else -1. */
int ok(char** arr, int row, int col, int k){
	int a, b, startRow, startCol;

	/* horizontally */
	for(a = 0; a < SIZE; a++)
		if(arr[row][a] == k+48 && a != col)
			return -1;

	/* Vertically */
	for(a = 0; a < SIZE; a++)
		if(arr[a][col] == k+48 && a != row)
			return -1;

	/* "Boxly" */
	startRow = row / 3 * 3;
	startCol = col / 3 * 3;

	for(a = startRow; a < startRow + BOXSIZE; a++)
		for(b = startCol; b < startCol + BOXSIZE; b++)
			if(arr[a][b] == k+48 && a != row && b != col)
				return -1;
	return 0;
}


/* The sudoku solver.
Returns 0 if puzzle solved, else -1.
If solved it updates arr with it. */
int su_solver(char** arr, int row, int col){

	/* Goto next row if column 9*/
	if(col == 9){
		col = 0;
		row++;
	}

	/* Checks if puzzle is complete */
	if(row == 9)
		return 0;

	/* See if square is already filled.
	If so, it returns whatever its child returns*/
	if (arr[row][col] != '0')
		 return su_solver(arr, row, col + 1);

	/* Tests numbers 1-9. If ok, then it
	calls recursively on itself */
	int k = 1;
	while(k <= 9){
		arr[row][col] = k + 48;
		if (ok(arr, row, col, k) == 0)
			if(su_solver(arr, row, col+1) == 0)
				return 0;

		k++;
	}
	arr[row][col] = '0';
	return -1;
}

/* Error message */
void err_sys(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
