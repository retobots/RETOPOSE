Import("env")

def make_hex(source, target, env):
    elf_path = str(target[0])
    hex_path = elf_path.replace(".elf", ".hex")
    objcopy = env.subst("$OBJCOPY")
    env.Execute(f'"{objcopy}" -O ihex "{elf_path}" "{hex_path}"')

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", make_hex)