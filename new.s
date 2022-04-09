.data
var1: .word 3 /* here, I'm storing a value of 3 into a variable var1. */

	.global _start

_start:
	/* first, we load the memory address of var into R0 */
	LDR R0, =var1

lockit:
	/* now, load a value from the address in R0 into R1
	and update the corresponding exclusive monitors */
	LDREX R1, [R0]
	
	/* ok, let's now do something with the value we loaded in */
	MOV R5, #4
	/* we are adding 4 to R1 */
	ADD R1, R1, R5
	/* ok now we need to attempt to store the updated value back
	into the R0 address */
	STREX R2, R1, [R0]
	/* was this store successful? the result will be in R2 
	if the store was successful the result will be equal to 0*/
	CMP R2, #1 
	BEQ lockit /* i don't know if this comparison is actually working, need to look into this more by having multiple threads run at the same time and using gdb to see if value in R2 is ever 1 (i.e. the store failed) */
	MOV R7, #1
	SVC 0
