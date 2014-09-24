
TARGET  = hardwareTest
TOOL    = arm-none-eabi-

OPTIMIZE        = -O0
USE_LTO         = NO
DEBUG       	= -g3 -ggdb

# compile options
MCU              = cortex-m0
CHIP            = STM32F0XX

LD_SCRIPT = stm32_flash.ld

#defines
        DEFS            = -D$(CHIP)
        DEFS			+= -DUSE_STDPERIPH_DRIVER
        DEFS			+= -DUSE_FULL_ASSERT
        
        BASE            = ../..
        CC              = $(TOOL)gcc
        CXX             = $(TOOL)g++
        LD              = $(TOOL)g++
        AS				= $(CC) -x assembler-with-cpp
        OBJCOPY         = $(TOOL)objcopy
        OBJDUMP         = $(TOOL)objdump
        SIZE            = $(TOOL)size -d
        RM              = rm -f
        CP              = cp
        MD              = mkdir

#dirs
        SRCDIR          = $(BASE)/src
        INCDIR          = $(BASE)/inc
        OBJDIR          = obj
        EXEDIR          = exe
        LSTDIR          = lst
        PRJDIR          = prj
        BAKDIR          = bak

#files
        HEX             = $(EXEDIR)/$(TARGET).hex
        BIN             = $(EXEDIR)/$(TARGET).bin
        AXF             = $(EXEDIR)/$(TARGET).axf
        ELF             = $(EXEDIR)/$(TARGET).elf
        MAP             = $(LSTDIR)/$(TARGET).map
        LSS             = $(LSTDIR)/$(TARGET).lss
        OK              = $(EXEDIR)/$(TARGET).ok
 
# source directories (all *.c, *.cpp and *.S files included)
DIRS    := $(SRCDIR)
DIRS	+= ${BASE}/src/mains/hardwareTest/
DIRS	+= $(BASE)/src/driver/System/
DIRS	+= $(BASE)/src/driver/Flash/

INCDIRS := ${BASE}/include/
INCDIRS += ${DIRS}
INCDIRS += $(BASE)/ext-src/CMSIS/Include/
INCDIRS += $(BASE)/ext-src/CMSIS/ST/STM32F0xx/Include/
INCDIRS += $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/inc/
INCDIRS += $(BASE)/ext-src/Utilities/STM32F0-Discovery/
       
# includes
INCS    := $(patsubst %, -I "%", $(DIRS))
INCS  	+= $(patsubst %, -I "%", $(INCDIRS))

# individual source files
SRCS    := $(BASE)/ext-src/Utilities/STM32F0-Discovery/stm32f0_discovery.c
SRCS	+= $(SRCDIR)/driver/startup_stm32f0xx.S
SRCS	+= $(SRCDIR)/driver/system_stm32f0xx.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_rcc.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_gpio.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_misc.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_syscfg.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_exti.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_flash.c
SRCS	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/stm32f0xx_tim.c

VPATH 	:= ${DIRS}
VPATH	+= $(BASE)/ext-src/Utilities/STM32F0-Discovery/
VPATH 	+= $(BASE)/ext-src/STM32F0xx_StdPeriph_Driver/src/
VPATH	+= $(SRCDIR)/driver/

