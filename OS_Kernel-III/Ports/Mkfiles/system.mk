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
# Goal:		makefile for uKOS systems (generic part).
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

PATH_XTOOLS		=  $(PATH_TOOLS_UNIX)

CC				=  $(PATH_COMPILER)/bin/$(PREFIX)gcc
OBJCOPY			=  $(PATH_COMPILER)/bin/$(PREFIX)objcopy
OBJDUMP			=  $(PATH_COMPILER)/bin/$(PREFIX)objdump
SIZE			=  $(PATH_COMPILER)/bin/$(PREFIX)size
AR				=  $(PATH_COMPILER)/bin/$(PREFIX)ar
BIN2CKSUM		=  $(PATH_TOOLS_UNIX)/bin2cksum
BIN2XML			=  $(PATH_TOOLS_UNIX)/bin2xml

PATH_INCLUDES	+= -I$(PATH_UKOS)/Boots
PATH_INCLUDES	+= -I$(PATH_UKOS)/Includes
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_comm
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_gene
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_kern
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_nlib
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_peri
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_tbox
PATH_INCLUDES	+= -I$(PATH_UKOS)/Lib_engi
PATH_INCLUDES	+= -I$(PATH_UKOS)/Ports
PATH_INCLUDES	+= -I$(PATH_UKOS)/Processes
PATH_INCLUDES	+= -I$(PATH_UKOS)/Protocols
PATH_INCLUDES	+= -I$(PATH_UKOS)/Tools
PATH_INCLUDES	+= -I$(PATH_VARI)/$(BOARD)
PATH_INCLUDES	+= -I$(PATH_BASE)/$(BOARD)
PATH_INCLUDES	+= -I$(PATH_VARI)/Shared
PATH_INCLUDES	+= -I$(PATH_VARI)/System
PATH_INCLUDES	+= -I$(PATH_VARI)/Runtime
PATH_INCLUDES	+= -I$(PATH_BASE)/Runtime
PATH_INCLUDES	+= -I$(PATH_VARI)/Includes/System
PATH_INCLUDES	+= -I$(PATH_BASE)/Includes/System
PATH_INCLUDES	+= -I$(PATH_VARI)/Includes/Board
PATH_INCLUDES	+= -I$(PATH_BASE)/Includes/Board
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Devices
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/models
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Generic/models
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/cpu_reg
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/macros
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Cores/$(CORE)/Runtime
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/Runtime
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Generic/Runtime

all :
	make -f ./makefile TARGET=EPROM clr_all
	make -f ./makefile TARGET=EPROM filer
	make -f ./makefile TARGET=EPROM build
	make -f ./makefile TARGET=EPROM utils
	make -f ./makefile TARGET=EPROM clr
	make -f ./makefile TARGET=EPROM size

clean :
	make -f ./makefile TARGET=EPROM clr

clean_all :
	make -f ./makefile TARGET=EPROM clr_all

# Clean

clr :
	rm -f *.a *.d *.o *.lst *.dis filer.h version.h *.su

clr_all :
	rm -f *.a *.d *.o *.lst *.dis *.map *.elf *.bin *.ck *.s3 *.xml filer.h version.h *.su *.stack

# Build the filer table

filer :
	echo $(basename $(notdir $(PROT))) \
	     $(basename $(notdir $(TOOL))) \
		 $(basename $(notdir $(RTCB))) \
	     $(basename $(notdir $(KERN))) \
	     $(basename $(notdir $(LIBX))) \
	     $(basename $(notdir $(SHAR))) \
	     $(basename $(notdir $(PROC))) > modules.tmp
	filer modules.tmp
	rm -f modules.tmp

# Collect the versioning information

	echo "#define VERSIONING_TOOL \"No versioning system\"  " > version.h
	echo "#define SW_VERSION \" \"  "                        >> version.h
    ifeq ($(VERSIONING),SVN)
	    echo "#define VERSIONING_TOOL \"svn:\"  "     > version.h
	    echo "#define SW_VERSION \"`svnversion`\"  " >> version.h
    endif
    ifeq ($(VERSIONING),GIT)
	    echo "#define VERSIONING_TOOL \"git:\"  "                     > version.h
	    echo "#define SW_VERSION \"`git rev-parse --short HEAD`\"  " >> version.h
    endif

# Compile the components & generate the uKOS system

build : makefile $(TARGET).elf

$(TARGET).elf : $(LINKS_LD) $(UKOS_COMPONENTS)
	$(CC) $(FLAGS_LD) \
	first.o \
	$(UKOS_COMPONENTS) \
	-lm -o $@

librtcb.a : $(RTCB) $(RTCB_HIDE)
	$(AR) rcs librtcb.a $(notdir $(RTCB)) $(notdir $(RTCB_HIDE))

libkern.a : $(KERN) $(KERN_HIDE)
	$(AR) rcs libkern.a $(notdir $(KERN)) $(notdir $(KERN_HIDE))

liblibx.a : $(LIBX) $(LIBX_HIDE)
	$(AR) rcs liblibx.a $(notdir $(LIBX)) $(notdir $(LIBX_HIDE))

libshar.a : $(SHAR) $(SHAR_HIDE)
	$(AR) rcs libshar.a $(notdir $(SHAR)) $(notdir $(SHAR_HIDE))

libproc.a : $(PROC) $(PROC_HIDE)
	$(AR) rcs libproc.a $(notdir $(PROC)) $(notdir $(PROC_HIDE))

libtool.a : $(TOOL) $(TOOL_HIDE)
	$(AR) rcs libtool.a $(notdir $(TOOL)) $(notdir $(TOOL_HIDE))

libprot.a : $(PROT) $(PROT_HIDE)
	$(AR) rcs libprot.a $(notdir $(PROT)) $(notdir $(PROT_HIDE))

libengi.a :
	cp -f $(PATH_PORT)/EquatesModels/Cores/$(CORE)/engines/libengi.a libengi.a

%.o : %.s
	$(CC) -c $(FLAGS_CC) $<

%.o : %.c
	$(CC) -c $(FLAGS_CC) $<

# Compute the sizes (data, text, etc.) of the system

ELFSIZE = $(SIZE) -A --radix=16 $(TARGET).elf

size :
	if [ -f $(TARGET).elf ]; then echo; echo "Size of the generated program"; $(ELFSIZE); echo; fi
