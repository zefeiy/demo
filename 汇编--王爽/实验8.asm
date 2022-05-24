
assume cs:codesg

codesg segment
	mov ax,4c00h
	int 21h

start: mov ax,0
	s: nop   ;ds = 075a,cs = 076a  nop -> F6EB nop(占一个bite)
	   nop
	   
	   mov di,offset  s di = 0008
	   mov si,offset s2 si = 0020
	   mov ax,cs:[si]  ;ax = EB(EB转移)F6 F6(补码)=-10 -> jmp 0018(jmp s1)
	   mov cs:[di],ax  ;cs:0008 -> F6EB  ->>>s
						
	s0: jmp short s   
	
	s1: mov ax,0
		int 21h
		mov ax,0    
	
	s2: jmp short s1  ;跳到s1
		nop
		
codesg ends

end start
