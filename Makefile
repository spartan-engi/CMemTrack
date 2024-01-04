src := main.c mem.c
obj := $(patsubst %.c, objs/%.o, $(src))
tgt := main.exe

# object folder
objDir := objs

# compiler flags
comFlg := -c -g
# linker flags
linFlg := -g

build: ${objDir} ${tgt}

clean:
	rm -f "${tgt}"
	rm -f ${objDir}/*

# makefile can actually make things now, from scratch
${objDir}:
	mkdir "${objDir}"

# fuses all objects into main
${tgt}: ${obj}
	gcc ${obj} ${linFlg} -o ${tgt}

# compiles each source to objs/
objs/%.o: %.c
	gcc $< ${comFlg} -o $@
