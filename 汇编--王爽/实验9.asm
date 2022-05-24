assume cs:codesg,ds:datasg,ss:stacksg

datasg segment
	db 'Welcome to masm!'  ;16个字节
	db 2,00100100b,00000001b
datasg ends

stacksg segment
	db 16 dup(0)
stacksg ends	

codesg segment
start:
		mov ax,datasg
		mov ds,ax
		mov ax,0b800h
		mov es,ax
		
		mov di,16

		mov bx,0
		mov si,160*12+32*2
		
		mov cx,3
	s1: push cx
		mov dh,[di]
		
		mov cx,16
	s:	mov dl,ds:[bx]   
		 ;组装完成
		
		mov es:[si],dx
		add si,2
		inc bx
		loop s
		
		pop cx
		inc di
		add si,80h
		sub bx,bx
		loop s1
		
	mov ax,4c00h
	int 21h
codesg ends
end start
