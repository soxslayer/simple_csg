BUILD?=0

ifeq ($(BUILD),0)

ROOT:=$(shell pwd)
BUILD_DIR:=build

.DEFAULT_GOAL:=all

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

%::
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	@$(MAKE) -C $(BUILD_DIR) -f $(ROOT)/Makefile --no-print-directory BUILD=1 ROOT=$(ROOT) $(MAKECMDGOALS)

else

define TARGET_TEMPLATE
PHONY_TARGETS+=$(1)
$(1): $(2)
	@echo -e '\e[0;33mBuilding \e[1;34m$(1)\e[0;0m'
	@[ -d $(3) ] || mkdir $(3)
	@$$(MAKE) -C $(3) -f $$(ROOT)/$(3)/Makefile --no-print-directory ROOT=$$(ROOT) SRC_ROOT=$$(ROOT)/$(3) OBJ_ROOT=$(shell pwd)
	@echo -e '\e[0;32mComplete\e[0;0m'
endef

$(eval $(call TARGET_TEMPLATE,simple_csg,libmath.a libcsg.a,gui))
$(eval $(call TARGET_TEMPLATE,libmath.a,,math))
$(eval $(call TARGET_TEMPLATE,libcsg.a,,csg))

.PHONY: $(PHONY_TARGETS)

endif
