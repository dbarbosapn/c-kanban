PROJECT_NAME := c-kanban

COMPILER := gcc
COMPILER_FLAGS := -Wall

OUTPUT_FOLDER := bin

SOURCE_FILES := $(wildcard src/*.c)
OBJ_FILES := $(patsubst %.c, %.o, $(SOURCE_FILES))

.PHONY: all compile clean run

all: compile clean .clear run

compile: $(OBJ_FILES)
	$(COMPILER) $(COMPILER_FLAGS) -o $(OUTPUT_FOLDER)/$(PROJECT_NAME) $(OBJ_FILES)

.clear: 
	clear

run:
	@./$(OUTPUT_FOLDER)/$(PROJECT_NAME)

clean:
	rm -f $(OBJ_FILES)