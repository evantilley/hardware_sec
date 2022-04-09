/* the global directive is the entry point */
.global _start
_start:
	mov r0, #2
	mov r7, #1
	svc 0
