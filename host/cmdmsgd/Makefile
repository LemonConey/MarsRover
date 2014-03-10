#Prject Info
TARGET  = cmdmsg-host
INC_DIR += .
LIB_DIR +=
SRC_DIR = .
LIB     += 
OUT_DIR =
LIB_DEP =

#Makefile Template For Application
TARGET  := $(strip $(TARGET))
#CC      = gcc$(GCC_SUFFIX)
#CXX     = g++$(GCC_SUFFIX)
#CPP     = cpp$(GCC_SUFFIX)
CFLAGS  += $(C_INC) -pthread -Wall
CXXFLAGS+= $(C_INC) -pthread -std=c++0x -Wall -Wno-conversion-null -Wno-unused-private-field -Wno-unused-value -Wno-non-literal-null-conversion

C_INC   = $(INC_DIR:%=-I%) $(INC)
C_LIB   = $(LIB_DIR:%=-L%) $(LIB)

C_SRC   = $(wildcard $(SRC_DIR:%=%/*.c))
CXX_SRC = $(wildcard $(SRC_DIR:%=%/*.cpp))
C_OBJ   = $(C_SRC:%.c=%.o)
CXX_OBJ = $(CXX_SRC:%.cpp=%.o)
C_DEP   = $(C_SRC:%.c=%.d)
CXX_DEP = $(CXX_SRC:%.cpp=%.d)

ALL_OBJ = $(C_OBJ) $(CXX_OBJ)
ALL_DEP = $(C_DEP) $(CXX_DEP)

ifeq ($(debug), 1)
CFLAGS   += -O0 -g -DLOCAL_DEBUG
CXXFLAGS += -O0 -g -DLOCAL_DEBUG
endif


.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(ALL_OBJ) $(ALL_DEP) $(TARGET) $(OUT_DIR)/$(TARGET)

$(C_DEP): %.d : %.c
	$(CPP) $(C_INC) -MM $< > $@

$(CXX_DEP): %.d : %.cpp
	$(CPP) $(C_INC) -MM $< > $@


ifneq ($(MAKECMDGOALS), clean)
-include $(ALL_DEP)
endif

$(TARGET): $(ALL_DEP) $(ALL_OBJ)
	$(CXX) $(ALL_OBJ) $(C_LIB) $(LDFLAGS) -o $@
