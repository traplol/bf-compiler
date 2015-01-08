CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
BIN_DIR:= bin
INCLUDES:= -Iinc
CFLAGS:= -O0 -g -Werror -Wall -pedantic -pedantic-errors -Wextra -std=c99 $(INCLUDES)
LDFLAGS:= -g
#CFLAGS:= -fsanitize=address -fno-optimize-sibling-calls -O0 -g -Werror -Wall -pedantic -pedantic-errors -Wextra -std=c99 $(INCLUDES)
#LDFLAGS:= -g -fsanitize=address -fno-optimize-sibling-calls
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
EXECUTABLE:= $(BIN_DIR)/bfc

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
