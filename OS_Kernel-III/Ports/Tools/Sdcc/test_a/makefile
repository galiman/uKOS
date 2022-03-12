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
# Goal:		makefile for the "test_a" application.
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

ifndef PATH_GCC_GPUTILS
    $(error PATH_GCC_GPUTILS is not defined)
endif
ifndef PATH_TOOLS_UNIX
    $(error PATH_TOOLS_UNIX is not defined)
endif

SOC				= pic16f630
PROBE			= no

PATH_COMPILER	= $(PATH_GCC_GPUTILS)

AS				= $(PATH_COMPILER)/bin/gpasm
PK				= $(PATH_TOOLS_UNIX)/pk2cmd

all :
	make -f ./makefile TARGET=test_a clr_all
	make -f ./makefile TARGET=test_a build
	make -f ./makefile TARGET=test_a clr
	make -f ./makefile TARGET=test_a download

clean :
	make -f ./makefile TARGET=test_a clr_all

clean_all :
	make -f ./makefile TARGET=test_a clr_all

# Clean

clr :
	rm -f *.d *.elf *.lst *.o *.map *.cod

clr_all :
	rm -f *.d *.elf *.lst *.map *.cod *.hex *.o *.a *.s2 *.s3 *.bin *.xml

# Build

build : $(TARGET).hex download

$(TARGET).hex : $(TARGET).asm makefile
	$(AS) -p$(SOC) $(TARGET).asm

# Download

download :
ifeq "$(PROBE)" "yes"
	$(PK) -P -F$(TARGET).hex -M
	$(PK) -P -T
endif
