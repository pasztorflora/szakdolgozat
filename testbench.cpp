#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <string>
#include <map>
#include "Smith_Watermann.h"
#include "S_W_hw.h"

using namespace std;

class input
{
    std::ifstream in;
    public:
    std::vector<std::vector<dihedral_angle> > sequences;

    void addelements(char* s, std::string pre, std::vector<std::string> &names, std::vector<std::string> &groups)
    {
    	dihedral_angle dih;
    	    std::vector<dihedral_angle> temp;
    		string name;
    		string actual;
    		char char_array[1024];
    		DIR *dir;
    		struct dirent *ent;
    		if ((dir = opendir (s)) != NULL) {
    		  while ((ent = readdir (dir)) != NULL) {
    			  //the name of the sequence can be stored if needed
    			  name=ent->d_name;
    			  //this might need to be changed, I get back a "." and a "..." which I need to throw away
    			  if(name.length()==11)
    				  {
    				  groups.push_back(pre);
    				  names.push_back(name);
    				  ifstream f;
    				  f.open(s+name);
    				  if(f.is_open())
    					  {
    					  //ignoring the header
    					  if(f.good())f.getline(char_array,1024);
    					  if(f.good())f.getline(char_array,1024);
    					  while(f.good())
    					  {
    						  f.getline(char_array,1024);
    						  actual=char_array;
    						  //ignoring the first 2 columns
    						  if(actual.find(' ')+1<actual.length()) actual=actual.substr(actual.find(' ')+1);
    						  if(actual.find(' ')+1<actual.length())
    							  {
    							  	  actual=actual.substr(actual.find(' ')+1);
    									std::string::size_type pointer;

    									try
    									{
    										dih.phi=((std::stof(actual,&pointer)/180)*32767);//*182);
    									}
    									catch(exception e)
    									{
    										continue;
    									}
    									try
    									{
    										dih.psi=((std::stof(actual.substr(pointer),&pointer)/180)*32767);//*182);
    									}
    									catch(exception e)
    									{
    										continue;
    									}
    									temp.push_back(dih);

    							  }
    					  }
    					  f.close();
    					  sequences.push_back(temp);
    					  temp.clear();
    					  }
    				  else
    					  cout<<"The file couldn't be opened"<<endl;

    				  }
    		  }
    		  closedir (dir);
    		} else {
    		  perror ("");
    		  cout<<"Directory not found"<<endl;
    		}
    }


};


