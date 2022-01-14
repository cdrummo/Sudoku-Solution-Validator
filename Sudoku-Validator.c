/*******************************************************/
/*   Multi-Threaded Sudoku Puzzle Solution Validator   */
/*		Author: Connor Drummond		       */
/*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*******************GLOBAL VARIABLES*******************/

typedef int bool;
#define TRUE 1
#define FALSE 0

typedef struct {
	int topRow;
  int bottomRow;
	int leftColumn;
  int rightColumn;
} parameters;

int sudokuPuzzle[9][9];
bool columnValid[9];
bool rowValid[9];
bool subgridValid[9];

void red () { printf("\033[1;31m"); }
void green () { printf("\033[1;32m"); }
void reset () { printf("\033[0m"); }
void * check(void * params);

int main(){
/*******************LOADING SUDOKU PUZZLE*******************/
  FILE *file1 = fopen("SudokuPuzzle.txt", "r");
  int i, j;

  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      fscanf(file1,"%d", &sudokuPuzzle[i][j]);
    }
  }

	fclose(file1);

	printf("========================PUZZLE=========================\n");
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      printf("%d", sudokuPuzzle[i][j]);
      printf(" ");
    }
  printf("\n");
  }

/*******************LOADING PARAMETERS*******************/
  parameters col_param[9];
	parameters row_param[9];
	parameters sg_param[9];

  for(i=0;i<9;i++){
    col_param[i].topRow = 0;
    col_param[i].bottomRow = 8;
    col_param[i].leftColumn = i;
    col_param[i].rightColumn = i;

		row_param[i].topRow = i;
    row_param[i].bottomRow = i;
    row_param[i].leftColumn = 0;
    row_param[i].rightColumn = 8;

		sg_param[i].topRow = (i/3) * 3;
    sg_param[i].bottomRow = sg_param[i].topRow + 2;
    sg_param[i].leftColumn = (i % 3) * 3;
    sg_param[i].rightColumn = sg_param[i].leftColumn + 2;
  }

/*******************THREAD CREATION*******************/
  pthread_t tid_column[9];
	pthread_t tid_row[9];
	pthread_t tid_sg[9];

	printf("========================THREADS========================\n");
  for(i=0;i<9;i++){
    pthread_create(&(tid_column[i]), NULL, check, &(col_param[i]));
		pthread_create(&(tid_row[i]), NULL, check, &(row_param[i]));
		pthread_create(&(tid_sg[i]), NULL, check, &(sg_param[i]));
  }

  for (i = 0; i < 9; i++) {
		pthread_join(tid_column[i],NULL);
		pthread_join(tid_row[i],NULL);
		pthread_join(tid_sg[i],NULL);
	}

/*******************PRINTING RESULTS*******************/
	printf("========================COLUMNS========================\n");
	for (i = 0; i < 9; i++) {
		printf("Column[%d]: Thread ID: %lX  ", i + 1, tid_column[i]);
		if(columnValid[i] == TRUE) {
			green();
			printf("VALID!\n");
			reset();
		}
		else{
			red();
			printf("INVALID! \n");
			reset();
		}
	}

	printf("==========================ROWS=========================\n");
	for (i = 0; i < 9; i++) {
		printf("Row[%d]: Thread ID: %lX  ", i + 1, tid_row[i]);
		if(rowValid[i] == TRUE) {
			green();
			printf("VALID!\n");
			reset();
		}
		else{
			red();
			printf("INVALID! \n");
			reset();
		}
	}

	printf("========================SUBGRIDS=======================\n");
	for (i = 0; i < 9; i++) {
		printf("Subgrid[%d]: Thread ID: %lX  ", i + 1, tid_sg[i]);
		if(subgridValid[i] == TRUE) {
			green();
			printf("VALID!\n");
			reset();
		}
		else{
			red();
			printf("INVALID! \n");
			reset();
		}
	}

	bool valid;
	for (i = 0; i < 9; i++) {
		if (columnValid[i] == FALSE || rowValid[i] == FALSE || subgridValid[i] == FALSE) {
			valid = FALSE;
			break;
		}
		else{
			valid = TRUE;
		}
	}

	printf("========================RESULTS========================\n");
	if (valid == TRUE) {
		printf("SUDOKU PUZZLE IS ");
		green();
		printf("VALID!\n");
		reset();
	}
	else {
		printf("SUDOKU PUZZLE IS ");
		red();
		printf("INVALID! \n");
		reset();
	}

}


/*******************FUNCTION: CHECK*******************/
void * check(void * params) {
  parameters * data = (parameters *)params;
  int startRow = data->topRow;
  int endRow = data->bottomRow;
  int startCol = data->leftColumn;
  int endCol = data->rightColumn;

	pthread_t self = pthread_self();

  int i, j;
  int checker[9] = {0,0,0,0,0,0,0,0,0}; //load with zeros for loop

  for (i = startRow; i <= endRow; i++) {
    for (j = startCol; j <= endCol; j++) {
      int val = sudokuPuzzle[i][j] - 1;
      if (checker[val] != 0) {
        checker[val] = 0;
      }
      else{
        checker[val] = 1;
      }
    }
  }

	bool valid;
	for (i = 0; i < 9; i++) {
		if (checker[i] == 0){
			valid = FALSE;
			break;
		}
		else{
			valid = TRUE;
		}
	}

	if (startCol == endCol) {
		columnValid[startCol] = valid;
	} //if
	else if (startRow == endRow) {
		rowValid[startRow] = valid;
	} // else if
	else {
		int indx = startRow + (startCol/3);
		subgridValid[indx] = valid;
	}

	printf("Thread ID: %lX Parameters: (%d, ", self, startRow + 1);
	printf("%d, %d, ", endRow + 1, startCol + 1);
	printf("%d) ", endCol + 1);

	if(valid == TRUE) {
		green();
		printf("VALID!\n");
		reset();
	}
	else{
		red();
		printf("INVALID! \n");
		reset();
	}
}
