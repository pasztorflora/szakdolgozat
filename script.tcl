############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project Smith_Watermann_final
set_top S_W_hw
add_files Smith_Watermann_final/src/S_W_hw.cpp
add_files Smith_Watermann_final/src/S_W_hw.h
add_files Smith_Watermann_final/src/Smith_Watermann.cpp
add_files Smith_Watermann_final/src/Smith_Watermann.h
add_files -tb Smith_Watermann_final/src/testbench.cpp
open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
source "./Smith_Watermann_final/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
