dts-dirs += apm
dts-dirs += arm
dts-dirs += cavium

DTS_DIR := qcom
ifeq ($(CONFIG_PROJECT_P7701),y)
DTS_DIR := p7701
endif

ifeq ($(CONFIG_PROJECT_P7705),y)
DTS_DIR := p7705
endif

ifeq ($(CONFIG_PROJECT_P7203),y)
DTS_DIR := p7203
endif

ifeq ($(CONFIG_PROJECT_P7201),y)
DTS_DIR := p7201
endif

ifeq ($(CONFIG_PROJECT_I9051),y)
DTS_DIR := i9051
endif

ifeq ($(CONFIG_PROJECT_P7771),y)
DTS_DIR := p7771
endif

ifeq ($(CONFIG_PROJECT_V11bn),y)
DTS_DIR := v11bn
endif

ifeq ($(CONFIG_PROJECT_V12bn),y)
DTS_DIR := v12bn
endif

ifeq ($(CONFIG_PROJECT_V11bnlite),y)
DTS_DIR := v11bnlite
endif

ifeq ($(CONFIG_PROJECT_V12bnlite),y)
DTS_DIR := v12bnlite
endif

ifeq ($(CONFIG_PROJECT_V3943),y)
DTS_DIR := v3943
endif

ifeq ($(CONFIG_PROJECT_V12cn_s),y)
DTS_DIR := v12cn_s
endif

dts-dirs += $(DTS_DIR)

subdir-y	:= $(dts-dirs)
