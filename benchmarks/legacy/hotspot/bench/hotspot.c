#include <stdio.h>
#include <stdlib.h>
//#include <omp.h>
#include <sys/time.h>
//using namespace std;
#include "../defines.h"

/* chip parameters	*/
#define t_chip 0.0005
#define chip_height 0.016
#define chip_width 0.016
/* ambient temperature, assuming no package at all	*/
#define amb_temp 80.0
//int num_omp_threads;

/* Transient solver driver routine: simply converts the heat 
 * transfer differential equations to difference equations 
 * and solves the difference equations by iterating
 */
void compute_tran_temp(double *result, double *temp, double *power) 
{
//	#ifdef VERBOSE
	int i = 0;
//	#endif

	double grid_height = chip_height / GRID_ROWS;
	double grid_width = chip_width / GRID_COLS;

	double Cap = FACTOR_CHIP * SPEC_HEAT_SI * t_chip * grid_width * grid_height;
	double Rx = grid_width / (2.0 * K_SI * t_chip * grid_height);
	double Ry = grid_height / (2.0 * K_SI * t_chip * grid_width);
	double Rz = t_chip / (K_SI * grid_height * grid_width);

	double max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
	double step = PRECISION / max_slope;
	double t;		
    double delta;
    int r, c;

     for (int i = 0; i < SIM_TIME ; i++)
	{
/*    #ifdef OPEN
	    omp_set_num_threads(NUM_THREADS);
        #pragma omp parallel for shared(power, temp,result) private(r, c, delta) firstprivate(GRID_ROWS, GRID_COLS) schedule(static)
    #endif
*/
	    for (r = 0; r < GRID_ROWS; r++) {
		    for (c = 0; c < GRID_COLS; c++) {
      			/*	Corner 1	*/
			    if ( (r == 0) && (c == 0) ) {
				    delta = (step / Cap) * (power[0] +
						    (temp[1] - temp[0]) / Rx +
						    (temp[GRID_COLS] - temp[0]) / Ry +
						    (amb_temp - temp[0]) / Rz);
			    }	/*	Corner 2	*/
			    else if ((r == 0) && (c == GRID_COLS-1)) {
				    delta = (step / Cap) * (power[c] +
						    (temp[c-1] - temp[c]) / Rx +
						    (temp[c+GRID_COLS] - temp[c]) / Ry +
						    (amb_temp - temp[c]) / Rz);
			    }	/*	Corner 3	*/
			    else if ((r == GRID_ROWS-1) && (c == GRID_COLS-1)) {
				    delta = (step / Cap) * (power[r*GRID_COLS+c] + 
						    (temp[r*GRID_COLS+c-1] - temp[r*GRID_COLS+c]) / Rx + 
						    (temp[(r-1)*GRID_COLS+c] - temp[r*GRID_COLS+c]) / Ry + 
						    (amb_temp - temp[r*GRID_COLS+c]) / Rz);					
			    }	/*	Corner 4	*/
			    else if ((r == GRID_ROWS-1) && (c == 0)) {
				    delta = (step / Cap) * (power[r*GRID_COLS] + 
						    (temp[r*GRID_COLS+1] - temp[r*GRID_COLS]) / Rx + 
						    (temp[(r-1)*GRID_COLS] - temp[r*GRID_COLS]) / Ry + 
						    (amb_temp - temp[r*GRID_COLS]) / Rz);
			    }	/*	Edge 1	*/
			    else if (r == 0) {
				    delta = (step / Cap) * (power[c] + 
						    (temp[c+1] + temp[c-1] - 2.0*temp[c]) / Rx + 
						    (temp[GRID_COLS+c] - temp[c]) / Ry + 
						    (amb_temp - temp[c]) / Rz);
			    }	/*	Edge 2	*/
			    else if (c == GRID_COLS-1) {
				    delta = (step / Cap) * (power[r*GRID_COLS+c] + 
						    (temp[(r+1)*GRID_COLS+c] + temp[(r-1)*GRID_COLS+c] - 2.0*temp[r*GRID_COLS+c]) / Ry + 
						    (temp[r*GRID_COLS+c-1] - temp[r*GRID_COLS+c]) / Rx + 
						    (amb_temp - temp[r*GRID_COLS+c]) / Rz);
			    }	/*	Edge 3	*/
			    else if (r == GRID_ROWS-1) {
				    delta = (step / Cap) * (power[r*GRID_COLS+c] + 
						    (temp[r*GRID_COLS+c+1] + temp[r*GRID_COLS+c-1] - 2.0*temp[r*GRID_COLS+c]) / Rx + 
						    (temp[(r-1)*GRID_COLS+c] - temp[r*GRID_COLS+c]) / Ry + 
						    (amb_temp - temp[r*GRID_COLS+c]) / Rz);
			    }	/*	Edge 4	*/
			    else if (c == 0) {
				    delta = (step / Cap) * (power[r*GRID_COLS] + 
						    (temp[(r+1)*GRID_COLS] + temp[(r-1)*GRID_COLS] - 2.0*temp[r*GRID_COLS]) / Ry + 
						    (temp[r*GRID_COLS+1] - temp[r*GRID_COLS]) / Rx + 
						    (amb_temp - temp[r*GRID_COLS]) / Rz);
			    }	/*	Inside the chip	*/
			    else {
				    delta = (step / Cap) * (power[r*GRID_COLS+c] + 
						    (temp[(r+1)*GRID_COLS+c] + temp[(r-1)*GRID_COLS+c] - 2.0*temp[r*GRID_COLS+c]) / Ry + 
						    (temp[r*GRID_COLS+c+1] + temp[r*GRID_COLS+c-1] - 2.0*temp[r*GRID_COLS+c]) / Rx + 
						    (amb_temp - temp[r*GRID_COLS+c]) / Rz);
			    }
      			
			    /*	Update Temperatures	*/
			    result[r*GRID_COLS+c] =temp[r*GRID_COLS+c]+ delta;

		    }
	    }
/*
    #ifdef OPEN
	    omp_set_num_threads(NUM_THREADS);
	    #pragma omp parallel for shared(result, temp) private(r, c) schedule(static)
    #endif
*/
	    for (r = 0; r < GRID_ROWS; r++) {
		    for (c = 0; c < GRID_COLS; c++) {
			    temp[r*GRID_COLS+c]=result[r*GRID_COLS+c];
		    }
	    }
	}	
}
