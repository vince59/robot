export DEBUG=yes
export CC=gcc
#LOG_LEVEL = 1 : error only, 2 : error and warning, 3 : error warning and info
ifeq ($(DEBUG),yes)
	CFLAGS=-I./include/ -I../lib_node/include/ -I../lib_system/include/ -W -Wall  -pedantic -g -D LOG_LEVEL=3
else
	CFLAGS=-I./include/ -I../lib_node/include/ -I../lib_system/include/ -W -Wall  -pedantic -D LOG_LEVEL=1
endif
export CFLAGS
export LDFLAGS=-L../lib_system -L../lib_node -lsystem -lnode -lpthread -lm
ROBOT_DIR=src/robot
SIMUL_DIR=src/simulator
SYSTM_DIR=src/lib_system
NODEL_DIR=src/lib_node
EXEC=simbot
MAKEFLAGS += --no-print-directory
all: $(EXEC)

$(EXEC):
ifeq ($(DEBUG),yes)
	@echo "Debug enabled"
else
	@echo "Make in release mode"
endif
	@(cd $(SYSTM_DIR) && $(MAKE))
	@(cd $(NODEL_DIR) && $(MAKE))
	@(cd $(SIMUL_DIR) && $(MAKE))
	@(cd $(ROBOT_DIR) && $(MAKE))

clean:
	@(cd $(SYSTM_DIR) && $(MAKE) $@)
	@(cd $(NODEL_DIR) && $(MAKE) $@)
	@(cd $(SIMUL_DIR) && $(MAKE) $@)
	@(cd $(ROBOT_DIR) && $(MAKE) $@)
	
mrproper: clean
	@(cd $(SYSTM_DIR) && $(MAKE) $@)
	@(cd $(NODEL_DIR) && $(MAKE) $@)
	@(cd $(SIMUL_DIR) && $(MAKE) $@)
	@(cd $(ROBOT_DIR) && $(MAKE) $@)
	@rm -f ./robot
	@rm -f ./simulator
	@rm -f ./log/*.log
	

