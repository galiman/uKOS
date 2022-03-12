#!/bin/bash
# usage: sh build.sh

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2011-01-05
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 185                  $:  Revision of last commit
# $Date:: 2017-08-17 15:20:55#$:  Date of last commit
#
# Project:	uKOS
# Goal:		Building all the projects.
#
#			usage:
#			sh build.sh
#
#   (c) 1992-2017, Franzi Edo.
#   --------------------------
#                                              __ ______  _____
#   Franzi Edo.                         __  __/ //_/ __ \/ ___/
#   5-Route de Cheseaux                / / / / ,< / / / /\__ \
#   CH 1400 Cheseaux-Noréaz           / /_/ / /| / /_/ /___/ /
#                                     \__,_/_/ |_\____//____/
#   edo.franzi@ukos.ch
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------------------------------------------------

PATH_PRG=${PATH_UKOS_KERNEL}/Applications/uKOS_Appls

if [ -z "$PATH_UKOS_KERNEL" ]; then
	echo "Variable PATH_UKOS_KERNEL is not set!" 
	exit 1
fi

if [ -z "$PATH_TOOLS_UNIX" ]; then
	echo "Variable PATH_TOOLS_UNIX is not set!"
	exit 1
fi

if [ -z "$PATH_GCC_CORTEX3" ]; then
	echo "Variable PATH_GCC_CORTEX3 is not set! Cortex-M3 targets will not build!"
	sleep 4
fi

if [ -z "$PATH_GCC_CORTEX7" ]; then
	echo "Variable PATH_GCC_CORTEX7 is not set! Cortex-M7 targets will not build!"
	sleep 4
fi

if [ -z "$PATH_GCC_ICYFLEX" ]; then
	echo "Variable PATH_GCC_ICYFLEX is not set! IcyFlex targets will not build!"
	sleep 4
fi

TAB=( 

#	Applications & Variant_1 Targets

	"asso_sema"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"asso_smpl"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"cmpl_stdl"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"debg_stop"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"evnt_cale"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"evnt_proc"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"intr_io"		"-"					"Discovery_407"		"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"	
	"intr_timer"	"-"					"Discovery_407"		"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"mbox_bidm"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"mbox_unim"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"mbox_unis"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_adco"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"-"					"-"					"-"					"-"					"-"					"-"					"-"				"-"
	"misc_basic"	"-"					"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"CSEM_VIP_777"	
	"misc_cordic"	"-"					"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_dsp"		"-"					"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_fpu"		"-"					"Discovery_407"		"Discovery_429"		"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_imgCont"	"-"					"-"					"-"					"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_imgSnap"	"-"					"-"					"-"					"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_lcd"		"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"				"-"			
	"misc_prtn"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_sdcard"	"-"					"Discovery_407"		"Discovery_429"		"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"
	"misc_speed"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_spyImg"	"-"					"-"					"-"					"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_suic"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_temp"		"-"					"-"					"-"					"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_trace"	"-"					"Discovery_407"		"Discovery_429"		"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_ttac"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"misc_txrx"		"-"					"-"					"-"					"-"					"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"
	"queue_unim"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"queue_unis"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_driv"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_memo"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_mtex"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_mult"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_smpl"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sema_tout"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sign_basic"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sign_multi"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sqe_kalman"	"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sqe_multi"		"-"					"-"					"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sqe_random"	"-"					"-"					"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"sqe_separe"	"-"					"-"					"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"stim_basic"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"syst_bench"	"-"					"-"					"-"					"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"
	"test_cntr"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_conc"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_except"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_malloc"	"-"					"-"					"Discovery_429"		"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_imu"		"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_jpeg"		"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"				"CSEM_VIP_777"
	"test_pool"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_precise"	"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_rati"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"test_RTC"		"-"					"-"					"-"					"Discovery_476"		"-"					"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"	
	"test_swtc"		"LAP_icycam_103"	"Discovery_407"		"Discovery_429"		"Discovery_476"		"Baphomet_407"		"Baphomet_429"		"Baphomet_746"		"CSEM_icycam"		"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	"uart_xchange"	"-"					"-"					"-"					"-"					"-"					"Baphomet_429"		"Baphomet_746"		"-"					"-"					"-"				"-"	
	"xfer_img"		"-"					"-"					"-"					"-"					"-"					"-"					"-"					"-"					"CSEM_VIP_429"		"CSEM_VIP_746"	"-"	
	)

SIZE_TAB=${#TAB[@]}
let "structure = 12"
let "nbElements = SIZE_TAB/structure"

build_failure=""
build_success=""

echo "Building all ..."
for (( h = 0 ; h < nbElements ; h++ ))
	do
		let "project = h * structure"
		for (( TARGET = 1 ; TARGET < structure ; TARGET++ ))
			do
				let "index = project + TARGET"
				if [ ${TAB[$index]} != "-" ]; then
					cd ${PATH_PRG}/${TAB[$project]}/${TAB[$index]}
					make clean_all
					make && build_success=$build_success"\n"${TAB[$project]}/${TAB[$index]} || build_failure=$build_failure"\n"${TAB[$project]}/${TAB[$index]}
					make clean
				fi
			done
	done

# Display the target list (successfully built and that have failed)
# only if one or more target failed

if [ ! -z "$build_success" ] ; then
	shopt -s xpg_echo
	echo "\nSuccessful build: " $build_success
fi

if [ ! -z "$build_failure" ] ; then
	shopt -s xpg_echo
	echo "\nFailed build: " $build_failure
fi

echo "Terminated!"
