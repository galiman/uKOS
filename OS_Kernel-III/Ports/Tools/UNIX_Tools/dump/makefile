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
# Goal:		makefile for the "dump" and "xfer" applications.
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

ifndef PATH_UKOS_KERNEL
    $(error PATH_UKOS_KERNEL is not defined)
endif
ifndef PATH_TOOLS_UNIX
    $(error PATH_TOOLS_UNIX is not defined)
endif
ifndef PATH_FTDI_INCLUDES
    $(error PATH_FTDI_INCLUDES is not defined)
endif
ifndef PATH_FTDI_LIBRARIES
    $(error PATH_FTDI_LIBRARIES is not defined)
endif

PATH_XTOOLS		=  $(PATH_TOOLS_UNIX)
PATH_COMMFTDI	=  $(PATH_UKOS_KERNEL)/Ports/Tools/UNIX_Tools/_Comm_FTDI

PATH_INCLUDES	=  -I$(PATH_UKOS_KERNEL)/Includes
PATH_INCLUDES	+= -I$(PATH_FTDI_INCLUDES)
PATH_INCLUDES	+= -I$(PATH_COMMFTDI)
PATH_LIBRARIES	=  -L$(PATH_FTDI_LIBRARIES)

all :
	make -f ./makefile TARGET=dump clr_all
	make -f ./makefile TARGET=xferUart clr_all
	make -f ./makefile TARGET=xferSpi clr_all
	make -f ./makefile TARGET=dump build
	make -f ./makefile TARGET=xferUart build
	make -f ./makefile TARGET=xferSpi build
	make -f ./makefile TARGET=dump clr
	make -f ./makefile TARGET=xferUart clr
	make -f ./makefile TARGET=xferSpi clr

clean :
	make -f ./makefile TARGET=dump clr
	make -f ./makefile TARGET=xferUart clr
	make -f ./makefile TARGET=xferSpi clr

clean_all :
	make -f ./makefile TARGET=dump clr_all
	make -f ./makefile TARGET=xferUart clr_all
	make -f ./makefile TARGET=xferSpi clr_all

# Clean

clr :
	rm -f $(TARGET)

clr_all :
	rm -f $(TARGET)

# Build

build : $(TARGET)

$(TARGET) : $(TARGET).c $(PATH_COMMFTDI)/d2x.c $(PATH_COMMFTDI)/vcp.c $(PATH_COMMFTDI)/spi.c
	gcc -pedantic -ansi -std=c99 -std=gnu99 -pthread -o $(TARGET) $(TARGET).c $(PATH_COMMFTDI)/d2x.c $(PATH_COMMFTDI)/vcp.c $(PATH_COMMFTDI)/spi.c $(PATH_INCLUDES) ${PATH_LIBRARIES} -lftd2xx -ldl 
	cp -f $(TARGET) $(PATH_XTOOLS)/$(TARGET)

