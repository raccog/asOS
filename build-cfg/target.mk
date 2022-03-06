ifeq (, $(TARGET))
    $(error Empty target system in subdirectory $(shell pwd))
endif