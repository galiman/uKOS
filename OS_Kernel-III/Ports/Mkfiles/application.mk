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
# Goal:		makefile for uKOS applications (generic part).
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

PATH_RUNT_GENE	=  $(PATH_PORT)/EquatesModels/Generic/Runtime
PATH_RUNT_CORE	=  $(PATH_PORT)/EquatesModels/Cores/$(CORE)/Runtime
PATH_RUNT_SOC	=  $(PATH_PORT)/EquatesModels/SOCs/$(SOC)/Runtime

all :
	make -f ./makefile TARGET=$(PROJECT) clr_all
	make -f ./makefile TARGET=$(PROJECT) build
	make -f ./makefile TARGET=$(PROJECT) clr

src :
	make -f ./makefile TARGET=$(PROJECT) clr_all
	make -f ./makefile TARGET=$(PROJECT) display

clean :
	make -f ./makefile TARGET=$(PROJECT) clr

clean_all :
	make -f ./makefile TARGET=$(PROJECT) clr_all

# Clean

clr :
	rm -f *.d *.o *.elf *.lst *.dis *.map *.bin *.su

clr_all :
	rm -f *.d *.o *.elf *.lst *.dis *.map *.bin *.s3 *.su *.stack

# Compile the components & generate the uKOS application

build : $(TARGET).bin $(TARGET).elf $(TARGET).s3 $(TARGET).lst size
display : $(TARGET).bin $(TARGET).elf $(TARGET).s3 $(TARGET).lst $(TARGET).dis size

$(TARGET).elf : $(OBJ) $(LINKS_LD)
	$(CC) $(FLAGS_LD) \
	$(OBJ) \
	$(MYLIB) \
	-Wl,--just-symbols=$(PATH_VARI)/System/EPROM.elf \
	-lc -lm -o $@

%.o : %.s
	$(CC) -c $(FLAGS_CC) $<

%.o : %.c
	$(CC) -c $(FLAGS_CC) $<

%.o : ../%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_USER)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_CORE)/%.s
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_CORE)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_SOC)/%.s
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_SOC)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_GENE)/%.s
	$(CC) -c $(FLAGS_CC) $<

%.o : $(PATH_RUNT_GENE)/%.c
	$(CC) -c $(FLAGS_CC) $<

%.lst : %.elf
	$(OBJDUMP) $(SWTCH_OBJDUMP) $< > $@

%.dis : %.elf
	$(OBJDUMP) -S $< > $@

%.bin : %.elf
	$(OBJCOPY) -O binary --strip-all $< $@

%.s3 : %.elf
	$(OBJCOPY) -O srec --srec-forceS3 $< $@

# Compute the sizes (data, text, etc.) of the application

ELFSIZE = $(SIZE) -A --radix=16 $(TARGET).elf
size :
	if [ -f $(TARGET).elf ]; then echo; echo "Size of the generated program"; $(ELFSIZE); echo; fi
	cat *.su | awk '{ $$3=""; printf("%s\t [Bytes] ---> %s\n",$$2, $$1); }' | sort -gro $(TARGET).stack