#calc obj files list
        OBJS    := $(SRCS)
        OBJS    += $(wildcard $(addsuffix /*.cpp, $(DIRS)))
        OBJS    += $(wildcard $(addsuffix /*.c, $(DIRS)))
        OBJS    += $(wildcard $(addsuffix /*.S, $(DIRS)))       
        OBJS    := $(notdir $(OBJS))
        OBJS    := $(OBJS:.cpp=.o)
        OBJS    := $(OBJS:.c=.o)
        OBJS    := $(OBJS:.S=.o)
        OBJS    := $(patsubst %, $(OBJDIR)/%, $(OBJS))

#files to archive
        ARCFILES        = \
                $(SRCDIR) \
                $(PRJDIR) \
                $(SCMDIR) \
                $(BASE)/makefile \
                $(BASE)/.cproject \
                $(BASE)/.project
# flags
        FLAGS   = -mcpu=$(MCU) -mthumb -ggdb
        FLAGS   += $(INCS)
        FLAGS   += -MD
        FLAGS   += $(DEFS) 
        FLAGS   += -Wa,-adhlns=$(addprefix $(LSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))
		
        AFLAGS  = $(FLAGS)

        CFLAGS  = $(FLAGS)
        CFLAGS  += $(OPTIMIZE)
        CFLAGS	+= -include stm32f0xx.h
        #CFLAGS  += -std=gnu99
        CFLAGS  += -D GCC_ARMCM3
        CFLAGS  += $(DEBUG)
        CFLAGS  += -ffunction-sections -fdata-sections


        CXXFLAGS        = $(FLAGS)
        CXXFLAGS        += $(OPTIMIZE)
        CXXFLAGS        += -g
        CXXFLAGS        += -fno-exceptions -fno-rtti
        CXXFLAGS        += -ffunction-sections -fdata-sections
        CXXFLAGS        += -fno-threadsafe-statics
        CXXFLAGS        += -funsigned-bitfields -fshort-enums
        CXXFLAGS        += -Wall -Wextra
        CXXFLAGS        += -Winline
        CXXFLAGS        += -Wpointer-arith -Wredundant-decls
        CXXFLAGS        += -Wshadow -Wcast-qual -Wcast-align -pedantic

        LD_FLAGS        = -mcpu=$(MCU)
        LD_FLAGS        += -mthumb
        LD_FLAGS        += -nostartfiles
        LD_FLAGS        += -Wl,-Map="$(MAP)",--cref
        LD_FLAGS        += -Wl,--gc-sections
        LD_FLAGS        += -T$(LD_SCRIPT)

ifeq ($(USE_LTO),YES)
        CFLAGS          += -flto
        CXXFLAGS        += -flto
        LD_FLAGS        += -flto $(OPTIMIZE)
endif


.SILENT :

.PHONY: all start dirs build clean program reset archive

############# targets

all : start dirs $(AXF) $(ELF) $(HEX) $(BIN) $(LSS) $(OK)

build: clean all

start:
	@echo --- building $(TARGET) $(OBJS)

$(LSS): $(ELF) makefile
		@echo --- making asm-lst...
		@$(OBJDUMP) -dC $(ELF) > $(LSS)

$(OK): $(ELF) makefile
	@$(SIZE) $(ELF)
	@echo "Errors: none"

$(AXF): $(OBJS)  makefile
	@echo --- linking... axf
	$(LD) $(OBJS)  $(LIBS) $(LD_FLAGS) -o "$(AXF)"
       
$(ELF): $(OBJS) makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LIBS) $(LD_FLAGS) -o "$(ELF)"

$(HEX): $(ELF) makefile
	@echo --- make hex...
	@$(OBJCOPY) -O ihex $(ELF) $(HEX)

$(BIN): $(ELF) makefile
	@echo --- make binary...
	@$(OBJCOPY) -O binary $(ELF) $(BIN)

VPATH += $(DIRS)

$(OBJDIR)/%.o: %.cpp makefile
	@echo --- compiling $<...
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.c makefile
	@echo --- compiling $<...
	$(CC) -c $(CFLAGS) -o $@ $<
	$(RM) $(OBJDIR)/$*.d

$(OBJDIR)/%.o: %.S makefile
	@echo --- assembling $<...
	$(AS) -c $(AFLAGS) -o $@ $<


dirs: $(OBJDIR) $(EXEDIR) $(LSTDIR) $(BAKDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(EXEDIR):
	-@$(MD) $(EXEDIR)

$(LSTDIR):
	-@$(MD) $(LSTDIR)

$(BAKDIR):
	-@$(MD) $(BAKDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.p 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(LSTDIR)/*.map 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lss 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(HEX)
	-@$(RM) $(BIN)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)

archive:
	@echo --- archiving...
	7z a $(BAKDIR)/$(TARGET)_`date +%Y-%m-%d,%H-%M-%S` $(ARCFILES)
	@echo --- done!

# dependencies
ifeq (,$(findstring build,$(MAKECMDGOALS)))
 ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  ifeq (,$(findstring dirs,$(MAKECMDGOALS)))
  -include $(wildcard $(OBJDIR)/*.p)
  endif
 endif
endif
