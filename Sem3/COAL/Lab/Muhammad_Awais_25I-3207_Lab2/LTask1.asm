.386
.model flat, stdcall
.stack 4096
.data	
	val1 DW 1234h
.code
	main PROC
	MOV ax, val1
	main ENDP
	END main