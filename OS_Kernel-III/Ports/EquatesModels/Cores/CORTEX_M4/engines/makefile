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
#
#   Developed for use with Sysquake Embedded.
#   Sysquake is a trademark of Calerga Sarl, used with permission. 
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

PATH_COMPILER	=  $(PATH_GCC_CORTEX3)

LIB_LME			=  ./lme/liblme.a
LIB_MPY			=  ./mpy/libmpy.a
LIB_OUT			=   libengi.a

# Target & Infrastructure

PREFIX			=  arm-none-eabi-

# Rules

AR				=  $(PATH_COMPILER)/bin/$(PREFIX)ar

all :
	make -f ./makefile clr_all
	make -f ./makefile extract
	make -f ./makefile archive
	make -f ./makefile clr

clean :
	make -f ./makefile clr

clean_all :
	make -f ./makefile clr

# Clean

clr_all :
	rm -f *.o *.a

clr :
	rm -f *.o

# Extract / Archive

extract : 
	$(AR) -x $(LIB_LME)
	$(AR) -x $(LIB_MPY) 

archive :
	$(AR) rcs ./$(LIB_OUT) *.o

