#
# Makefile for the input core drivers.
#

# Each configuration option enables a list of files.

obj-$(CONFIG_INPUT)		+= input-core.o
input-core-y := input.o input-compat.o input-mt.o ff-core.o

obj-$(CONFIG_INPUT_FF_MEMLESS)	+= ff-memless.o
obj-$(CONFIG_INPUT_POLLDEV)	+= input-polldev.o
obj-$(CONFIG_INPUT_SPARSEKMAP)	+= sparse-keymap.o
obj-$(CONFIG_INPUT_MATRIXKMAP)	+= matrix-keymap.o

obj-$(CONFIG_INPUT_MOUSEDEV)	+= mousedev.o
obj-$(CONFIG_INPUT_JOYDEV)	+= joydev.o
obj-$(CONFIG_INPUT_EVDEV)	+= evdev.o
obj-$(CONFIG_INPUT_EVBUG)	+= evbug.o

obj-$(CONFIG_INPUT_KEYBOARD)	+= keyboard/
obj-$(CONFIG_INPUT_MOUSE)	+= mouse/
obj-$(CONFIG_INPUT_JOYSTICK)	+= joystick/
obj-$(CONFIG_INPUT_TABLET)	+= tablet/
obj-$(CONFIG_INPUT_TOUCHSCREEN)	+= touchscreen/
obj-$(CONFIG_INPUT_MISC)	+= misc/

obj-$(CONFIG_INPUT_APMPOWER)	+= apm-power.o
obj-$(CONFIG_INPUT_KEYRESET)	+= keyreset.o
obj-$(CONFIG_INPUT_KEYCOMBO)	+= keycombo.o

#TINNO BEGIN
# guomingyi add for fingerprint start.
#lijian add for Fingerprint2-N
$(warning --singyue-user----CONFIG_TINNO_FINGERPRINT2_N--------------$(CONFIG_TINNO_FINGERPRINT2_N)--)
ifneq ($(strip $(CONFIG_TINNO_FINGERPRINT2_N)),)
  $(warning --singyue-user------------------Fingerprint2-N---)
  include $(PWD)/vendor/tinno/Fingerprint2-N/kernel_macro_cfg
else
  $(warning --singyue-user------------------Fingerprint-N---)
  include $(PWD)/vendor/tinno/Fingerprint-N/kernel_macro_cfg
endif
ifneq ($(strip $(TINNO_FINGERPRINT_SUPPORT)),)
obj-y += ../../../../$(kernel_fp_source_dir)/
endif
#TINNO END
