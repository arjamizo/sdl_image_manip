#include <stdio.h>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <string>

void print(unsigned char *tab, int size) {
  std::copy(tab, tab+size, std::ostream_iterator<int>(std::cout, "\n"));
}

void fil(unsigned char *tab, int w, int h, int bpp) {
  return;
}

void neg_without_mmx(unsigned char *tab, int w, int h, int bpp) {
  register int I=w*h;
  for(register int i=0; i<I; ++i) {
    tab[i]=~ tab[i];
  }
  return;
}

extern "C" void filter(unsigned char *tab, int w, int h, int bpp);

int main(int, char *argv[]) {
  unsigned char *tab=new unsigned char[24];
  for(int i=0; i<3*8; i+=3) {
    tab[i]=tab[i+1]=tab[i+2]=((char)~0)<<(i/3);
  }
  int w=3, h=8;
  fil(tab, w, h, 3);
  /**
   *in gdb ./main: 
   (gdb) disass main
   shows that arguments in 64-bit goes to registers
      0x0000000000400934 <+93>:    mov    -0x38(%rbp),%edx
   0x0000000000400937 <+96>:    mov    -0x34(%rbp),%ebx
   0x000000000040093a <+99>:    lea    -0x30(%rbp),%rax
   0x000000000040093e <+103>:   mov    $0x3,%ecx
   0x0000000000400943 <+108>:   mov    %ebx,%esi
   0x0000000000400945 <+110>:   mov    %rax,%rdi
   0x0000000000400948 <+113>:   callq  0x4008c4 <_Z3filPhiii>
   (gdb) disass fil
   shows that even simple function stores those arguments just after being called on stack
   Dump of assembler code for function _Z3filPhiii:
   0x00000000004008c4 <+0>:     push   %rbp
   0x00000000004008c5 <+1>:     mov    %rsp,%rbp
   0x00000000004008c8 <+4>:     mov    %rdi,-0x8(%rbp)
   0x00000000004008cc <+8>:     mov    %esi,-0xc(%rbp)
   0x00000000004008cf <+11>:    mov    %edx,-0x10(%rbp)
   0x00000000004008d2 <+14>:    mov    %ecx,-0x14(%rbp)
   0x00000000004008d5 <+17>:    leaveq
   0x00000000004008d6 <+18>:    retq
   :next steps examine stack: 
   (gdb) x/w $rbp-0x8
   0x7fffffffea08: 4294961728
   (gdb) x/w $rbp-0xc
   0x7fffffffea04: 3
   (gdb) x/w $rbp-0x10
   0x7fffffffea00: 4
   (gdb) x/w $rbp-0x14
   0x7fffffffe9fc: 3
   function arguments are on stack and in registers 
%rdi(buf), 
%esi(w), 
%edx(h), 
%ecx(bpp) 
it is useful for writing asm 64-bit insertions to 64-bit c++
   **/
  print(tab, w*h);
  filter(tab, w, h, 3);
  print(tab, w*h);
  return 0;
} 
