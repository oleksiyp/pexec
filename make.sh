nasm exec_cmd.asm -o exec_cmd
gcc bin2c.c -o bin2c
./bin2c -i exec_cmd -o exec_cmd.h -l 10 -a exec_cmd
rm bin2c
gcc pexec.c -O3 -Wunused-result -o pexec
gcc hw.c -o hw
