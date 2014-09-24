__asm__(".code16\n");
__asm__("jmpl $0x0000, $main\n");

void main() {
}

File: test.ld
Entry(main);
SECTIONS
{
 . = 0x7C00;
 .text : AT(0x7C00)
  {
	*(.text);
	}
	.sig : AT(0x7DFE)
	{
		SHORT(0xaa55);
	}
}

