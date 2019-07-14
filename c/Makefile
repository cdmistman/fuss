SRC_DIR   := ./src
OUT_DIR   := ./out
BUILD_DIR := ./build

BIN       := $(OUT_DIR)/fuss
SRCS      := $(wildcard $(SRC_DIR)/*.c)
OBJS      := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS    :=
LDLIBS    :=

$(BIN): $(OBJS)
	mkdir -p $(OUT_DIR)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c $(SRC_DIR)/$(wildcard *.h)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OUT_DIR) $(BUILD_DIR) tmp/ *.plist *.out

test: $(BIN)
	perl test.pl

valgrind: $(BIN)
	valgrind -q --leak-check=full --log-file=./out/valgrind.out $(BIN)

.PHONY: clean test
