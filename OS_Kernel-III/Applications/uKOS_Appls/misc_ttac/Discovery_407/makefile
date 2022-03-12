# makefile.
# =========

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2006-06-28
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 164                  $:  Revision of last commit
# $Date:: 2017-06-28 12:06:53#$:  Date of last commit
#
# Project:	uKOS
# Goal:		makefile for the "misc_ttac" application.
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

SHELL			=  /bin/sh

ifndef PATH_UKOS_KERNEL
    $(error PATH_UKOS_KERNEL is not defined)
endif
ifndef PATH_GCC_CORTEX3
    $(error PATH_GCC_CORTEX3 is not defined)
endif
ifndef PATH_TOOLS_UNIX
    $(error PATH_TOOLS_UNIX is not defined)
endif

PROJECT			=  misc_ttac

OBJ				=  header.o crt0_App.o
OBJ				+= $(PROJECT).o

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

PREFIX			=  arm-none-eabi-
BOARD			=  Discovery_407
V				=  Variant_Test
SOC				=  STM32F407
CORE			=  CORTEX_M4
STANDARD		=  -std=c99
ISO_IEC_9899	=  -pedantic -ansi
OPTIMISATION	=  -Os

include	$(PATH_PORT)/Mkfiles/application.mk
include	$(PATH_PORT)/Mkfiles/application_$(CORE).mk
include $(wildcard *.d)
