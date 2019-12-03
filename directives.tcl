############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode s_axilite "S_W_hw"
set_directive_interface -mode s_axilite "S_W_hw" K
set_directive_interface -mode s_axilite "S_W_hw" gap
set_directive_interface -mode s_axilite "S_W_hw" L1
set_directive_interface -mode s_axilite "S_W_hw" N
set_directive_interface -mode s_axilite "S_W_hw" L2
set_directive_interface -mode m_axi -depth 1024 -offset slave -bundle readwrite -num_read_outstanding 8 -num_write_outstanding 8 -max_read_burst_length 64 -max_write_burst_length 64 "S_W_hw" input_maxs
set_directive_interface -mode m_axi -depth 1024 -offset slave -bundle readwrite -num_read_outstanding 8 -num_write_outstanding 8 -max_read_burst_length 64 -max_write_burst_length 64 "S_W_hw" input_lengths1
set_directive_interface -mode m_axi -depth 1024 -offset slave -bundle read1 -num_read_outstanding 8 -max_read_burst_length 64 "S_W_hw" input_lengths2
set_directive_interface -mode m_axi -depth 1024 -offset slave -bundle read2 -num_read_outstanding 8 -max_read_burst_length 64 "S_W_hw" input_group1
set_directive_interface -mode m_axi -depth 1024 -offset slave -bundle read3 -num_read_outstanding 8 -max_read_burst_length 64 "S_W_hw" input_group2
set_directive_array_partition -type complete -dim 1 "S_W_hw" prev
set_directive_array_partition -type complete -dim 1 "S_W_hw" top
set_directive_array_partition -type complete -dim 1 "S_W_hw" left
set_directive_loop_merge "S_W_hw/ZERO"
set_directive_pipeline "S_W_hw/INIT"
set_directive_pipeline "S_W_hw/INIT_TEMP"
set_directive_loop_merge "S_W_hw/LOADING"
set_directive_pipeline "S_W_hw/INIT_MAXS"
set_directive_pipeline "S_W_hw/INIT_GROUP1"
set_directive_pipeline "S_W_hw/INIT_GROUP2"
set_directive_pipeline "S_W_hw/RETURN"
set_directive_pipeline "S_W_hw/wholegroup"
set_directive_dependence -variable prev -class array -type inter -dependent false "S_W_hw/wholegroup"
set_directive_dependence -variable top -class array -type inter -dependent false "S_W_hw/wholegroup"
set_directive_dependence -variable left -class array -type inter -dependent false "S_W_hw/wholegroup"
set_directive_dependence -variable temp_results -class array -type inter -dependent false "S_W_hw/wholegroup"
set_directive_data_pack "S_W_hw" actual
set_directive_data_pack "S_W_hw" zero_init
set_directive_dependence -variable maxs -class array -type inter -dependent false "S_W_hw/wholegroup"
set_directive_data_pack "S_W_hw" input_group1
set_directive_data_pack "S_W_hw" input_group2
