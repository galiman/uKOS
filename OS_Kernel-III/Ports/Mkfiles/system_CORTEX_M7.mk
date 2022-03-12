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
# Goal:		makefile for uKOS systems (cortex-m7 specific).
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

# Compiler flags

PATH_COMPILER	=  $(PATH_GCC_CORTEX7)

FLAGS_UKOS		=  -D__UKOS__ -D__$(BOARD)__ -D__$(SOC)__ -D__$(CORE)__ -D__LITTLE_ENDIAN__
SWTCH_OBJDUMP	=  -f -p -g -D -d -h -t -s

FLAGS_CC		=  -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mthumb -fsingle-precision-constant
FLAGS_CC		+= -Wall -c -g $(ISO_IEC_9899) $(OPTIMISATION) -Wlogical-op
FLAGS_CC		+= $(STANDARD)
FLAGS_CC		+= $(FLAGS_UKOS) $(FLAGS_USER)
FLAGS_CC		+= $(PATH_INCLUDES)
FLAGS_CC		+= -MMD $(addprefix -I,$(INCLUDE))

# Additional checks
# The commented switches are not considered

#FLAGS_CC		+= -Wunused-parameter
#FLAGS_CC		+= -Wcast-qual
#FLAGS_CC		+= -Wmissing-prototypes
#FLAGS_CC		+= -Wmissing-declarations
#FLAGS_CC		+= -Wimplicit-fallthrough=3 
#FLAGS_CC		+= -Wshift-negative-value

FLAGS_CC		+= -Wclobbered  
FLAGS_CC		+= -Wempty-body  
FLAGS_CC		+= -Wignored-qualifiers 
FLAGS_CC		+= -Wmissing-field-initializers  
FLAGS_CC		+= -Wmissing-parameter-type
FLAGS_CC		+= -Wold-style-declaration
FLAGS_CC		+= -Woverride-init  
FLAGS_CC		+= -Wsign-compare
FLAGS_CC		+= -Wtype-limits  
FLAGS_CC		+= -Wuninitialized  
FLAGS_CC		+= -Wunused-but-set-parameter
FLAGS_CC		+= -Wshadow
FLAGS_CC		+= -Wpointer-arith
FLAGS_CC		+= -Wstrict-prototypes
FLAGS_CC		+= -Wunreachable-code
FLAGS_CC		+= -Wnonnull
FLAGS_CC		+= -Winit-self
FLAGS_CC		+= -Wmissing-braces
FLAGS_CC		+= -Wparentheses
FLAGS_CC		+= -Wswitch

# Recommended for CERT C

FLAGS_CC		+= -Wchar-subscripts
FLAGS_CC		+= -Wswitch-default
FLAGS_CC		+= -Wcast-align
FLAGS_CC		+= -Wtrigraphs
FLAGS_CC		+= -Wcast-qual
FLAGS_CC		+= -Wformat
FLAGS_CC		+= -Wcast-qual
FLAGS_CC		+= -Wcomment
FLAGS_CC		+= -Wformat-security
FLAGS_CC		+= -Wimplicit
FLAGS_CC		+= -Wsequence-point

# Analisys

FLAGS_CC		+= -fstack-usage
FLAGS_CC		+= -mpoke-function-name

LINKS_LD		=  $(PATH_MAPP)/link.ld
FLAGS_LD		=  -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mthumb -fsingle-precision-constant
FLAGS_LD		+= -Wall -nostartfiles
FLAGS_LD		+= -T$(LINKS_LD)
FLAGS_LD		+= -Wl,-Map=$(TARGET).map,--cref

# Generate some useful & necessary files

utils :
	$(OBJDUMP) $(SWTCH_OBJDUMP) $(TARGET).elf > $(TARGET).lst
	$(OBJDUMP) -S $(TARGET).elf > $(TARGET).dis
	$(OBJCOPY) -O binary --strip-all $(TARGET).elf $(TARGET).bin
	$(OBJCOPY) -I binary -O srec --srec-forceS3 --strip-al $(TARGET).bin $(TARGET).s3
	$(BIN2CKSUM) $(TARGET).bin $(TARGET).ck
	cat *.su | awk '{ $$3=""; printf("%s\t ---> %s\n",$$2, $$1); }' | sort -gro $(TARGET).stack
