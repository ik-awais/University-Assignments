.386
.model flat, stdcall
.stack 4096
.data	
	counter DW 0
.code
	main PROC
	MOV cx, counter
	INC cx
	INC cx
	INC cx
	DEC cx
	main ENDP
	END main