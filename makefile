TARGET = $(OUT_DIR)/MineSweeper.exe

# "release" or "debug"
BUILD_TYPE := debug

CC  := gcc
CXX := g++
RCC := windres

CFLAGS   =
CXXFLAGS = -std=c++20 \
           -Wall \
           -Wextra \
           -I./include \
           -IC:/dxlib
CPPFLAGS = -D_UNICODE \
           -DUNICODE \
           -D_WIN32 \
           -DDX_GCC_COMPILE \
           -DDX_NON_USING_NAMESPACE_DXLIB

ifeq "$(strip $(BUILD_TYPE))" "release"
CFLAGS   +=
CXXFLAGS += -O2
CPPFLAGS += -DNDEBUG
else
CFLAGS   +=
CXXFLAGS += -g -O0
CPPFLAGS += -D_DEBUG
endif

LDFLAGS = -static \
          -mwindows \
          -LC:/dxlib
LDLIBS = -lDxLibW \
         -lDxUseCLibW \
         -lDxDrawFunc \
         -ljpeg \
         -lpng \
         -lzlib \
         -ltiff \
         -ltheora_static \
         -lvorbis_static \
         -lvorbisfile_static \
         -logg_static \
         -lbulletdynamics \
         -lbulletcollision \
         -lbulletmath \
         -lopusfile \
         -lopus \
         -lsilk_common \
         -lcelt

SRC_DIR = src
INC_DIR = include
RES_DIR = res
OUT_DIR = bin/$(strip $(BUILD_TYPE))

SRCS   = $(shell find $(SRC_DIR) -name "*.$(SRC_PREFIX)")
OBJS   = $(call src-to-obj,$(SRCS)) $(call res-to-obj,$(RESS))
RESS   = $(shell find $(RES_DIR) -name "*.$(RES_PREFIX)")
DEPEND = $(OUT_DIR)/depend.mk

SRC_PREFIX = cc
OBJ_PREFIX = o
RES_PREFIX = rc


# $(call src-to-obj,src-files)
src-to-obj = $(patsubst $(SRC_DIR)/%.$(SRC_PREFIX),$(OUT_DIR)/%.$(OBJ_PREFIX),$1)

res-to-obj = $(patsubst $(RES_DIR)/%.$(RES_PREFIX),$(OUT_DIR)/%.$(OBJ_PREFIX),$1)


.PHONY: all
all: $(TARGET)
	$(RM) $(DEPEND)

.PHONY: clean
clean:
	$(RM) -r $(OUT_DIR)/*

.PHONY: announce-build-type
announce-build-type:
	@echo -e "build type: \033[31m$(BUILD_TYPE)\033[m"

$(DEPEND): $(SRCS)
	@echo Updating dependencies...
	@$(foreach i,$^,$(filter-out ,$(COMPILE.$(SRC_PREFIX))) -MT $(call src-to-obj,$i) -MM $i >> $@;)

$(OUT_DIR)/%.$(OBJ_PREFIX): $(SRC_DIR)/%.$(SRC_PREFIX)
	@mkdir -p $(dir $@)
	$(COMPILE.$(SRC_PREFIX)) $(OUTPUT_OPTION) $<

$(OUT_DIR)/%.$(OBJ_PREFIX): $(RES_DIR)/%.$(RES_PREFIX)
	$(RCC) $< $@

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPEND)
endif
-include announce-build-type

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(LINK.$(SRC_PREFIX)) $^ $(LOADLIBES) $(LDLIBS) -o $@
