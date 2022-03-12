# makefile.
# =========

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2006-06-28
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 185                  $:  Revision of last commit
# $Date:: 2017-08-17 15:20:55#$:  Date of last commit
#
# Project:	uKOS
# Goal:		makefile for building the uKOS system.
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

SHELL			= /bin/sh

ifndef PATH_UKOS_KERNEL
    $(error PATH_UKOS_KERNEL is not defined)
endif
ifndef PATH_GCC_CORTEX3
    $(error PATH_GCC_CORTEX3 is not defined)
endif
ifndef PATH_TOOLS_UNIX
    $(error PATH_TOOLS_UNIX is not defined)
endif

# uKOS System description
# -----------------------

# Project paths

# - PATH_UKOS	--> Main uKOS folder					--> OS_Kernel-III
# - PATH_PORT	--> Main uKOS port folders				--> OS_Kernel-III/Ports
# - PATH_BASE	--> MyProjects Root Base folders		--> MyProjects/Ports/Targets/xyz/Base
# - PATH_VARI	--> MyProjects Root Variant_xx folders	--> MyProjects/Ports/Targets/xyz/Variant_xx
# - PATH_MAPP	--> Linker script folder				--> MyProjects/Ports/Targets/xyz/...Base or Variant_xx
# - PATH_USER	--> User project folder					--> MyProjects/xyz/Projects

PATH_UKOS		=  $(PATH_UKOS_KERNEL)
PATH_PORT		=  $(PATH_UKOS_KERNEL)/Ports
PATH_BASE		=  $(PATH_UKOS_KERNEL)/Ports/Targets/$(BOARD)/Base
PATH_VARI		=  $(PATH_UKOS_KERNEL)/Ports/Targets/$(BOARD)/$(V)
PATH_MAPP		=  $(PATH_BASE)/Runtime

PATH_INCLUDES	=  -I$(PATH_UKOS)

# Target & Infrastructure

# - VERSIONING	--> SVN
# - VERSIONING	--> GIT
# - VERSIONING	--> NONE

PREFIX			=  arm-none-eabi-
BOARD			=  LAP_icycam_103
V				=  Variant_Test
SOC				=  STM32F103
CORE			=  CORTEX_M3
STANDARD		=  -std=c99
ISO_IEC_9899	=  -pedantic -ansi
OPTIMISATION	=  -Os
VERSIONING		=  SVN

# The runtime & commun components

RTCB			=												$(PATH_PORT)/EquatesModels/Cores/$(CORE)/Runtime/crt0.o
RTCB			+=												$(PATH_VARI)/Runtime/init.o
RTCB			+=												$(PATH_BASE)/Runtime/exce.o
RTCB			+=												$(PATH_BASE)/Runtime/cmns.o
RTCB			+= $(PATH_UKOS)/Boots/boot/boot.o
RTCB_HIDE		=												$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/Runtime/first.o
RTCB_HIDE		+=												$(PATH_VARI)/Runtime/pbus.o

# The uKernel components

KERN			=  $(PATH_UKOS)/Lib_kern/kern/kern.o
KERN_HIDE		=  $(PATH_UKOS)/Lib_kern/kern/mbox.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/sema.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/chls.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/asso.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/evnt.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/sign.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/temp.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/desc.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/daem.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/ovly.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/sche.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/stat.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/debg.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/stim.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/pool.o
KERN_HIDE		+= $(PATH_UKOS)/Lib_kern/kern/buff.o
KERN_HIDE		+=												$(PATH_BASE)/Lib_kern/kern/stub_kern_kernel.o

# The Lib_xxxx components

LIBX			=  $(PATH_UKOS)/Lib_tbox/strg/strg.o
LIBX			+= $(PATH_UKOS)/Lib_tbox/misc/misc.o
LIBX			+= $(PATH_UKOS)/Lib_tbox/glob/glob.o
LIBX			+= $(PATH_UKOS)/Lib_peri/adc0/adc0.o
LIBX			+= $(PATH_UKOS)/Lib_gene/iotx/iotx.o
LIBX			+= $(PATH_UKOS)/Lib_gene/syos/syos.o
LIBX			+= $(PATH_UKOS)/Lib_gene/text/text.o
LIBX			+= 												$(PATH_BASE)/Lib_comm/comm/comm.o
LIBX			+= $(PATH_UKOS)/Lib_comm/urt0/urt0.o
LIBX			+= $(PATH_UKOS)/Lib_nlib/nlib/nlib.o
LIBX_HIDE		=												$(PATH_VARI)/Lib_tbox/misc/stub_misc.o
LIBX_HIDE		+= 												$(PATH_BASE)/Lib_tbox/glob/stub_glob.o
LIBX_HIDE		+= 												$(PATH_BASE)/Lib_peri/adc0/stub_adc0.o
LIBX_HIDE		+= 												$(PATH_BASE)/Lib_gene/syos/stub_syos.o
LIBX_HIDE		+= 												$(PATH_BASE)/Lib_comm/urt0/stub_urt0_usart1.o

# The shared components

SHAR			=
SHAR_HIDE		=

# The process components

PROC			=  $(PATH_UKOS)/Processes/alive/alive.o
PROC			+= $(PATH_UKOS)/Processes/start_up/start_up.o
PROC_HIDE		=  												$(PATH_BASE)/Processes/alive/stub_alive.o
PROC_HIDE		+= 												$(PATH_VARI)/Processes/start_up/stub_start_up.o

# The tool components

TOOL			=  $(PATH_UKOS)/Tools/bangla/BaNgLa.o
TOOL			+= $(PATH_UKOS)/Tools/binfill/binfill.o
TOOL			+= $(PATH_UKOS)/Tools/console/console.o
TOOL			+= $(PATH_UKOS)/Tools/cycle/cycle.o
TOOL			+= $(PATH_UKOS)/Tools/dump/dump.o
TOOL			+= $(PATH_UKOS)/Tools/dumptrace/dumptrace.o
TOOL			+= $(PATH_UKOS)/Tools/fill/fill.o
TOOL			+= $(PATH_UKOS)/Tools/hexloader/hexloader.o
TOOL			+= $(PATH_UKOS)/Tools/list/list.o
TOOL			+= $(PATH_UKOS)/Tools/man/man.o
TOOL			+= $(PATH_UKOS)/Tools/memory/memory.o
TOOL			+= $(PATH_UKOS)/Tools/pdp11loader/pdp11loader.o
TOOL			+= $(PATH_UKOS)/Tools/process/process.o
TOOL			+= $(PATH_UKOS)/Tools/restart/restart.o
TOOL			+= $(PATH_UKOS)/Tools/run/run.o
TOOL			+= $(PATH_UKOS)/Tools/semaphore/semaphore.o
TOOL			+= $(PATH_UKOS)/Tools/sloader/sloader.o
TOOL			+= $(PATH_UKOS)/Tools/uart0/uart0.o
TOOL			+= $(PATH_UKOS)/Tools/uKOS/uKOS.o
TOOL_HIDE		=

# The protocol components

PROT			=  $(PATH_UKOS)/Protocols/B/B.o
PROT			+= $(PATH_UKOS)/Protocols/L/L.o
PROT_HIDE		=

# All the uKOS librairies components

UKOS_COMPONENTS	= librtcb.a liblibx.a libproc.a libtool.a libprot.a libkern.a

# The included makefiles

include	$(PATH_PORT)/Mkfiles/system.mk
include	$(PATH_PORT)/Mkfiles/system_$(CORE).mk
include $(wildcard *.d)

