_CLANG                  =$(if $(V), $(CLANG), @$(CLANG))
_GCC                    =$(if $(V), $(GCC), @$(GCC))
RM                     :=$(if $(V), rm, @rm)
MKDIR                  :=$(if $(V), mkdir, @mkdir)

BUILD_DIR              :=__BUILD_
TARGET                 =$(BUILD_DIR)/$(notdir $(PWD)).elf

src                    :=$(shell find . -name "*.[c]")
obj                    :=$(subst .c,.o,$(src))
obj                    :=$(addprefix $(BUILD_DIR)/,$(obj))

.PHONY:all check clean

all:check $(BUILD_DIR) $(TARGET)

CFLAG                  =-Wall -Wextra -I/usr/include -I/usr/riscv64-linux-gnu/include --target=$(TARGET_TRIPLE) \
                            -DRV64 -march=rv64gcv1p0_zbb -mllvm -riscv-v-vector-bits-min=512 \
                            -menable-experimental-extensions -Ofast -c
LFLAG                  :=-Ofast -static
LIB                    :=-lm

check:
ifndef CLANG
	@echo "Environment variable CLANG must be set"
	exit 1
endif
ifndef GCC
	@echo "Environment variable GCC must be set"
	exit 1
endif
ifndef TARGET_TRIPLE
	@echo "Environment variable TARGET_TRIPLE must be set"
	exit 1
endif

$(BUILD_DIR):
	@echo "    CleanDir $@"
	$(RM) -rf $@
	@echo "    MakeDir  $@"
	$(MKDIR) $@

$(TARGET):$(obj)
	@echo "    Link     $@"
	$(_GCC) $(LFLAG) $^ -o $@ $(LIB)

$(obj):$(BUILD_DIR)/%.o:%.c
	@echo "    MakeDir  $(dir $@)"
	$(MKDIR) -p $(dir $@)
	@echo "    Compile  $<"
	$(_CLANG) $(CFLAG) $< -o $@

clean:
	@echo "    CleanDir $(BUILD_DIR)"
	$(RM) -rf $(BUILD_DIR)
