#include "Smith_Watermann.h"


//scoring functions

	resulttype gap(resulttype starter, bool previous)
{
	if(previous)
		return starter/5;
	return starter;
}

	resulttype distance (dihedral_angle seq1, dihedral_angle seq2)
{

	dtype a=seq1.phi-seq2.phi;
	dtype b=seq1.psi-seq2.psi;
	int d=my_abs(a)+my_abs(b);
	if(d>32767)
		return 32767;
	return d;

}

//helpers
	resulttype my_abs(resulttype a)
	{
		if(a>0)
			return a;
		return -a;
	}

	resulttype max(resulttype a, resulttype b, resulttype c, resulttype d)
{
		resulttype maximum=a;
    if(b>maximum) maximum=b;
    if(c>maximum) maximum=c;
    if(d>maximum) maximum=d;
    return maximum;
}

	resulttype dif(resulttype a, resulttype b)
{
	if(a>b)
		return a-b;
	return b-a;
}

	resulttype min(resulttype a, resulttype b)
{
	if(a>b)
		return b;
	return a;
}

//top function

void Smith_Waterman(cell& result, cell left,cell top, cell prev,dihedral_angle seq1,
		dihedral_angle seq2, resulttype K, resulttype max_gap)
{
	resulttype hor=left.value+gap(max_gap,left.horizontal_gap);
	resulttype ver=top.value+gap(max_gap,top.vertical_gap);
	resulttype diag=prev.value+K-distance(seq1,seq2);

	if(hor==ver&&hor>=diag&&hor>0)
	{
		result.value=hor;
		result.horizontal_gap=true;
		result.vertical_gap=true;
	}
	else
	{
		if(hor>ver)
			{
				if(hor>=diag)
				{
					if(hor>=0)
					{
						result.value=hor;
						result.horizontal_gap=true;
						result.vertical_gap=false;
					}
					else
					{
						result.value=0;
						result.horizontal_gap=false;
						result.vertical_gap=false;
					}
				}
				else
				{
					if(diag>0)
					{
						result.value=diag;
					}
					else
					{
						result.value=0;
					}

					result.horizontal_gap=false;
					result.vertical_gap=false;
				}
			}
			else
			{
				if(ver>=diag)
					{
						if(ver>=0)
						{
							result.value=ver;
							result.horizontal_gap=false;
							result.vertical_gap=true;
						}
						else
						{
							result.value=0;
							result.horizontal_gap=false;
							result.vertical_gap=false;
						}
					}
				else
					{
						if(diag>0)
						{
							result.value=diag;
						}
						else
						{
							result.value=0;
						}

						result.horizontal_gap=false;
						result.vertical_gap=false;
					}
			}
	}




}