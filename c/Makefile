CC        := clang

SRC_DIR   := ./src
OUT_DIR   := ./out

BIN       := $(OUT_DIR)/fuss
SRCS      := $(wildcard $(SRC_DIR)/*.c)
OBJS      := $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))

CFLAGS    :=
LDLIBS    :=

$(BIN): $(OBJS)
	mkdir -p $(OUT_DIR)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c $(SRC_DIR)/$(wildcard *.h)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OUT_DIR) tmp/ *.plist *.out

test: $(BIN)
	perl test.pl

valgrind: $(BIN)
	valgrind -q --leak-check=full --log-file=./out/valgrind.out $(BIN)

.PHONY: clean test
