#ifndef S_W_H_INCLUDED
#define S_W_H_INCLUDED
#include "Smith_Watermann.h"
#include "S_W_hw.h"
int modN(int N, int a);
void S_W_hw(int N, int L1, int L2, resulttype input_maxs[Seq_in_one_group*Seq_in_one_group],
		int input_lengths1[Seq_in_one_group], int input_lengths2[Seq_in_one_group],
		dihedral_angle input_group1[Seq_in_one_group*Max_seq_len],
		dihedral_angle input_group2[Seq_in_one_group*Max_seq_len],
		resulttype K, resulttype gap);
#endif
