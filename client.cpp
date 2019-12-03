#include <malloc.h>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <iostream>
 #include <cstdlib> 
 #include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 5001   //The port on which to listen for incoming data
#define SERVER "192.168.1.10"

using namespace std;

struct dihedral_angle
{
	short phi,psi;
};

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
    					  cout<<"The file couldn't be opened"<<s+name<<endl;

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
   // int Seq_in_one_group=4;
   int Seq_in_one_group=4;
    int Max_seq_len=256;
    struct sockaddr_in si_me, si_other;
    int s;
    char *buff,*recv_buff;
    int i,n;
    int bytes_left;
    std::vector<std::string> names;
    std::vector<std::string> groups;
//LOADING DATA
    input inp;
    inp.addelements("/home/pasfl/Downloads/Re Szakdolgozat/pdz_bbm/","A", names, groups);
    inp.addelements("/home/pasfl/Downloads/Re Szakdolgozat/sh3_bbm/","B", names, groups);
    //inp.addelements("/home/pasfl/fpga/Smith_Watermann_4/src/group1/","A", names, groups);
    //inp.addelements("/home/pasfl/fpga/Smith_Watermann_4/src/group2/","B", names, groups);
    //inp.addelements("/home/pasfl/Downloads/lipocalin_bbm/","A", names, groups);
    //inp.addelements("/home/pasfl/Downloads/globin_bbm/","B", names, groups);
    //inp.addelements("/home/pasfl/fpga/myresults/groupa/","A", names, groups);
    //inp.addelements("/home/pasfl/fpga/myresults/groupb/","B", names, groups);
    


    int num_of_seqs=inp.sequences.size();
    int * sizes=(int*)memalign(32, 2*sizeof(int));
    sizes[0]=num_of_seqs+Seq_in_one_group;
    sizes[1]=Max_seq_len*Seq_in_one_group;
    for(int i=0;i<num_of_seqs;i++)
    {
        sizes[1]+=inp.sequences[i].size();
    }

    dihedral_angle *proteins=(dihedral_angle*)memalign(32,sizes[1]*sizeof(dihedral_angle));
    int * lengths=(int*)memalign(32,sizes[0]*sizeof(int));

    for(int i=0;i<num_of_seqs;i++)
    {
        lengths[i]=inp.sequences[i].size();
    }
    for(int i=0;i<Seq_in_one_group;i++)
    {
        lengths[i+num_of_seqs]=0;
    }
    dihedral_angle * helper=proteins;
    for(int i=0;i<num_of_seqs;i++)
    {
        for(int j=0;j<inp.sequences[i].size();j++)
        {
            helper->phi=inp.sequences[i][j].phi;
            helper->psi=inp.sequences[i][j].psi;
            helper++;
        }
    } 
//END
//SOCKET 
    //create a TCP socket
    s=socket(AF_INET, SOCK_STREAM, 0);
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    inet_aton(SERVER , &si_other.sin_addr);
    //bind socket to port
    connect(s,(struct sockaddr*)&si_other,sizeof(si_other));
//END
//SENDING  
    //SIZES
    bytes_left = 2*sizeof(int);
    buff=(char *)sizes;
    while (bytes_left > 0)
    {
        n=send(s, buff, bytes_left, 0);
        bytes_left -= n;
        buff+= n;
    }
    //LENGTHS
    bytes_left = sizes[0]*sizeof(int);
    buff=(char *)lengths;
    while (bytes_left > 0)
    {
        n=send(s, buff, bytes_left, 0);
        bytes_left -= n;
        buff+= n;
    }
    //SEQUENCES
    bytes_left = sizes[1]*sizeof(dihedral_angle);
    buff=(char *)proteins;
    while (bytes_left > 0)
    {
        n=send(s, buff, bytes_left, 0);
        bytes_left -= n;
        buff+= n;
    }
//END

    //NEW
    int comparisons=(((num_of_seqs+Seq_in_one_group-1)/Seq_in_one_group+1)*((num_of_seqs+Seq_in_one_group-1)/Seq_in_one_group)/2);
    int* results=(int*)memalign(32,Seq_in_one_group*Seq_in_one_group*comparisons*sizeof(int));
    int Ks=1;
    int gaps=1;
    ofstream output;
    output.open("/home/pasfl/fpga/myresults/result2.txt");
    int K=29000;
   
    for(int it=0;it<Ks;it++)
    {
        K+=1000;
        int gap=-29000;
        for(int it2=0;it2<gaps;it2++)
        {
            gap -=1000;
                      
            int aa[101];
            int bb[101];
            int ab[101];
            for(int i=0;i<101;i++)
            {
            aa[i]=0;
            bb[i]=0;
            ab[i]=0;
            }
            
        bytes_left = Seq_in_one_group*Seq_in_one_group*comparisons*sizeof(int);
        recv_buff=(char *)results;
        while (bytes_left > 0)
        {

            n=recv(s,recv_buff,bytes_left, 0);
            bytes_left -= n;
            recv_buff+= n;
        }
        
        
        //K, gap    
        int first_group=0;
        int second_group=0;
        int act_first_group=0;
        int act_second_group=0;

        for(int comp=0;comp<comparisons;comp++)
        {
           
            int seq_in_actual_1=0;
            int seq_in_actual_2=0;
           
            for(int k=0;k<Seq_in_one_group;k++)
            {
                if(first_group+k<inp.sequences.size())
                {
                    seq_in_actual_1++;
                }
            }
            for(int l=0;l<Seq_in_one_group;l++)
            {
                if(second_group<inp.sequences.size())
                {
                    second_group++;
                    seq_in_actual_2++;
                }
            }
            if(second_group>=inp.sequences.size())
            {
                first_group+=Seq_in_one_group;
                second_group=first_group;
            }
            for(int s1=0;s1<seq_in_actual_1;s1++)
            {
                for(int s2=0;s2<seq_in_actual_2;s2++)
                {
                    if(num_of_seqs>s1+act_first_group&&num_of_seqs>s2+act_second_group&&(act_first_group!=act_second_group||s1<s2))
                    {
                        int Mmax;
                        if(lengths[s1+act_first_group]<lengths[s2+act_second_group])
                                Mmax=lengths[s1+act_first_group];
                        else
                            Mmax=lengths[s2+act_second_group];

                        int similarity=results[comp*Seq_in_one_group*Seq_in_one_group+s1*Seq_in_one_group+s2]*100/(Mmax*K);

                        if(groups[s1+act_first_group]==groups[s2+act_second_group])
                        {
                            
                            if(groups[s1+act_first_group]=="A")
                                {
                                    aa[similarity]++;

                                }
                            else
                            {
                                bb[similarity]++;

                            }
                            //same[similarity]++;
                           
                        }
                        else
                        {
                                ab[similarity]++;
                                
                        }

                    }
                }
            }
            act_first_group=first_group;
            act_second_group=second_group;
        }
        output<<"K: "<<K<<" gap: "<<gap<<endl;
        for(int i=0;i<101;i++)
        {
            output<<aa[i]<<endl;
        }
           for(int i=0;i<101;i++)
        {
            output<<bb[i]<<endl;
        }
        for(int i=0;i<101;i++)
        {
            output<<ab[i]<<endl;
        }
        
        }

    }
    output.close();   
    //END
    
    return 0;    
}


