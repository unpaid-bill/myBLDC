import os
import math as m

def main():
    TYPE = "uint32_t"
    TAB  = "    "
    file_path = "./inc/sine_table.h"
    COLS = 4
    with open(file_path, 'w') as f:
        
        f.write("#ifndef __SINE_TABLE_H__\n")
        f.write("#define __SINE_TABLE_H__\n")
        f.write("\n#include \"stm32f103xb.h\"\n")
        f.write(f"\nconst {TYPE} sin_table[] = {{\n")
        
        count = 0
        first = True
        for x in range(0,360, 1):
            x_rad = (x * m.pi) / 180
            sign = " "
            result = m.sin(x_rad)
            if(result < 0):
                sign = "-"

            if(not first):
                f.write(",")
                if((count % COLS) == 0):
                    f.write("\n")
            else:
                first = False
                
            f.write("{}{}{:.15f}".format(TAB, sign, abs(result)))
            count += 1
        f.write("\n};\n")

        f.write("\n#endif /* __SINE_TABLE_H__ */\n")

    with open(file_path, 'r') as f:
        print(f.read())

if __name__ == "__main__":
    main()