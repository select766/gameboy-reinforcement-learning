# find *.c file to SRCS in Windows
SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)

mountaincar.gb: $(SRCS) $(HEADERS) bg_map_offseted.c trained.c
	..\..\gbdk-win64\gbdk\bin\lcc.exe -o mountaincar.gb gb_main.c mountaincar.c qlearning.c trained.c run_pretrained.c train.c display.c

bg_map_offseted.c: bg_map.c
	python bg_map_offset.py bg_map.c bg_map_offseted.c 128

trained.c: trained.bin
	python trained_to_c.py trained.bin trained.c pc_trained_params

mountaincar.exe: $(SRCS) $(HEADERS)
	gcc -o mountaincar.exe pc_main.c mountaincar.c qlearning.c
