         ;�����嵥6-1
         ;�ļ�����c06_mbr.asm
         ;�ļ�˵����Ӳ����������������
         ; 
      
         jmp near start
         
  mytext db 'L',0x07,'a',0x07,'b',0x07,'e',0x07,'l',0x07,' ',0x07,'o',0x07,\
            'f',0x07,'f',0x07,'s',0x07,'e',0x07,'t',0x07,':',0x07
  number db 0,0,0,0,0
  
  start:
         mov ax,0x07c0                  ;�������ݶλ���ַ 
         mov ds,ax
         
         mov ax,0xb800                 ;���ø��Ӷλ���ַ��Ҳ���ǽ�ES�Ĵ���ָ���Դ����ʼ��ַ 
         mov es,ax
         
         cld													 ;���������־����DF��־λ���㣬�������������
         mov si,mytext  						   ;SI��DS������ݶεĵ�ַ DS:SI �������ݵ���ʵ�����ַ                
         mov di,0    									 ;DI��ES����Դ�������ַ ES:DI �����Դ����ʵ�����ַ
         mov cx,(number-mytext)/2      ;ʵ���ϵ��� 13
         rep movsw 										 ;ѭ��movsw��ֱ��cx�Ĵ�������Ϊ0��repָ����������ͣ�
     
         ;�õ�����������ƫ�Ƶ�ַ
         mov ax,number
         
         ;���������λ
         mov bx,ax
         mov cx,5                      ;ѭ������ 
         mov si,10                     ;���� 
  digit: 
         xor dx,dx
         div si
         mov [bx],dl                   ;������λ
         inc bx 											 ;ʹbx�Ĵ������ֵ��1
         loop digit
         
         ;��ʾ������λ
         mov bx,number 								 ;��number�Ļ���ַ���͸�BX�Ĵ���
         mov si,4                      ;bx+si �õ��ַ�����ÿһ���ַ���SI��4�ݼ���0����������Ҫ����ʾ��λ�ϵ�����
   show:
         mov al,[bx+si]
         add al,0x30									 ;�õ�����Ӧ��ASCII��
         mov ah,0x04									 ;��Ӧ����ɫ����
         mov [es:di],ax								 ;AX����һ���������֣�ǰ8λ����ʾ����ֵ����8λ���ַ���ASCII��
         add di,2		 			     ;DI�Ĵ�����֮ǰ�ù��������ڡ�Label offset:�� �ַ������棬�պ���������number�Ļ���ַ��������ʾ
         dec si						     ;SI-1,��number����Ļ���ַ����λ����λ��decָ���Ӱ��SF��־λ����SI�Ĵ�����ֵΪ0��ʱ��SF�ı�־λ��1
         jns show							 ;�ж�SF��־λ�Ƿ�Ϊ0����SF��־λ��Ϊ0������ִ��show���Ĵ��롣��SF��־λΪ0������������ָ��ִ����һ��ָ�
         
         mov word [es:di],0x0744		;���ֽ�0x07�Ǻڵװ��ֵ����ԣ����ֽ�0x44���ַ���D����ASCII��

         jmp near $   							;�൱�� infi: jmp near infi

  times 510-($-$$) db 0             ; ����512�ֽ��У���Ҫ�������ֽ�����Щ��
                   db 0x55,0xaa     ;һ����Ч��������������������ֽڱ�����0x55 0xaa