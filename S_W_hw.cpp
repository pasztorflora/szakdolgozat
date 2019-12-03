#include "Smith_Watermann.h"
#include "S_W_hw.h"


void S_W_hw(int N, int L1, int L2, resulttype input_maxs[Seq_in_one_group*Seq_in_one_group],
		int input_lengths1[Seq_in_one_group], int input_lengths2[Seq_in_one_group],
		dihedral_angle input_group1[Seq_in_one_group*Max_seq_len],
		dihedral_angle input_group2[Seq_in_one_group*Max_seq_len],
		resulttype K, resulttype gap)
{

		resulttype maxs[Seq_in_one_group][Seq_in_one_group];
		int lengths1[Seq_in_one_group];
		int lengths2[Seq_in_one_group];
		dihedral_angle group1[Seq_in_one_group*Max_seq_len];
		dihedral_angle group2[Seq_in_one_group*Max_seq_len];

		cell temp_results[Seq_in_one_group][Seq_in_one_group][Max_seq_len-2];  //1.csoport, masodik csoport,elozo sor eredmenye
		cell prev[Seq_in_one_group*Seq_in_one_group];
		cell top[Seq_in_one_group*Seq_in_one_group];
		cell left[Seq_in_one_group*Seq_in_one_group];

		cell zero_init;
		zero_init.horizontal_gap=false; zero_init.value=0; zero_init.vertical_gap=false;
		cell actual=zero_init;

		int pos1=0;
		int pos2=0;
		int pos3=0;
		ZERO:
		{
			INIT:
			for(int a=0;a<Seq_in_one_group*Seq_in_one_group;a++)
			{
				prev[a]=zero_init;
				top[a]=zero_init;
				left[a]=zero_init;
			}
			INIT_TEMP:
			for(int a=0;a<Seq_in_one_group*Seq_in_one_group*(Max_seq_len-2);a++)
			{
				temp_results[pos1][pos2][pos3]=zero_init;
				pos3++;
				if(pos3==Max_seq_len-2)
				{
					pos3=0;
					pos2++;
					if(pos2==Seq_in_one_group)
					{
						pos2=0;
						pos1++;
					}
				}
			}
		}
		LOADING:
		{

		INIT_MAXS:
		for(int a=0;a<Seq_in_one_group*Seq_in_one_group;a++)
		{
				maxs[a/Seq_in_one_group][a%Seq_in_one_group]=0;
		}
		int summ=0;
		INIT_LENGTHS1:
		for(int a=0;a<Seq_in_one_group;a++)
		{
			lengths1[a]=input_lengths1[a];
			INIT_GROUP1:for(int b=0;b<lengths1[a];b++)
			{
				group1[a*Max_seq_len+b]=input_group1[summ];
				summ++;
			}

		}
		summ=0;
		INIT_LENGTHS2:
		for(int a=0;a<Seq_in_one_group;a++)
		{
			lengths2[a]=input_lengths2[a];
			INIT_GROUP2:for(int b=0;b<lengths2[a];b++)
			{
				group2[a*Max_seq_len+b]=input_group2[summ];
				summ++;
			}

		}
		}

	int iterations=N*N*L1*L2;

		int temp_pos=0;
	    int seq1_pos=0;int seq2_pos=0;
	    int group1_pos=0;int group2_pos=0;
wholegroup:	    for(int i=0;i<Seq_in_one_group*Seq_in_one_group*Max_seq_len*Max_seq_len;i++)
	    {
			if(i<iterations)
			{
				if(seq1_pos<lengths1[group1_pos]&&seq2_pos<lengths2[group2_pos]) //szekvencia v�ge
					        {

					        	if(seq1_pos==0&&seq2_pos==0)
					        	{

									 Smith_Waterman(actual,
											 	 	zero_init,
													zero_init,
													zero_init,
													group1[group1_pos*Max_seq_len+seq1_pos],group2[group2_pos*Max_seq_len+seq2_pos],
													K,gap);
					        	}
					        	else
					        	{
					        		if(seq2_pos==0)
					        		{
					        			Smith_Waterman(actual,
					        							left[group1_pos*Seq_in_one_group+group2_pos],
														zero_init,
														zero_init,
														group1[group1_pos*Max_seq_len+seq1_pos],group2[group2_pos*Max_seq_len+seq2_pos],
														K,gap);
					        		}
					        		else
					        		{
					        			if(seq1_pos==0)
					        			{
					        				Smith_Waterman(actual,
					        								zero_init,
															top[group1_pos*Seq_in_one_group+group2_pos],
															zero_init,
															group1[group1_pos*Max_seq_len+seq1_pos],group2[group2_pos*Max_seq_len+seq2_pos],
															K,gap);
					        			}
					        			else
					        			{
					        				Smith_Waterman(actual,
					        						left[group1_pos*Seq_in_one_group+group2_pos],
													top[group1_pos*Seq_in_one_group+group2_pos],
															prev[group1_pos*Seq_in_one_group+group2_pos],
															group1[group1_pos*Max_seq_len+seq1_pos],group2[group2_pos*Max_seq_len+seq2_pos],
															K,gap);

					        			}
					        		}
					        	}


					            if(actual.value>maxs[group1_pos][group2_pos])
					            {
					            	maxs[group1_pos][group2_pos]=actual.value;
					            }

					        }
				prev[group1_pos*Seq_in_one_group+group2_pos]=top[group1_pos*Seq_in_one_group+group2_pos];
				top[group1_pos*Seq_in_one_group+group2_pos]=temp_results[group1_pos][group2_pos][temp_pos];
				temp_results[group1_pos][group2_pos][temp_pos]=left[group1_pos*Seq_in_one_group+group2_pos];
				left[group1_pos*Seq_in_one_group+group2_pos]=actual;


					        	        if(group1_pos==N-1)
					        	        {
					        	            group1_pos=0;
					        	            if(group2_pos==N-1)
					        	            {
					        	                group2_pos=0;
					        	                if(temp_pos==L1-3)
					                            {
					                                temp_pos=0;
					                            }
					                            else
					                            {
					                                temp_pos++;
					                            }
					        	                if(seq1_pos==L1-1)
					        	                {
					        	                    seq1_pos=0;
					        	                    seq2_pos++;
					        	                }
					        	                else
					        	                {
						        	                seq1_pos++;
					        	                }
					        	            }
					        	            else
					        	            {
						        	            group2_pos++;
					        	            }
					        	        }
					        	        else
					        	        {
					        	        	group1_pos++;
					        	        }
			}
			else
			{
				break;
			}


	    }
RETURN:
	for(int a=0;a<Seq_in_one_group*Seq_in_one_group;a++)
		{
			input_maxs[a]=maxs[a/Seq_in_one_group][a%Seq_in_one_group];
		}


}
