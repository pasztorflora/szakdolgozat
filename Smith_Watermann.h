#ifndef SMITH_WATERMANN_H_INCLUDED
#define SMITH_WATERMANN_H_INCLUDED
#include "/opt/Xilinx/Vivado/2018.3/include/gmp.h"
#include "ap_int.h"
typedef short dtype;
typedef int resulttype;

//constants
const int Seq_in_one_group=4;       //number of sequences in one group
const int Max_seq_len=256;			//maximal sequence length

struct dihedral_angle
	{
	dtype phi,psi;
	};
struct cell
	{
	resulttype value;
	bool horizontal_gap, vertical_gap;
	};

//top function
void Smith_Waterman(cell& result, cell left,cell top, cell prev,dihedral_angle seq1,
		dihedral_angle seq2, resulttype K, resulttype max_gap);

//scoring functions
resulttype gap(resulttype starter, int previous);
resulttype distance (dihedral_angle seq1, dihedral_angle seq2, dtype K);

//helpers
resulttype min(resulttype a, resulttype b);
resulttype dif(resulttype a, resulttype b);
resulttype max(resulttype a, resulttype b, resulttype c, resulttype d);
resulttype my_abs(resulttype a);
#endif
