-include nemu/Makefile.git

default:
	@echo "Please run 'make' under any subprojects to compile."
 
clean:
	-$(MAKE) -C nemu clean
	-$(MAKE) -C nexus-am clean
	-$(MAKE) -C nanos-lite clean
	-$(MAKE) -C navy-apps clean

submit: clean
	git gc
	cd .. && tar cj $(shell basename `pwd`) > $(STU_ID).tar.bz2

count: 
	@echo "代码行数统计："
	@find nemu/ -type f \( -name '*.c' -o -name '*.h' \) -exec grep -v '"^\s*$"' {} \; | wc -l


.PHONY: default clean submit