int main()
{
	//parameters that needs to be optimized
	int gap=-10000;
	int K=25000;

	double ab=0; int numab=0;
	double aa=0; int numaa=0;
    std::vector<std::string> names;
    std::vector<std::string> groups;


	bool good_results=true;
    input inp;

    inp.addelements("/home/pasfl/Downloads/Re Szakdolgozat/pdz_bbm/","A", names, groups);
    inp.addelements("/home/pasfl/Downloads/Re Szakdolgozat/sh3_bbm/","B", names, groups);

    ofstream output;
    output.open("/home/pasfl/fpga/Smith_Watermann_4/src/output2.txt");
    int first_group=0;  //index of the start of the first group
    int second_group=0; //index of the start of the second group
    int act_first_group=0;
    int act_second_group=0;
    cout<<"seqs "<<inp.sequences.size()<<endl;
    int comparisons=(((inp.sequences.size()+Seq_in_one_group-1)/Seq_in_one_group+1)*((inp.sequences.size()+Seq_in_one_group-1)/Seq_in_one_group)/2);//csoportokból 2 minden kombinációban
    cout<<"Comparisons :"<<comparisons<<endl;

    for(int j=0;j<comparisons;j++)
    {
        vector<vector<dihedral_angle> > group1(Seq_in_one_group);
        vector<vector<dihedral_angle> > group2(Seq_in_one_group);
        //the number of sequences in the actual group and maximum of the sequence lengths
        int seq_in_actual_1=0;
        int seq_in_actual_2=0;
        int max_length_act_1=0;
        int max_length_act_2=0;
        //creating the groups
       for(int k=0;k<Seq_in_one_group;k++)
        {
            if(first_group+k<inp.sequences.size())
            {
                group1[k]=inp.sequences[first_group+k];
                seq_in_actual_1++;
                if(group1[k].size()>max_length_act_1)
                {
                    max_length_act_1=group1[k].size();
                }
            }
        }
        for(int l=0;l<Seq_in_one_group;l++)
        {
            if(second_group<inp.sequences.size())
            {
                group2[l]=inp.sequences[second_group];
                second_group++;
                seq_in_actual_2++;
                if(group2[l].size()>max_length_act_2)
                {
                    max_length_act_2=group2[l].size();
                }
            }
        }
        if(second_group>=inp.sequences.size())
        {
            first_group+=Seq_in_one_group;
            second_group=first_group;
        }


    resulttype maxs[Seq_in_one_group*Seq_in_one_group];
    int lengths1 [Seq_in_one_group];
    int lengths2 [Seq_in_one_group];
    dihedral_angle gr1[Seq_in_one_group*Max_seq_len];
    dihedral_angle gr2[Seq_in_one_group*Max_seq_len];
//initialization of the arrays passed to the top level function
    for(int init=0;init<seq_in_actual_1;init++)
    {
        lengths1[init]=group1[init].size();
        if(group2.size()>init)
        {
        lengths2[init]=group2[init].size();
        }
        else
        {
            lengths2[init]=0;
        }


    }
    int summ=0;
    for(int a=0;a<group1.size();a++)
    {
        for(int b=0;b<group1[a].size();b++)
        {
            gr1[summ]=group1[a][b];
            summ++;
        }
    }
    summ=0;
    for(int a=0;a<group2.size();a++)
    {
        for(int b=0;b<group2[a].size();b++)
        {
            gr2[summ]=group2[a][b];
            summ++;
        }
    }
    //call of top level function
    S_W_hw(seq_in_actual_1,max_length_act_1,max_length_act_2,maxs,lengths1,lengths2,gr1,gr2,K,gap);
    for(int i=0;i<16;i++)
    {
    	std::cout<<maxs[i]<<std::endl;
    }


    //check whether the results are correct
    cell zero_init;
    zero_init.value=0;
    zero_init.horizontal_gap=0;
    zero_init.vertical_gap=0;
    vector<vector<vector<cell> > > result(Seq_in_one_group*Seq_in_one_group, vector<vector<cell> >(Max_seq_len+1, vector<cell>(Max_seq_len+1,zero_init)));
    vector<int> maximums (Seq_in_one_group*Seq_in_one_group,0);
    int iterations=seq_in_actual_1*seq_in_actual_2*max_length_act_2*max_length_act_1;//egy csoportbontáson belül a bázispárösszehasonlítások

    int seq1_pos=1;int seq2_pos=1;//0. '-'
    int group1_pos=0;int group2_pos=0;

    for(int i=0;i<iterations;i++)
    {
        if(seq1_pos>group1[group1_pos].size()||seq2_pos>group2[group2_pos].size()) //szekvencia vége
        {
            result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos][seq2_pos].value=0;
        }
        else
        {

        Smith_Waterman(result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos][seq2_pos],
        				result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos-1][seq2_pos],
                        result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos][seq2_pos-1],
                        result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos-1][seq2_pos-1],
                        group1[group1_pos][seq1_pos-1],group2[group2_pos][seq2_pos-1],
						K,gap);//-1 mert az elsõ "-", 1 tõl indexelem
            if(result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos][seq2_pos].value>maximums[group1_pos*Seq_in_one_group+group2_pos])
            {
             maximums[group1_pos*Seq_in_one_group+group2_pos]=result[group1_pos*Seq_in_one_group+group2_pos][seq1_pos][seq2_pos].value;
            }
        }

        group1_pos++;

        if(group1_pos==seq_in_actual_1)
        {
            group1_pos=0;
            group2_pos++;
            if(group2_pos==seq_in_actual_2)
            {
                group2_pos=0;
                seq1_pos++;
                if(seq1_pos==max_length_act_1+1)
                {
                    seq1_pos=1;
                    seq2_pos++;
                }
            }
        }
        if(seq2_pos==max_length_act_2+1)//vége a csoportnak, kiírás
        {
            for(int s1=0;s1<seq_in_actual_1;s1++)
            {
                for(int s2=0;s2<seq_in_actual_2;s2++)
                {

                	if(maximums[s1*Seq_in_one_group+s2]!=maxs[s1*Seq_in_one_group+s2])
                	{
                		good_results=false;
                	}

                	int Mmax;
                	if(group1[s1].size()<group2[s2].size())
                			Mmax=group1[s1].size();
                	else
                		Mmax=group2[s2].size();
                	float similarity=maximums[s1*Seq_in_one_group+s2]/Mmax;
                	similarity=similarity/K;
                	if(names.size()>s1+act_first_group&&names.size()>s2+act_second_group&&(act_first_group!=act_second_group||s1<=s2))
                	{
                		//output<<names[s1+act_first_group]<<" "<<names[s2+act_second_group]<< "  Points:"<<maximums[s1*Seq_in_one_group+s2] <<"  Similarity: "<<similarity*100<<std::endl;
                		output<<groups[s1+act_first_group]<<" "<<groups[s2+act_second_group]<< "  Points:"<<maximums[s1*Seq_in_one_group+s2] <<"  Similarity: "<<similarity*100<<std::endl;

                						if(groups[s1+act_first_group]==groups[s2+act_second_group])
                		                    {
                		                    	aa=aa+similarity;
                		                    	numaa++;
                		                    }
                		                    else
                							{

                		                    		ab=ab+similarity;
                		                    		numab++;

                							}

                	}

                	//COMMENT
                    //writes out the whole matrix
                	/*
                    output<<"-\t";
                    for(int b1=0;b1<Max_seq_len;b1++)
                    {
                        if(b1<group2[s2].size())
                            output<<group2[s2][b1].psi<<"\t";
                        else
                           output<<"-\t";
                    }
                    output<<std::endl;
                    for(int a=1;a<result[s1*Seq_in_one_group+s2].size();a++)
                    {
                        if(a-1<group1[s1].size())
                            output<<group1[s1][a-1].psi<<"\t";
                        else
                            output<<"-\t";
                        for(int b=1;b<result[s1*Seq_in_one_group+s2][a].size();b++)
                        {
                            output<<result[s1*Seq_in_one_group+s2][a][b].value<<"\t";
                        }
                        output<<std::endl;

                    }
					*/
                    //COMMENT
                }
            }
            act_first_group=first_group;
            act_second_group=second_group;

            break;

        }
    }
    }
    output.close();
    cout<<"AA: "<<(double)aa*100/numaa<<endl;
        cout<<"AB/BA: "<<(double)ab*100/numab<<endl;


    if(good_results)
    {
    	cout<<"Good result"<<endl;
        return 0;
    }
    else
    {
    	cout<<"Error"<<endl;
    	return 1;
    }


}
//https://0xstubs.org/measuring-time-in-a-bare-metal-zynq-application/