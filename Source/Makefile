# SEGA Saturn SDK // Sample Common Base [Makefile]
.SILENT:

ifndef TOPDIR
export TOPDIR		:= $(shell cd ../ && pwd)
export TOPSRC		= $(TOPDIR)/Source
export TARGETDIR	= $(TOPDIR)/Bin
export OBJDIR		= $(TOPDIR)/Obj/$(BUILD)
endif

SOURCEDIR	+= Source
ISO	?= False

TARGET	:= $(SAMPLE)
OUTFILE	= $(TARGETDIR)/$(TARGET)
EXT		:= .elf

SATURN_BIN=$(SATURN_ROOT)/toolchain/bin
CROSS_COMPILE=$(SATURN_BIN)/saturn-sh2-elf-

CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)as
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy
MAKE=$(SATURN_BIN)/saturn-make

CFLAGS = -m2 -I$(SATURN_SAMPLE_COMMON)/Source/Headers -I$(TOPSRC)/Headers

ifeq ($(DEVCART),True)
CFLAGS += -DBUILD_USBDEVCART
endif

LDFLAGS = -m2 -Xlinker -T$(SATURN_SAMPLE_COMMON)/Source/Saturn.lnk \
	-Xlinker -Map -Xlinker $(TARGETDIR)/$(TARGET).map \
	-nostartfiles

debug:	BUILD = Debug
debug:	BUILD_TYPE = debug
debug:	BUILD_DEF = DEBUG
debug:	TARGET := $(TARGET)D
debug:	CFLAGS	+= -g -ggdb
debug:	$(TARGET)

release: BUILD = Release
release: BUILD_TYPE = release
release: BUILD_DEF = RELEASE
release: TARGET := $(TARGET)
release: LDFLAGS += -Xlinker --strip-debug
release: CFLAGS += -O2
release: $(TARGET)

ifneq ($(OBJDIR), $(CURDIR))
TARGETDIR:
	mkdir -p $(TARGETDIR)

OBJDIR:
	mkdir -p $(OBJDIR)

CFILES	:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.c))) \
			$(foreach dir,$(SATURN_SAMPLE_COMMON)/Source/Source,$(notdir $(wildcard $(dir)/*.c)))
# Ignore the crt files from the object files
ASFILES	:= $(foreach dir,$(SOURCEDIR),$(notdir $(filter-out $(wildcard $(dir)/crt*.s), $(wildcard $(dir)/*.s)))) \
			$(foreach dir,$(SATURN_SAMPLE_COMMON)/Source/Source,$(notdir $(filter-out $(wildcard $(dir)/crt*.s), $(wildcard $(dir)/*.s))))
export VPATH	:= $(foreach dir,$(SOURCEDIR),$(CURDIR)/$(dir)) $(SATURN_SAMPLE_COMMON)/Source/Source
export OBJS		:= $(CFILES:.c=.o) $(ASFILES:.s=.o)
# VPATH should allow the crt*.s files to be picked up (assuming they have not been moved)
export CRTOBJS	:= crt0.o

$(TARGET): OBJDIR TARGETDIR
	$(MAKE) -C $(OBJDIR) -f $(SATURN_SAMPLE_COMMON)/Source/Makefile $(BUILD_TYPE)
	# This seems a lot less elegant than I'd like
ifeq ($(ISO),True)
	# Thanks, The Rockin'-B
	mkdir -p $(TARGETDIR)/CD
	echo "ABSTRACT" > $(TARGETDIR)/CD/ABS.TXT
	echo "BIBLIOGRAPHY" > $(TARGETDIR)/CD/BIB.TXT
	echo "COPYRIGHT" > $(TARGETDIR)/CD/CPY.TXT
	cp $(OUTFILE).bin $(TARGETDIR)/CD/0.BIN
	printf "Making ISO\n"
	mkisofs -quiet -sysid "SEGA SATURN" -volid "SaturnApp" \
	-volset "SaturnApp" -publisher "SEGA ENTERPRISES, LTD." \
	-preparer "SEGA ENTERPRISES, LTD." -appid "SaturnApp" \
	-abstract "ABS.TXT" -copyright "CPY.TXT" -biblio "BIB.TXT" \
	-generic-boot IP.BIN -full-iso9660-filenames \
	-o $(OUTFILE).iso $(TARGETDIR)/CD
endif

else

$(TARGET): $(CRTOBJS) $(OBJS)
	printf "Linking: $(OUTFILE)$(EXT) ... "
	$(CC) -o $(OUTFILE)$(EXT) $(LDFLAGS) $(CRTOBJS) $(OBJS)
	printf "Done\n"
	printf "Converting $(OUTFILE)$(EXT) to $(OUTFILE).bin ... "
	$(OBJCOPY) -O binary $(OUTFILE)$(EXT) $(OUTFILE).bin
	printf "Done\n"

%.o: %.c
	printf "Compiling: $< ... "
	$(CC) -MMD -MP -MF $*.d $(CFLAGS) -c $< -o $@
	printf "Done\n"

%.o: %.s
	printf "Assembling: $< ... "
	$(AS) $< -o $@
	printf "Done\n"

-include $(OBJDIR)/*.d

endif

.PHONY: clean

clean:
	printf "Removing $(TARGETDIR)/$(SAMPLE)* ... "
	rm -rf $(TARGETDIR)/$(SAMPLE)*
	printf "Done\n"
	printf "Removing $(OBJDIR) ... "
	rm -rf $(OBJDIR)
	printf "Done\n"

