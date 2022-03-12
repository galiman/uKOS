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
# Goal:		makefile for the uKOS standalone sLoader.
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

PATH_INCLUDES	=  -I$(PATH_UKOS)
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
PATH_INCLUDES	+= -I$(PATH_PORT)/Targets/$(BOARD)
PATH_INCLUDES	+= -I$(PATH_PORT)/Targets/$(BOARD)/System
PATH_INCLUDES	+= -I$(PATH_PORT)/Targets/$(BOARD)/Runtime
PATH_INCLUDES	+= -I$(PATH_PORT)/Targets/$(BOARD)/Includes/System
PATH_INCLUDES	+= -I$(PATH_PORT)/Targets/$(BOARD)/Includes/Board
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Devices
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/Generic/models
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/models
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/cpu_reg
PATH_INCLUDES	+= -I$(PATH_PORT)/EquatesModels/SOCs/$(SOC)/macros

PATH_TOOLS		=  $(PATH_PORT)/Tools/Standalone/sLoader
PATH_RUNTIME	=  $(PATH_PORT)/Targets/$(BOARD)/Base/Runtime

CC				=  $(PATH_COMPILER)/bin/$(PREFIX)gcc
OBJCOPY			=  $(PATH_COMPILER)/bin/$(PREFIX)objcopy
SIZE			=  $(PATH_COMPILER)/bin/$(PREFIX)size
BIN2XML			=  $(PATH_TOOLS_UNIX)/bin2xml

LINKS_LD		=  $(PATH_RUNTIME)/link
FLAGS_OBJDUMP	=  -f -p -g -D -d -h -t -s

all :
	make -f ./makefile TARGET=$(PROJECT) clr_all
	make -f ./makefile TARGET=$(PROJECT) build
	make -f ./makefile TARGET=$(PROJECT) clr

clean :
	make -f ./makefile TARGET=$(PROJECT) clr

clean_all :
	make -f ./makefile TARGET=$(PROJECT) clr_all

# Clean

clr :
	rm -f *.d *.o *.elf *.map

clr_all :
	rm -f *.d *.o *.elf *.map *.bin *.xml

# Build

build : $(TARGET).bin $(TARGET).elf $(TARGET).xml size

$(TARGET).elf : makefile $(OBJ) $(LINKS_LD).ld
	$(CC) $(FLAGS_LD) \
	$(OBJ) \
	-o $@

%.o : %.s
	$(CC) -c $(FLAGS_CC) $<

%.o : %.c
	$(CC) -c $(FLAGS_CC) $<

%.o : ../%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNTIME)/%.s
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNTIME)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_TOOLS)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.bin : %.elf
	$(OBJCOPY) -O binary --strip-all $< $@

%.xml : %.bin
	$(BIN2XML) -$(XMLbehavior) -$(XMLHEADER) $(XMLADDRESS) -$(XMLMEMORY) $< $@

ELFSIZE = $(SIZE) -A --radix=16 $(TARGET).elf
size :
	if [ -f $(TARGET).elf ]; then echo; echo "Size of the generated program"; $(ELFSIZE); echo; fi
