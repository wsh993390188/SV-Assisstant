.model flat,stdcall	  ;				//使用flat模式,(4GB平坦内存模式),stdcall标准调用约定
public Memory_Map
.code 
Memory_Map PROC
.Init
	pushad;
	mov eax,esp;
	add eax, 8;						//第一个参数传递给edi
	mov edi, eax;					// input address	
	mov eax,esp;
	add eax, 12;					//第二个参数传递给edi
	mov esi, eax;					// input address
.do_e820:

	xor ebx, ebx;					// ebx must be 0 to start
	xor bp, bp;						// keep an entry count in bp
	mov edx, 0x534D4150;			// Place "SMAP" into edx
	mov eax, 0xE820;
	mov [es:edi + 20], 1;			// force a valid ACPI 3.X entry
	mov ecx, 24;					// ask for 24 bytes
	int 0x15;
	jc short .memory_map_failed;	// carry set on first call means "unsupported function"
	mov edx, 0x534D4150;			// Some BIOSes apparently trash this register?
	cmp eax, edx;					// on success, eax must have been reset to "SMAP"
	jnz short .memory_map_failed;
	test ebx, ebx;					// ebx = 0 implies list is only 1 entry long (worthless)
	jz short .memory_map_failed;
	jmp short .jmpin;

.e820lp:
	mov eax, 0xE820;				// eax, ecx get trashed on every int 0x15 call
	mov [es:edi + 20], 1;			// force a valid ACPI 3.X entry
	mov ecx, 24;					// ask for 24 bytes again
	int 0x15;
	jc short .e820f;				// carry set means "end of list already reached"
	mov edx, 0x534D4150;			// repair potentially trashed register

.jmpin:
	jcxz .skipent;					// skip any 0 length entries
	cmp cl, 20;						// got a 24 byte ACPI 3.X response?
	jbe short .notext;
	test [es:edi + 20], 1;			// if so: is the "ignore this data" bit clear?
	jz short .skipent;	

.notext:
	mov ecx, [es:edi + 8];			// get lower uint32_t of memory region length
	or ecx, [es:edi + 12];			// "or" it with upper uint32_t to test for zero
	jz .skipent;					// if length uint64_t is 0, skip entry
	inc bp;							// got a good entry: ++count, move to next storage spot
	add edi, 24;

.skipent:
	test ebx, ebx;					// if ebx resets to 0, list is complete
	jnz short .e820lp;

.e820f:
	mov [es:si], bp;	// store the entry count
	clc; 							// there is "jc" on end of list to this point, so the carry must be cleared
	jmp E820exit;					// emulator works well 
.memory_map_failed:
	stc;							// "function unsupported" error exit
	jmp E820exit;	
.E820exit:
	popad;
	ret;
Memory_Map ENDP
end
		