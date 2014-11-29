SOURCES := part1.c part2.c mips.c
HEADERS := elf.h types.h mips.h

ASM_TESTS := simple insts rt3 rt13 rt25

all: mips $(addprefix mipscode/, $(ASM_TESTS))

.PHONY: disasmtest runtest %_test

mipscode/%.o: mipscode/%.s
	mips-gcc -c -o $@ $<

$(addprefix mipscode/, $(ASM_TESTS)): %: %.s mipscode/mips.ld
	mips-gcc -o $(patsubst %.s, %.o, $<) -c $<
	mips-ld -o $@ $(patsubst %.s, %.o, $<) -T mipscode/mips.ld

mips: $(SOURCES) $(HEADERS)
	gcc -g -Wall -Werror -Wfatal-errors -O2 -o $@ $(SOURCES)

disasmtest: mips mipscode/insts
	./mips -d mipscode/insts > insts.dump
	@diff insts.dump mipscode/insts.dump && echo "DISASSEMBLY TEST PASSED!" || echo "DISASSEMBLY TEST FAILED!"

disasmtest: mips mipscode/insts
	./mips -d mipscode/insts > insts.dump
	@diff insts.dump mipscode/insts.dump && echo "DISASSEMBLY TEST PASSED!" || echo "DISASSEMBLY TEST FAILED!"


%_test: mipscode/% mipscode/%.trace
	./mips -r $< > test.trace
	@diff test.trace $(addsuffix .trace, $(basename $<)) && echo "$@ PASSED" || echo "$@ FAILED"

runtest: mips insts_test rt3_test rt13_test rt25_test
	@echo "Tests Complete"

clean:
	rm -f mips $(addprefix mipscode/,  $(ASM_TESTS)) $(addsuffix .o, $(addprefix mipscode/,  $(ASM_TESTS))) *.dump *.trace
