################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/src/fuzz.cpp \
../test/src/unit-algorithms.cpp \
../test/src/unit-allocator.cpp \
../test/src/unit-capacity.cpp \
../test/src/unit-class_const_iterator.cpp \
../test/src/unit-class_iterator.cpp \
../test/src/unit-class_lexer.cpp \
../test/src/unit-class_parser.cpp \
../test/src/unit-comparison.cpp \
../test/src/unit-concepts.cpp \
../test/src/unit-constructor1.cpp \
../test/src/unit-constructor2.cpp \
../test/src/unit-convenience.cpp \
../test/src/unit-conversions.cpp \
../test/src/unit-deserialization.cpp \
../test/src/unit-element_access1.cpp \
../test/src/unit-element_access2.cpp \
../test/src/unit-inspection.cpp \
../test/src/unit-iterator_wrapper.cpp \
../test/src/unit-iterators1.cpp \
../test/src/unit-iterators2.cpp \
../test/src/unit-json_patch.cpp \
../test/src/unit-json_pointer.cpp \
../test/src/unit-modifiers.cpp \
../test/src/unit-pointer_access.cpp \
../test/src/unit-readme.cpp \
../test/src/unit-reference_access.cpp \
../test/src/unit-regression.cpp \
../test/src/unit-serialization.cpp \
../test/src/unit-testsuites.cpp \
../test/src/unit-unicode.cpp \
../test/src/unit.cpp 

OBJS += \
./test/src/fuzz.o \
./test/src/unit-algorithms.o \
./test/src/unit-allocator.o \
./test/src/unit-capacity.o \
./test/src/unit-class_const_iterator.o \
./test/src/unit-class_iterator.o \
./test/src/unit-class_lexer.o \
./test/src/unit-class_parser.o \
./test/src/unit-comparison.o \
./test/src/unit-concepts.o \
./test/src/unit-constructor1.o \
./test/src/unit-constructor2.o \
./test/src/unit-convenience.o \
./test/src/unit-conversions.o \
./test/src/unit-deserialization.o \
./test/src/unit-element_access1.o \
./test/src/unit-element_access2.o \
./test/src/unit-inspection.o \
./test/src/unit-iterator_wrapper.o \
./test/src/unit-iterators1.o \
./test/src/unit-iterators2.o \
./test/src/unit-json_patch.o \
./test/src/unit-json_pointer.o \
./test/src/unit-modifiers.o \
./test/src/unit-pointer_access.o \
./test/src/unit-readme.o \
./test/src/unit-reference_access.o \
./test/src/unit-regression.o \
./test/src/unit-serialization.o \
./test/src/unit-testsuites.o \
./test/src/unit-unicode.o \
./test/src/unit.o 

CPP_DEPS += \
./test/src/fuzz.d \
./test/src/unit-algorithms.d \
./test/src/unit-allocator.d \
./test/src/unit-capacity.d \
./test/src/unit-class_const_iterator.d \
./test/src/unit-class_iterator.d \
./test/src/unit-class_lexer.d \
./test/src/unit-class_parser.d \
./test/src/unit-comparison.d \
./test/src/unit-concepts.d \
./test/src/unit-constructor1.d \
./test/src/unit-constructor2.d \
./test/src/unit-convenience.d \
./test/src/unit-conversions.d \
./test/src/unit-deserialization.d \
./test/src/unit-element_access1.d \
./test/src/unit-element_access2.d \
./test/src/unit-inspection.d \
./test/src/unit-iterator_wrapper.d \
./test/src/unit-iterators1.d \
./test/src/unit-iterators2.d \
./test/src/unit-json_patch.d \
./test/src/unit-json_pointer.d \
./test/src/unit-modifiers.d \
./test/src/unit-pointer_access.d \
./test/src/unit-readme.d \
./test/src/unit-reference_access.d \
./test/src/unit-regression.d \
./test/src/unit-serialization.d \
./test/src/unit-testsuites.d \
./test/src/unit-unicode.d \
./test/src/unit.d 


# Each subdirectory must supply rules for building sources it contributes
test/src/%.o: ../test/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


