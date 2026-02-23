# =============================================================================
# emojicode agent — Build System
# =============================================================================

# -- Tools --------------------------------------------------------------------
EMOJICODEC := emojicodec
CXX        := clang++
LINKER     := clang++

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  ARCH_FLAG := -arch x86_64
else
  ARCH_FLAG :=
endif

# -- Paths --------------------------------------------------------------------
SOURCE     := src/main.🍇
EMOJI_SRCS := $(shell find src -name '*.🍇' 2>/dev/null || true)
EMOJI_OBJ  := out/main.o
OUTPUT     := out/main

# C++ FFI modules
NATIVE_SRCS := src/infrastructure/adapters/ai/openrouter-ai.adapter.cpp \
               src/infrastructure/adapters/persistence/native-file-system.adapter.cpp \
               src/infrastructure/adapters/ui/terminal-input.adapter.cpp
NATIVE_OBJS := out/native-file-system.adapter.o \
               out/openrouter-ai.adapter.o \
               out/terminal-input.adapter.o

EMOJI_LIBS := /usr/local/EmojicodePackages/s/libs.a \
              /usr/local/EmojicodePackages/runtime/libruntime.a \
              /usr/local/EmojicodePackages/testtube/libtesttube.a

OUTDIR := out
WORKDIR ?= workdir

# -- Phony targets ------------------------------------------------------------
.PHONY: all build run clean test help vhs

all: run

$(OUTDIR):
	@mkdir -p $(OUTDIR)

# -- C++ FFI compilation ------------------------------------------------------
out/terminal-input.adapter.o: src/infrastructure/adapters/ui/terminal-input.adapter.cpp | $(OUTDIR)
	@echo "  CXX  $<"
	@$(CXX) $(ARCH_FLAG) -std=c++17 -c -fPIC $< -o $@ -I/usr/local/include

out/openrouter-ai.adapter.o: src/infrastructure/adapters/ai/openrouter-ai.adapter.cpp | $(OUTDIR)
	@echo "  CXX  $<"
	@$(CXX) $(ARCH_FLAG) -std=c++17 -c -fPIC $< -o $@ -I/usr/local/include

out/native-file-system.adapter.o: src/infrastructure/adapters/persistence/native-file-system.adapter.cpp | $(OUTDIR)
	@echo "  CXX  $<"
	@$(CXX) $(ARCH_FLAG) -std=c++17 -c -fPIC $< -o $@ -I/usr/local/include

# -- Emojicode compilation ----------------------------------------------------
$(EMOJI_OBJ): $(SOURCE) $(EMOJI_SRCS) $(NATIVE_OBJS) | $(OUTDIR)
	@echo "  EMC  $(SOURCE)"
	@$(EMOJICODEC) -c $(SOURCE) -o $(EMOJI_OBJ)

# -- Link ---------------------------------------------------------------------
$(OUTPUT): $(EMOJI_OBJ) $(NATIVE_OBJS)
	@echo "  LD   $@"
	@$(LINKER) $(ARCH_FLAG) $(EMOJI_OBJ) $(NATIVE_OBJS) -o $@ \
		$(EMOJI_LIBS) \
		-lcurl

build: $(OUTPUT)

run: build
	@if [ -f .env ]; then set -a && . ./.env && set +a; fi; \
	cd $(WORKDIR) && exec ../$(OUTPUT)

test: $(NATIVE_OBJS) | $(OUTDIR)
	@mkdir -p out/tests
	@mkdir -p out/tests/sandbox
	@echo "  EMC  src/tests/main.🍇"
	@$(EMOJICODEC) -c src/tests/main.🍇 -o out/tests/test.o
	@echo "  LD   out/tests/test"
	@$(LINKER) $(ARCH_FLAG) out/tests/test.o $(NATIVE_OBJS) -o out/tests/test \
		$(EMOJI_LIBS) \
		-lcurl
	@out/tests/test

vhs: build
	@echo "  VHS  docs/assets/demo.tape"
	@vhs < docs/assets/demo.tape

clean:
	@echo "  CLEAN $(OUTDIR)/"
	@rm -rf $(OUTDIR)/
