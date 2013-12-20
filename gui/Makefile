TARGET:=simple_csg

.PHONY: all
all: $(TARGET)

include $(ROOT)/Makefile.common

SRCS:=$(notdir $(wildcard $(SRC_ROOT)/*.cpp))
OBJS:=$(SRCS:.cpp=.o)
DEPS:=$(OBJS:.o=.d)

EXT_LIBS:=glew gl sdl2

CPPFLAGS+=$(shell pkg-config --cflags $(EXT_LIBS))
LDFLAGS+=$(shell pkg-config --libs $(EXT_LIBS))

include $(DEPS)

vpath %.cpp $(SRC_ROOT)
vpath %.a $(OBJ_ROOT)/math
vpath %.a $(OBJ_ROOT)/csg

$(TARGET): $(OBJS) libcsg.a libmath.a
	$(LD) $(LDFLAGS) $^ -o $@
