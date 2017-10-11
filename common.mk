SRCS+=$(wildcard $(SOURCE_DIR)/*.c)
OBJS+=$(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o : ./%.c
	@#echo "cc $(CC) -c $(CFLAGS) $< -o $@"
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o : ./%.cpp
	@#echo "cpp $(CC) -c $(CPPFLAGS) $< -o $@"
	$(CC) -c $(CPPFLAGS) $< -o $@

build: $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)
