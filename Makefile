# =======================
# Configurable Variables
# =======================

NAME         := the-debt-cycle
SRC          := *.c
BUILD_DIR    := build
WEB_DIR      := web
RAYLIB_DIR   := raylib/src
PLATFORM     ?= PLATFORM_DESKTOP
BUILD_MODE   ?= RELEASE

# Default compiler and flags
CC           := gcc
CFLAGS       := -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Wextra -Wmissing-prototypes -Wstrict-prototypes
LDFLAGS      :=
LDLIBS       := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXT          :=

# =======================
# Platform Adjustments
# =======================

ifeq ($(PLATFORM),PLATFORM_WEB)
	CC      := emcc
	EXT     := .html
	CFLAGS  += -O2 -sUSE_GLFW=3 -sASYNCIFY -sTOTAL_MEMORY=134217728 -sFORCE_FILESYSTEM=1 -DPLATFORM_WEB --preload-file assets@/assets
	LDFLAGS += --shell-file $(RAYLIB_DIR)/minshell.html
	LDLIBS  := $(RAYLIB_DIR)/libraylib.web.a
	OUT_DIR := $(WEB_DIR)
else ifeq ($(OS),Windows_NT)
	CC      := gcc
	CFLAGS  += -DPLATFORM_DESKTOP
	LDFLAGS += -static-libgcc -static-libstdc++
	LDLIBS  := -lraylib -lopengl32 -lgdi32 -lwinmm
	OUT_DIR := $(BUILD_DIR)
else
	CFLAGS  += -DPLATFORM_DESKTOP
	OUT_DIR := $(BUILD_DIR)
endif

# =======================
# Targets
# =======================

.PHONY: all clean raylib web windows

all: $(OUT_DIR)/$(NAME)$(EXT)

$(OUT_DIR)/$(NAME)$(EXT): raylib
	@mkdir -p $(OUT_DIR)
	$(CC) $(SRC) \
		$(CFLAGS) -I$(RAYLIB_DIR) \
		-L$(RAYLIB_DIR) $(LDFLAGS) $(LDLIBS) \
		-o $@
	cp -r assets $(OUT_DIR)/

raylib:
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=$(PLATFORM)

	cd $(RAYLIB_DIR) && \
	emcc -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c utils.c -Os -Wall -DPLATFORM_WEB && \
	emcc -c raudio.c -Os -Wall -DPLATFORM_WEB && \
	emar rcs libraylib.web.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o

web:
	$(MAKE) PLATFORM=PLATFORM_WEB

windows:
	$(MAKE) PLATFORM=PLATFORM_DESKTOP

clean:
	$(MAKE) -C $(RAYLIB_DIR) clean
	rm -rf $(BUILD_DIR)/* $(WEB_DIR)/*
	rm -f $(RAYLIB_DIR)/*.o $(RAYLIB_DIR)/libraylib.web.a
