Import("env")

def make_hex(source, target, env):
    elf_path = str(target[0])
    hex_path = elf_path.replace(".elf", ".hex")
    # Lấy đường dẫn gốc của trình biên dịch GCC RISC-V để suy ra objcopy
    cc_path = env.subst("$CC")
    objcopy = cc_path.replace("gcc", "objcopy")
    
    env.Execute(f'"{objcopy}" -O ihex "{elf_path}" "{hex_path}"')

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", make_hex)