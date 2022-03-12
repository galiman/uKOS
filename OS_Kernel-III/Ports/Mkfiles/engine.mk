# makefile.
# =========

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2006-06-28
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 148                  $:  Revision of last commit
# $Date:: 2017-06-04 17:55:00#$:  Date of last commit
#
# Project:	uKOS
# Goal:		makefile for uKOS for building the libxyz.a module.
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

# Rules

CC				=  $(PATH_COMPILER)/bin/$(PREFIX)gcc
AR				=  $(PATH_COMPILER)/bin/$(PREFIX)ar

all :
	make -f ./makefile clr_all
	make -f ./makefile extract
	make -f ./makefile build
	make -f ./makefile archive
	make -f ./makefile clr

clean :
	make -f ./makefile clr

clean_all :
	make -f ./makefile clr_all

# Clean

clr :
	rm -f *.d *.o

clr_all :
	rm -f *.d *.o ../*.a

# Extract / Archive

extract : 
	$(AR) -x $(PATH_LIB_IN)/$(LIB_IN)

archive :
	$(AR) rcs ../$(LIB_OUT) *.o $(OBJ)

# Build

build : $(OBJ)

%.o : %.c
	$(CC) -c $(FLAGS_CC) $<
