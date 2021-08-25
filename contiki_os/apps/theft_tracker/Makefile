CONTIKI=../..
PROJ_DIR=.
PROJ_SRC_DIR=src
PROJ_INC_DIR=include
PROJ_LIB_DIR=lib
PROJ_BUILD_DIR=build

ifndef TARGET
TARGET=sky
endif

ifndef TYPE
TYPE=mote
endif

ifndef DEBUG
DEBUG=n
endif

TARGET_LIBFILES+=-L $(PROJ_LIB_DIR) -ltheft_tracker_utils

CLEAN+=${addprefix $(PROJ_SRC_DIR)/, *.$(TARGET)}

ifeq ($(TYPE), mote)
OUTPUT=${addprefix $(PROJ_SRC_DIR)/, theft_tracker_mote}
all:  $(OUTPUT)
endif

ifeq ($(DEBUG), y)
CFLAGS+=-DDEBUG
endif

ifeq ($(TYPE), gtw1)
CFLAGS+=-DGATEWAY_1
OUTPUT=${addprefix $(PROJ_SRC_DIR)/, theft_tracker_gateway_1}
all:  $(OUTPUT)
$(OUTPUT) : $(OUTPUT).c
$(OUTPUT).c : $(PROJ_SRC_DIR)/theft_tracker_gateway.ctemp
	cp $< $@
endif

ifeq ($(TYPE), gtw2)
CFLAGS+=-DGATEWAY_2
OUTPUT=${addprefix $(PROJ_SRC_DIR)/, theft_tracker_gateway_2}
all:  $(OUTPUT)
$(OUTPUT) : $(OUTPUT).c
$(OUTPUT).c : $(PROJ_SRC_DIR)/theft_tracker_gateway.ctemp
	cp $< $@
endif


$(PROJ_LIB_DIR)/theft_tracker_utils.o : $(PROJ_SRC_DIR)/theft_tracker_utils.c
	msp430-gcc -mmcu=msp430f1611 ${addprefix -I,$(SOURCEDIRS) $(PROJ_INC_DIR) $(CONTIKI)} -c $< -o $@

$(PROJ_LIB_DIR)/libtheft_tracker_utils.a : $(PROJ_LIB_DIR)/theft_tracker_utils.o
	msp430-ar -cvq $@ $<

libs: $(PROJ_LIB_DIR)/libtheft_tracker_utils.a

clean_libs: 
	rm -f ${addprefix $(PROJ_LIB_DIR)/,*.o *.a }

CONTIKI_WITH_RIME=1

include $(CONTIKI)/Makefile.include


