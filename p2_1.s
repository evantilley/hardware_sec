@P2_1.s ARM Assembly Language Program To Add Some Data and Store the SUM in R3.
.global _start
_start: MOV R1, #0x25 @ R1 = 0x25
MOV R2, #0x34 @ R2 = 0x34
ADD R3, R2, R1 @ R3 = R2 + R1
MOV R7, #1
SVC 0
