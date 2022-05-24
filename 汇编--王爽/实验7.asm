

assume cs:codesg,ds:datasg,ss:stacksg

datasg segment
db '1975','1976','1977','1978','1979','1980','1981','1982','1983'
db '1984','1975','1986','1987','1988','1989','1990','1991','1992'
db '1993','1994','1995'

dd 16,22,382,1356,2390,8000,16000,24486,50065,97479,140417,197514
dd 345980,590827,803530,1183000,1843000,2759000,3753000,4649000,5937000

dw 3,7,9,13,28,38,130,220,476,778,1001,1442,2258,2793,4037,5635,8226
dw 11542,14430,15257,17800
datasg ends

table segment
	db 21 dup('year summ ne ?? ') ;16个字节
table ends

stacksg segment
	dw 8 dup(0)
stacksg ends


codesg segment
	start:
		mov sp,16
		
		mov ax,table
		mov es,ax
		mov bp,0
		mov di,168
		mov bx,0
		
		mov cx,21
	s:  push cx
		mov si,0
		
		mov cx,4
	s1: mov al,ds:[bx+si]
		mov es:[bp+si],al
		inc si
		loop s1
		
		add si,2
		mov ax,ds:[bx+84]
		mov word ptr es:[bp+si],ax
		
		add si,2
		mov ax,ds:[84+di]
		mov es:[bp+si],ax
		
		add si,2
		
		
		add bx,4
		add bp,16
		add di,2
		
		pop cx
		loop s
		
		

codesg ends
	
end start


