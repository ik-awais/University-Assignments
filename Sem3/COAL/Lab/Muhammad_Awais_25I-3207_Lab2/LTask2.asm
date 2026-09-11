.386
.model flat, stdcall
.stack 4096
.data	
	num1 DW 25h
.code
	main PROC
	MOV bx, num1
	MOV al, bl
	main ENDP
	END main