################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Automated.c \
../Basic.c \
../CUError.c \
../Console.c \
../MyMem.c \
../TestDB.c \
../TestRun.c \
../Util.c \
../forensics.c \
../forensicsMain.c \
../testForensics.c 

C_DEPS += \
./Automated.d \
./Basic.d \
./CUError.d \
./Console.d \
./MyMem.d \
./TestDB.d \
./TestRun.d \
./Util.d \
./forensics.d \
./forensicsMain.d \
./testForensics.d 

OBJS += \
./Automated.o \
./Basic.o \
./CUError.o \
./Console.o \
./MyMem.o \
./TestDB.o \
./TestRun.o \
./Util.o \
./forensics.o \
./forensicsMain.o \
./testForensics.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -I"C:\Users\angel\Eclipse\The Bone Mapper - Forensic Case Management Engine" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./Automated.d ./Automated.o ./Basic.d ./Basic.o ./CUError.d ./CUError.o ./Console.d ./Console.o ./MyMem.d ./MyMem.o ./TestDB.d ./TestDB.o ./TestRun.d ./TestRun.o ./Util.d ./Util.o ./forensics.d ./forensics.o ./forensicsMain.d ./forensicsMain.o ./testForensics.d ./testForensics.o

.PHONY: clean--2e-

