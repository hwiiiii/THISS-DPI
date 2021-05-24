addi x5, x5, 15
addi x6, x6, 4
addi x28, x28, -2
addi x29, x29, -1
addi x30, x30, 256

mul x7, x5, x28
mulw x7, x5, x28
mulh x7, x5, x30
mulhsu x7, x28, x29
mulhu x7, x15, x29

div x7, x5, x28
divw x7, x5, x28
divu x7, x5, x6
divu x7, x5, x29
divuw x7, x30, x6
divuw x7, x30, x29

rem x7, x5, x28
remw x7, x5, x28
remu x7, x5, x6
remu x7, x5, x29
remuw x7, x30, x6
remu x7, x30, x29
