[section .data]
			consolein: db '%d',0
			
			consoleout: db '%u%u%u%u',10,0


					
			result 	dd 0
						dd 0
						dd 0
						dd 0
			
			add dword [result], 1

			current 	dd 0
						dd 0
						dd 0
						dd 0
						
			add dword [current], 1

									
[section .text]
			 global _main
			 extern _printf
			 extern _scanf
_main:

			push result
			push consolein
			call _scanf
			add esp,8
			
			mov eax, [result]
			mov [current], eax
			dec word [current]
			
			mov eax, 0
			
			; jmp factorial_loop
			
			
factorial_loop: 
	
		mov 	ebx, [current]			; assign current to eax
		mov		eax, [result]
		
		cmp		ebx, 1					; if(ebx == 1)
		je		end						
		
		mul 	ebx
		mov		[result], eax
		mov 	ecx, edx
		
		mov 	eax, [result+4]
		mul		ebx
		add		eax, ecx
		mov		[result+4], eax
		mov 	ecx, edx
		
		mov 	eax, [result+8]
		mul		ebx
		add		eax, ecx
		mov		[result+8], eax
		mov 	ecx, edx
		
		mov 	eax, [result+12]
		mul		ebx
		add		eax, ecx
		mov		[result+12], eax
		
		
		cmp		edx, 0
		jne 	overflow
		
		
		dec 	word [current]			; current--
		jmp 	factorial_loop       		; return to start
		
		
overflow: 		
			
end:				
			push dword[result]
			push dword[result + 4]
			push dword[result + 8]
			push dword[result + 12]
			push consoleout
			call _printf
			add esp,20
			mov eax,0					;clear eax
			ret								;end
