
assume cs:codesg,ss:stacksg,ds:datasg


datasg segment
	db '1. display      '
	db '2. brows        '
	db '3. replace      '
	db '4. modify       '
datasg ends

stacksg segment
	dw 0,0,0,0,0,0,0,0
stacksg ends


codesg segment
start:	mov ax,datasg
		mov ds,ax			
		  
		mov ax,stacksg
		mov ss,ax
		mov sp,16	
		  
		mov bx,0
		mov cx,4
	s:  push cx
		mov si,3
		  
		mov cx,4 
	s2: mov al,[si+bx]
		and al,11011111b
		mov [si+bx],al
		inc si 
		loop s2
		
		add bx,16
		pop cx
		loop s
		
		mov ax,4c00h
		int 21h

codesg ends

end start