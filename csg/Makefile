TARGET:=libcsg.a

.PHONY: all
all: $(TARGET)

include $(ROOT)/Makefile.common

SRCS:=$(notdir $(wildcard $(SRC_ROOT)/*.cpp))
OBJS:=$(SRCS:.cpp=.o)
DEPS:=$(OBJS:.o=.d)

include $(DEPS)

vpath %.cpp $(SRC_ROOT)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
