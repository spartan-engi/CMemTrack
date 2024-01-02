src := main.c mem.c
obj := $(patsubst %.c, objs/%.o, $(src))
tgt := main.exe

# compiler flags
comFlg := -c -g
# linker flags
linFlg := -g

build: ${tgt}

clean:
	rm -f ${obj} ${tgt}

# fuses all objects into main
${tgt}: ${obj}
	gcc ${obj} ${linFlg} -o ${tgt}

# compiles each source to objs/
objs/%.o: %.c
	gcc $< ${comFlg} -o $@
