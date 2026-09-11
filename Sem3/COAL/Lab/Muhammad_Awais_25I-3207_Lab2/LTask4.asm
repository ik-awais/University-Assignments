.386
.model flat, stdcall
.stack 4096
.data	
	
.code
	main PROC
	MOV DL, 05h
	MOV DH, 0Ah
	INC DL
	INC DH
	main ENDP
	END main
	exit