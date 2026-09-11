.386
.model flat, stdcall
.stack 4096
.data	
	smallVal DB 7Fh
	storeHere DB 0
.code
	main PROC
	MOV al, smallVal
	INC al
	MOV storeHere, al
	main ENDP
	END main