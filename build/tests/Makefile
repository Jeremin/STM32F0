#---------
#
# CppUTest Examples Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

BASE = ../..
SRCDIR = $(BASE)/src
TSTDIR = $(BASE)/tests

#--- Inputs ----#
COMPONENT_NAME = STM32F0

#we need 2 hierarchy levels to compare for the ../.. in our path
CPPUTEST_OBJS_DIR = objs/lvl1/lvl2
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

PROJECT_HOME_DIR = .
CPPUTEST_HOME = $(BASE)/ext-src/cpputest-3.6

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = GccStdC

CPPUTEST_WARNINGFLAGS += -Wall 
CPPUTEST_WARNINGFLAGS += -Werror 
CPPUTEST_WARNINGFLAGS += -Wswitch-default 
CPPUTEST_WARNINGFLAGS += -Wswitch-enum  
CPPUTEST_CFLAGS += -Wstrict-prototypes -std=gnu99
CPPUTEST_USE_GCOV = Y
CPPUTEST_ENABLE_DEBUG = Y

SRC_DIRS = \
	$(SRCDIR)/lib

TEST_SRC_DIRS = \
	$(TSTDIR) \
	$(TSTDIR)/lib \	

INCLUDE_DIRS =\
  .\
  $(CPPUTEST_HOME)/include \
  $(SRC_DIRS) \
  $(BASE)/inc

include $(CPPUTEST_HOME)/build/MakefileWorker.mk


