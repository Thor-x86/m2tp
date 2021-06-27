ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

default:
ifeq ($(detected_OS),Windows)
	make clean; make build
else
	make clean && make build
endif
.PHONY: default

build:
ifeq ($(detected_OS),Windows)
	cd .cmake; cmake --build .
else
	cd .cmake && make -j$(nproc)
endif
.PHONY: build

configure:
ifeq ($(detected_OS),Windows)
	if not exist ".cmake" mkdir .cmake; cd .cmake; cmake ..
else
	mkdir -p .cmake out; cd .cmake && cmake ..
endif
.PHONY: configure

clean:
ifeq ($(detected_OS),Windows)
	rm out\libm2tp-member.dll; rm out\libm2tp-leader.dll; rmdir .cmake\CMakeFiles
else
	rm -rf out/libm2tp-* .cmake/CMakeFiles
endif
.PHONY: clean

clean_all:
ifeq ($(detected_OS),Windows)
	rmdir /Q /S .cmake; rmdir /Q /S build; rmdir /Q /S out
else
	rm -rf .cmake build out
endif
.PHONY: clean_all

rebuild:
ifeq ($(detected_OS),Windows)
	make clean_all; make configure; make build
else
	make clean_all && make configure && make build
endif
.PHONY: rebuild

test-unit:
ifeq ($(detected_OS),Windows)
	.cmake\m2tp-common-test-unit.exe
	.cmake\m2tp-member-test-unit.exe
	.cmake\m2tp-leader-test-unit.exe
else
	.cmake/m2tp-common-test-unit
	.cmake/m2tp-member-test-unit
	.cmake/m2tp-leader-test-unit
endif
