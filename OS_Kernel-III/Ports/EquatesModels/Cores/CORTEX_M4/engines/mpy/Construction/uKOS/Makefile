include	../py/mkenv.mk

# For uKOS
# include py core make definitions

include ../py/py.mk

INC				+= -I.
INC				+= -I..
INC				+= -I$(BUILD)

# Select the compiler
# cortex M4 or M7

ifeq ($(TARGET), M4)
CROSS_COMPILE	=  $(PATH_GCC_CORTEX3)/bin/arm-none-eabi-

FLAGS_UKOS		=  -D__UKOS__ -D__CORTEX_M4__

CFLAGS_CORTEX	=  -mthumb -mtune=cortex-m4 -mabi=aapcs-linux
CFLAGS_CORTEX 	+= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS_CORTEX	+= -fsingle-precision-constant -Wdouble-promotion -fshort-enums
CFLAGS_CORTEX	+= $(FLAGS_UKOS)

CFLAGS			= $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_CORTEX) $(COPT) -Os -DNDEBUG

ARCHIVE			= $(BUILD)/microythonM4
endif

ifeq ($(TARGET), M7)
CROSS_COMPILE	=  $(PATH_GCC_CORTEX7)/bin/arm-none-eabi-

FLAGS_UKOS		=  -D__UKOS__ -D__CORTEX_M7__

CFLAGS_CORTEX	=  -mthumb -mtune=cortex-m7 -mabi=aapcs-linux
CFLAGS_CORTEX	+= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard
CFLAGS_CORTEX	+= -fsingle-precision-constant -Wdouble-promotion -fshort-enums
CFLAGS_CORTEX	+= $(FLAGS_UKOS)

CFLAGS			= $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_CORTEX) $(COPT) -Os -DNDEBUG

ARCHIVE			= $(BUILD)/microythonM7
endif

LIBS			=

SRC_C			= lib/utils/stdout_helpers.c \
				  lib/utils/pyexec.c \
				  lib/libc/string0.c \
				  lib/libm/math.c \
				  lib/libm/fmodf.c \
				  lib/libm/nearbyintf.c \
				  lib/libm/ef_sqrt.c \
				  lib/libm/kf_rem_pio2.c \
				  lib/libm/kf_sin.c \
				  lib/libm/kf_cos.c \
				  lib/libm/kf_tan.c \
				  lib/libm/ef_rem_pio2.c \
				  lib/libm/sf_sin.c \
				  lib/libm/sf_cos.c \
				  lib/libm/sf_tan.c \
				  lib/libm/sf_frexp.c \
				  lib/libm/sf_modf.c \
				  lib/libm/asinfacosf.c \
				  lib/libm/atanf.c \
				  lib/libm/atan2f.c \
				  lib/mp-readline/readline.c \
				  sf_ldexp_efr.c \
				  $(BUILD)/_frozen_mpy.c \

#	lib/libm/sf_ldexp.c \

OBJ				= $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(ARCHIVE).aa

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)../tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(ARCHIVE).aa: $(OBJ)
	$(AR) rcs $(ARCHIVE).aa $(BUILD)/py/*.o $(BUILD)/extmod/*.o $(BUILD)/lib/embed/*.o $(BUILD)/lib/libc/*.o $(BUILD)/lib/mp-readline/*.o $(BUILD)/lib/utils/*.o $(BUILD)/build/*.o 

include ../py/mkrules.mk
