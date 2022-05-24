
;-----------------------------------------------------
; 模块化子程序，将指定字符以0 结尾的字符串写入显存
;---------------------------------------------------
assume cs :codesg,ds:datasg

datasg segment
	db 'Welcome to masm!',0
datasg ends


codesg segment
start:
	mov ax,datasg
	mov ds,ax
	mov dh,8 ;行号
	mov dl,3 ;列号
	mov cl,0 
	mov bl,2  ;颜色
	mov si,0
	call show_str
	
	mov ax,4c00h
	int 21h

show_str:
	mov ax,0b800h
	mov es,ax
	mov ax,160
	mul dh
	mov di,ax    ;获得行号计算后的偏移量
	;add dl,dl		;;获得列号偏移量


s:	mov cl,0   ;我的debug点，注意loop指令会改变cx的值，所以要改变 cl的值
	mov bh,ds:[si]
	mov ch,bh
	jcxz ok
	mov es:[di+6],bh
	mov es:[di+7],bl
	inc si
	add di,2
	loop s
ok: ret

codesg ends
end start

