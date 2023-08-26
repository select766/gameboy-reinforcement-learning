# find *.c file to SRCS in Windows
SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)

mountaincar.gb: $(SRCS) $(HEADERS) bg_map_offseted.c
	..\..\gbdk-win64\gbdk\bin\lcc.exe -o mountaincar.gb gb_main.c mountaincar.c qlearning.c

bg_map_offseted.c: bg_map.c
	python bg_map_offset.py bg_map.c bg_map_offseted.c 128

mountaincar.exe: $(SRCS) $(HEADERS)
	gcc -o mountaincar.exe pc_main.c mountaincar.c qlearning.c
