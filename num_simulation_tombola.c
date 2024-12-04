#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TRUE	1
#define FALSE	0
#define ROWS	3
#define COL		9
#define N	1000000
#define TOTAL_BAR	50

double val_medio(int *a, int size){
	double vm = 0;
	for(int i = 0; i < size; i++){
		vm += (i + 1)*(((double) a[i] ) / N);
	}
	return vm;
}

double varianza(int *a, int size){
	double var = 0;
	double v1 = 0;
	double vm = val_medio(a, size);
	for(int i = 0; i < size; i++){
		v1 += (i + 1)*(i + 1)*(((double)a[i] )/ N);
	}
	var = v1 - vm*vm;
	return var;
}

void print_array(int a[ROWS][COL]){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COL; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

void print_counter(int *a, int size){
	for(int i = 0; i < size; i++)
		printf("\n%d: %d\t%.3f%%", i + 1, a[i], ( ((double) a[i]) / N )* 100);
}

int check_nums(int a[ROWS][COL], int c){
	if(((a[0][c] == 0) && (a[1][c] == 0)) || ((a[0][c] == 0) && (a[2][c] == 0)) || ((a[1][c] == 0) && a[2][c] == 0))
		return TRUE;
	else if((a[0][c] == a[1][c]) || (a[0][c] == a[2][c]) || (a[1][c] == a[2][c]))
		return FALSE;
	return TRUE;
}

void zeros_one_d(int *a, int size){
	for(int i=0; i < size; i++)
		a[i] = 0;
}

void zeros_two_d(int a[ROWS][COL]){
	for(int i=0; i < ROWS; i++)
		for(int j = 0; j < COL; j++)
			a[i][j] = 0;
}

void init_tabellone(int *a){
	for(int i = 0; i < 90; i++)
		a[i] = i + 1;
}

int check_tombola(int a[ROWS][COL]){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COL; j++){
			if(a[i][j] != 0)
				return FALSE;
		}
	}
	return TRUE;
}

int find(int *a, int size, int num){
	for(int i = 0; i < size; i++)
		if(a[i] == num)
			return TRUE;
	return FALSE;
}

void find_and_remove(int a[ROWS][COL], int num){
	for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COL; j++)
			if(a[i][j] == num){
				a[i][j] = 0;
				return;
			}
}

void find_and_remove_one_d(int *a, int size, int n){
	for(int i = 0; i < size; i++)
		if(a[i] == n)
			a[i] = 0;
}

void progress_bar(int totale, double progresso){
	system("CLS");
	printf("\n[");
	for(int i = 0; i < floor(progresso); i++){
		printf("#");
	}
	for(int i = floor(progresso); i < totale; i++){
		printf("-");
	}
	printf("]");
	printf("  %.2f%%", (progresso / totale) * 100);
}

int main(){
	int tabella[ROWS][COL];
	int rc[ROWS]; // row counter, quanti numeri ci sono in ogni riga
	int num;
	int estratto;
	int numero_tombola;
	int counter[90];
	int tabellone[90];

	zeros_one_d(counter, 90);
	srand(time(NULL));
	for(int r = 0; r < N; r++){
		zeros_one_d(rc, ROWS); // inizializza tutti gli elementi di rc a zero
		zeros_two_d(tabella);	 // inizializza tutti gli elementi della tabella a zero
		do{
			// CREA TABELLA
			/*
			Le tabelle devono avere 15 colonne e 5 numeri per riga distribuiti in modo equiprobabile (20%). Bisogna impostarlo
			con un ciclo while che continua fino a quando non si riempiono le 5 caselle.
			Ogni colonna ha un range:
			- colonna 0: 0 < x <= 10
			- colonna 1: 10 < x <= 20
			...
			- colonna 8: 80 < x <= 90
			I numeri di ogni colonna sono assegnati con la stessa verosimiglianza
			Nelle caselle alle quali non viene assegnato un numero si pone zero.
			*/
			for(int i = 0; i < COL; i++){
				for(int j = 0; j < ROWS; j++){
					if((rc[j] < 5) && ((rand() % 100) < 20) && (tabella[j][i] == 0)){ // 20% si riempie quella casella se la riga conta meno di 5 numeri e non è stato assegnato nessun'altro numero prima
					num = (rand() % 10 + 1) + 10*i;
					tabella[j][i] = num;

					while(!check_nums(tabella, i)){ // fino a quando il numero pescato è lo stesso su più di una riga, ripesca
						num = (rand() % 10 + 1) + 10*i;
						tabella[j][i] = num;
					}
					rc[j]++;
					}
				}
			}
		}while((rc[0] + rc[1] + rc[2]) < 15); // fino a quando nella tabella non si hanno 15 numeri
		// print_array(tabella);


		// Estrazione e conteggio
		/*
		L'estrazione consiste nell'estrarre un numero a caso dal tabellone, se fa parte della propria tabella viene
		conteggiato, altrimenti si va avanti. Si vuole contare come si distribuiscono le probabilità di fare tombola
		*/
		init_tabellone(tabellone);
		numero_tombola = 0;
		while(!check_tombola(tabella)){ // fino a quando non si ha fatto tombola
			estratto = (rand() % 90) + 1;
			if(find(tabellone, 90, estratto)){
				// printf("\n%d", estratto);
				find_and_remove(tabella, estratto);
				find_and_remove_one_d(tabellone, 90, estratto);
				numero_tombola++;
			}
		}
		counter[numero_tombola-1]++;
		// printf("\n");
		if(!(r % 10000)){
			printf("\ninside");
			progress_bar(TOTAL_BAR, (((double) r )/ N) * TOTAL_BAR);
		}
	}
	system("CLS");
	print_counter(counter, 90);
	double var = varianza(counter, 90);
	double valore_medio = val_medio(counter, 90);

	printf("\nIl valore medio: %.2f", valore_medio);
	printf("\nLa varianza: %.2f", var);
	return 0;
}