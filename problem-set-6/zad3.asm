[section .data]
			writenum: db '%d ',0
			number dd 1
[section .text]
			 global _main
			 extern _printf
_main:
			push ebx						;save beginning values of ebx, edx
			push edx
			
			mov eax, [number]		;print 2
			push eax			
			push writenum
			call _printf
			add esp, 8
			jmp num_loop				;start loop 3, 5... 10000
			
end:		
			pop edx						;set ebx, edx as at the beginning
			pop ebx
			mov eax,0					;clear eax
			ret								;end
			
num_loop:
			inc word[number]			;increment number
			mov eax, [number]		;check scope
			cmp eax, 10000
			je end
			mov word[divisor], 2 	;reset divisor
			jmp check_prime			;check prime
			; jmp end

check_prime:
			mov eax, [number]		;compare divisor with number (if equal - number is prime)
			mov ebx, [divisor]
			cmp eax, ebx
			je is_prime
		
			mov edx,0					;divide number by divider (if remainder is == 0, number is not prime)
			mov eax, [number]
			cdq
			mov ebx, [divisor]
			div ebx
			cmp edx,0
			je num_loop
			
			inc word[divisor]			
			jmp check_prime

is_prime:									;if number is equal, show on screen
			mov eax, [number]
			push eax
			push writenum
			call _printf
			add esp,8
			jmp num_loop