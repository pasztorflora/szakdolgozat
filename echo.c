/*
 * Copyright (C) 2016 - 2018 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include "xs_w_hw.h"
#include "xil_cache.h"

#include <stdio.h>
#include <string.h>

#include "lwip/sockets.h"
#include "netif/xadapter.h"
#include "lwipopts.h"
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"

#define THREAD_STACKSIZE 1024

u16_t echo_port = 5001;

struct dihedral_angle
{
        short phi,psi;
};

void print_echo_app_header()
{
    xil_printf("%20s %6d %s\r\n", "echo server",
                        echo_port,
                        "$ telnet <board_ip> 7");

}

/* thread spawned for each connection */
void process_echo_request(void *p)
{
        int sd = (int)p;
        int RECV_BUF_SIZE = 2048;
        char recv_buf[RECV_BUF_SIZE];
        int n, nwrote;
        int bytes_left;
        char* buff_pointer;
        XS_w_hw InstancePtr;
        XS_w_hw_Initialize(&InstancePtr, 0);

        //MYCODE
        int checker1;
        short checker2;
        char* receive_buffer;
        char* send_buffer;
        int Seq_in_one_group=4;
        int Max_seq_len=256;

                int *sizes=(int*)memalign(32,2*sizeof(int));
                //RECEIVING SIZES
                bytes_left = 2*sizeof(int);
                receive_buffer=(char *)sizes;
                while (bytes_left > 0)
                {
                        n=read(sd,receive_buffer,bytes_left);
                        bytes_left -= n;
                        receive_buffer+= n;
                }
                Xil_DCacheFlushRange((INTPTR)sizes,sizeof(int)*2);
                //mycode
                {
                                int* maxs=(int*)memalign(32, Seq_in_one_group*Seq_in_one_group*sizeof(int));
                        int *lengths=(int*)memalign(32, sizes[0]*sizeof(int));
                        //lehet hogy paddelni kell
                        struct dihedral_angle *sequences=(struct dihedral_angle*)memalign(32, sizes[1]*sizeof(struct dihedral_angle));
                        //lengths
                        bytes_left = sizes[0]*sizeof(int);
                        receive_buffer=(char *)lengths;
                        while (bytes_left > 0)
                        {
                                n=read(sd,receive_buffer,bytes_left);
                                bytes_left -= n;
                                receive_buffer+= n;
                        }
                        //sequences
                        bytes_left = sizes[1]*sizeof(struct dihedral_angle);
                        receive_buffer=(char *)sequences;
                        while (bytes_left > 0)
                        {
                                n=read(sd,receive_buffer,bytes_left);
                                bytes_left -= n;
                                receive_buffer+= n;
                        }

                        Xil_DCacheFlushRange((INTPTR)lengths,sizeof(int)*sizes[0]);
                        Xil_DCacheFlushRange((INTPTR)sequences,sizeof(struct dihedral_angle)*sizes[1]);

                        int num_of_seqs=sizes[0]-Seq_in_one_group;
                        int comparisons=(((num_of_seqs+Seq_in_one_group-1)/Seq_in_one_group+1)*((num_of_seqs+Seq_in_one_group-1)/Seq_in_one_group)/2);//csoportokból 2 minden kombinációban
                        int * results=(int*)memalign(32, comparisons*Seq_in_one_group*Seq_in_one_group*sizeof(int));
                        int gaps=1;
                        int* gap=(int*)memalign(32,gaps*sizeof(int));
                        int Ks=1;
                        int* K=(int*)memalign(32,Ks*sizeof(int));
                        for(int it=0;it<Ks;it++)
                        {
                        	K[it]=30000+it*1000;
                        }
                        Xil_DCacheFlushRange((INTPTR)K,sizeof(int)*Ks+31);
                        for(int it=0;it<gaps;it++)
						{
							gap[it]=-30000-it*1000;
						}
						Xil_DCacheFlushRange((INTPTR)gap,sizeof(int)*gaps+31);


                        for(int it=0;it<Ks;it++)
                        {
                        	 for(int it2=0;it2<gaps;it2++)
                        	                        	 {

                            int * result_pointer=results;
                            int first_group=0;  //index of the start of the first group
                            int second_group=0; //index of the start of the second group
                            int sum_lengths=0;
                            int index1=0;
                            int index2=0;
                            for(int j=0;j<comparisons;j++)
                                {

                                    //the number of sequences in the actual group and maximum of the sequence lengths
                                    int seq_in_actual_1=0;
                                    int seq_in_actual_2=0;
                                    int max_length_act_1=0;
                                    int max_length_act_2=0;
                                    struct dihedral_angle* group1=sequences+index1;
                                    struct dihedral_angle* group2=sequences+index2;
                                    int * lengths1=lengths+first_group;
                                    int * lengths2=lengths+second_group;
                                    //creating the groups
                                   for(int k=0;k<Seq_in_one_group;k++)
                                    {
                                        if(first_group+k<num_of_seqs)
                                        {
                                            seq_in_actual_1++;
                                            sum_lengths+=lengths[first_group+k];
                                            if(lengths[first_group+k]>max_length_act_1)
                                            {
                                                max_length_act_1=lengths[first_group+k];
                                            }
                                        }
                                    }
                                   for(int k=0;k<Seq_in_one_group;k++)
								   {
										   if(second_group+k<num_of_seqs)
										   {
												   seq_in_actual_2++;
												   index2+=lengths[second_group+k];
												   if(lengths[second_group+k]>max_length_act_2)
												   {
														   max_length_act_2=lengths[second_group+k];
												   }
										   }
								   }
                                   second_group+=Seq_in_one_group;
                                    if(second_group>=num_of_seqs)
                                    {
                                        first_group+=Seq_in_one_group;
                                        second_group=first_group;
                                        index1=sum_lengths;
                                        index2=index1;

                                    }
                                    sum_lengths=index1;


                                //call of top level function
                               // S_W_hw(seq_in_actual_1,max_length_act_1,max_length_act_2,maxs,lengths1,lengths2,gr1,gr2,K,gap);



                                            Xil_DCacheFlushRange((INTPTR)maxs,sizeof(int)*Seq_in_one_group*Seq_in_one_group+31);
                                            Xil_DCacheInvalidateRange((INTPTR)maxs,sizeof(int)*Seq_in_one_group*Seq_in_one_group+31);



                                                                     XS_w_hw_Set_N(&InstancePtr, seq_in_actual_1);
                                                                     XS_w_hw_Set_L1(&InstancePtr, max_length_act_1);
                                                                     XS_w_hw_Set_L2(&InstancePtr, max_length_act_2);
                                                                     XS_w_hw_Set_input_lengths1(&InstancePtr, (u32)lengths1);
                                                                     XS_w_hw_Set_input_lengths2(&InstancePtr, (u32)lengths2);
                                                                     XS_w_hw_Set_input_group1(&InstancePtr, (u32)group1);
                                                                     XS_w_hw_Set_input_group2(&InstancePtr, (u32)group2);
                                                                     XS_w_hw_Set_input_maxs(&InstancePtr,(u32) maxs);
                                                                     XS_w_hw_Set_K(&InstancePtr, K[it]);
                                                                     XS_w_hw_Set_gap(&InstancePtr, gap[it2]);
                                                                     XS_w_hw_Start(&InstancePtr);
                                                                    while(!XS_w_hw_IsDone(&InstancePtr));
                                                                    Xil_DCacheInvalidateRange((INTPTR)maxs,sizeof(int)*Seq_in_one_group*Seq_in_one_group+31);

                                                                    for(int a=0;a<Seq_in_one_group*Seq_in_one_group;a++)
                                                                    {
                                                                    	*result_pointer=maxs[a];
                                                                    	result_pointer++;
                                                                    }


                                }
                            Xil_DCacheFlushRange((INTPTR)results,sizeof(int)*Seq_in_one_group*Seq_in_one_group*comparisons+31);

    						bytes_left = Seq_in_one_group*Seq_in_one_group*comparisons*sizeof(int);
    						send_buffer=(char *)results;
    						while (bytes_left > 0)
    						{
    										n=write(sd, send_buffer,bytes_left);
    										bytes_left -= n;
    										send_buffer+= n;
    						}
                        }
                }


                        //xil_printf(" %d /r/n", 245);
                        /*
                        bytes_left = sizes[0]*sizeof(int);
                                                send_buffer=(char *)lengths;
                                                while (bytes_left > 0)
                                                {
                                                                n=write(sd, send_buffer,bytes_left);
                                                                bytes_left -= n;
                                                                send_buffer+= n;
                                                }
                                                bytes_left = sizes[1]*sizeof(struct dihedral_angle);
                                                send_buffer=(char *)sequences;
                                                while (bytes_left > 0)
                                                {
                                                                n=write(sd, send_buffer,bytes_left);
                                                                bytes_left -= n;
                                                                send_buffer+= n;
                                                }
                        */
                }
                //endmycode


        /* close connection */
        close(sd);
        vTaskDelete(NULL);
}

void echo_application_thread()
{
        xil_printf("ECHO_APLICATION /r/n");
        int sock, new_sd;
        int size;
#if LWIP_IPV6==0
        struct sockaddr_in address, remote;

        memset(&address, 0, sizeof(address));

        if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
                return;

        address.sin_family = AF_INET;
        address.sin_port = htons(echo_port);
        address.sin_addr.s_addr = INADDR_ANY;
#else
        struct sockaddr_in6 address, remote;

        memset(&address, 0, sizeof(address));

        address.sin6_len = sizeof(address);
        address.sin6_family = AF_INET6;
        address.sin6_port = htons(echo_port);

        memset(&(address.sin6_addr), 0, sizeof(address.sin6_addr));

        if ((sock = lwip_socket(AF_INET6, SOCK_STREAM, 0)) < 0)
                return;
#endif

        if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
                return;

        lwip_listen(sock, 0);

        size = sizeof(remote);

        while (1) {
                if ((new_sd = lwip_accept(sock, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0) {
                        sys_thread_new("echos", process_echo_request,
                                (void*)new_sd,
                                THREAD_STACKSIZE,
                                DEFAULT_THREAD_PRIO);
                }
        }
}
