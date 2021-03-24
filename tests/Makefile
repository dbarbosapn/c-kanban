PROJECT_NAME := c-kanban-tests

COMPILER := gcc
COMPILER_FLAGS := -w -g

LIB_FOLDER := ../lib

SOURCE_FILES := $(wildcard *.c) $(wildcard $(LIB_FOLDER)/*.c)
OBJ_FILES := $(patsubst %.c, %.o, $(SOURCE_FILES))

.PHONY: all compile run

all: compile .clear run

compile: 
	$(COMPILER) $(COMPILER_FLAGS) -I$(LIB_FOLDER) -o $(PROJECT_NAME).out $(SOURCE_FILES)

.clear: 
	clear

run:
	@./$(PROJECT_NAME).out