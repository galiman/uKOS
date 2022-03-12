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
# Goal:		makefile for uKOS for building the libengi.a module.
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
ifndef PATH_GCC_CORTEX7
    $(error PATH_GCC_CORTEX7 is not defined)
endif

LIB_IN			=  microythonM7.aa
LIB_OUT			=  libmpy.a
OBJ				=  mpyt.o

# Project paths

# - PATH_UKOS	--> Main uKOS folder					--> OS_Kernel-III
# - PATH_PORT	--> Main uKOS port folders				--> OS_Kernel-III/Ports
# - PATH_BASE	--> MyProjects Root Base folders		--> MyProjects/Ports/Targets/xyz/Base
# - PATH_VARI	--> MyProjects Root Variant_xx folders	--> MyProjects/Ports/Targets/xyz/Variant_xx
# - PATH_MAPP	--> Linker script folder				--> MyProjects/Ports/Targets/xyz/...Base or Variant_xx
# - PATH_USER	--> User project folder					--> MyProjects/xyz/Projects

PATH_UKOS		=  $(PATH_UKOS_KERNEL)
PATH_PORT		=  $(PATH_UKOS_KERNEL)/Ports

PATH_INCLUDES	=  -I$(PATH_UKOS)
PATH_INCLUDES	+= -I$(PATH_UKOS)/Includes
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_tbox
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_comm
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_gene
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_kern
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_engi

PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/py
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/uKOS
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/uKOS/build
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/uKOS/build/genhdr
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/lib
PATH_INCLUDES	+= -I$(PATH_UKOS_KERNEL)/Ports/Third_Party/micropython-master/lib/utils

PATH_INCLUDES	+= -I$(PATH_UKOS)/Ports/EquatesModels/Cores/$(CORE)/engines/mpy/Construction

# Target & Infrastructure

PREFIX			=  arm-none-eabi-
CORE			=  CORTEX_M7
STANDARD		=  -std=c99
ISO_IEC_9899	=  -pedantic -ansi
OPTIMISATION	=  -Os

PATH_COMPILER	=  $(PATH_GCC_CORTEX7)
PATH_LIB_IN		=  $(PATH_PORT)/EquatesModels/Cores/$(CORE)/engines/mpy/Construction/uKOS/build

FLAGS_UKOS		=  -D__UKOS__ -D__$(CORE)__ -D__LITTLE_ENDIAN__

FLAGS_CC		=  -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mthumb -fsingle-precision-constant
FLAGS_CC		+= -Wall -c -g $(ISO_IEC_9899) $(OPTIMISATION) -Wlogical-op
FLAGS_CC		+= $(STANDARD)
FLAGS_CC		+= $(FLAGS_UKOS)
FLAGS_CC		+= $(PATH_INCLUDES)
FLAGS_CC		+= -MMD $(addprefix -I,$(INCLUDE))

include	$(PATH_PORT)/Mkfiles/engine.mk
include $(wildcard *.d)
