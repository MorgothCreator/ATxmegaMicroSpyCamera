################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../cmd_parser.cpp \
../dev_mmcsd.cpp \
../dma.cpp \
../io_spi.cpp \
../io_twi.cpp \
../io_usart.cpp \
../main.cpp \
../dev_ov7690.cpp \
../pm.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
cmd_parser.o \
dev_mmcsd.o \
dma.o \
io_spi.o \
io_twi.o \
io_usart.o \
main.o \
dev_ov7690.o \
pm.o

OBJS_AS_ARGS +=  \
cmd_parser.o \
dev_mmcsd.o \
dma.o \
io_spi.o \
io_twi.o \
io_usart.o \
main.o \
dev_ov7690.o \
pm.o

C_DEPS +=  \
cmd_parser.d \
dev_mmcsd.d \
dma.d \
io_spi.d \
io_twi.d \
io_usart.d \
main.d \
dev_ov7690.d \
pm.d

C_DEPS_AS_ARGS +=  \
cmd_parser.d \
dev_mmcsd.d \
dma.d \
io_spi.d \
io_twi.d \
io_usart.d \
main.d \
dev_ov7690.d \
pm.d

OUTPUT_FILE_PATH +=ATxmegaMicroSpyCamera.elf

OUTPUT_FILE_PATH_AS_ARGS +=ATxmegaMicroSpyCamera.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



















./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"$(PackRepoDir)\atmel\XMEGAE_DFP\1.0.30\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega32e5 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="ATxmegaMicroSpyCamera.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega32e5  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "ATxmegaMicroSpyCamera.elf" "ATxmegaMicroSpyCamera.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "ATxmegaMicroSpyCamera.elf" "ATxmegaMicroSpyCamera.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "ATxmegaMicroSpyCamera.elf" > "ATxmegaMicroSpyCamera.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "ATxmegaMicroSpyCamera.elf" "ATxmegaMicroSpyCamera.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "ATxmegaMicroSpyCamera.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "ATxmegaMicroSpyCamera.elf" "ATxmegaMicroSpyCamera.a" "ATxmegaMicroSpyCamera.hex" "ATxmegaMicroSpyCamera.lss" "ATxmegaMicroSpyCamera.eep" "ATxmegaMicroSpyCamera.map" "ATxmegaMicroSpyCamera.srec" "ATxmegaMicroSpyCamera.usersignatures"
	