#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef int       INT;

#define MaxMem  65535
#define Byte16  65535

int    un, go, db;
int    sp, pc, bp, aa, bb;
int    C, Z, N, O;
unsigned char  mem[MaxMem+1];
unsigned char  dis[32];

void Error(char* mesg)
{
   fprintf(stderr, "Error [0x%04x]:  %s\n", pc, mesg);
//   exit(1);
}

int SetN(INT x)
{
   return N= (x & 0x8000) ? 1 : 0;
}

int SetZ(INT x)
{
   return Z= (x & 0xFFFF)==0 ? 1 : 0;
}

int SetO(INT x)
{
   return O= x ? 1 : 0;
}

int SetC(INT x)
{
   return C= x & 0x10000 ? 1 : 0;
}

int ReadByte(int addr)
{
   return mem[addr & MaxMem];
}

void WriteByte(int addr, int x)
{
   mem[addr & MaxMem]= x;
}

int ReadInt(int addr)
{
   addr&= MaxMem;
   return ReadByte(addr) | (ReadByte(addr+1)<<8);
}

void WriteInt(int addr, int x)
{
   WriteByte(addr, x);
   WriteByte(addr+1, x>>8);
}

void Push(int x)
{
   WriteByte(sp,x);
   sp= (sp+1) & MaxMem;
}

void PushV(int v)
{
   int k;
   for (k=0; k<v; ++k)
      Push(0);
}

int  Pop(void)
{
   sp= (sp-1) & MaxMem;
   return ReadByte(sp);
}

void PopV(int v)
{
   int k;
   for (k=0; k<v; ++k)
      Pop();
}


void iPush(int x)
{
   Push(x&255);
   Push(x>>8);
}

int iPop(void)
{
   int hi= Pop();
   int lo= Pop();
   return (hi<<8) + lo;
}

void NOP(void)
{
   pc++;
   sprintf(dis,"NOP");
}

void BAD(void)
{
   char str[128];
   sprintf(str, "Invalid instruction code 0x%02x", mem[pc]);
   Error(str);
   go= 0;
   sprintf(dis,"BAD");
}

INT neg(INT x)
{
  return x & 0x8000 ? 1 : 0;
}

INT pos(INT x)
{
  return x & 0x8000 ? 0 : 1;
}

void ADD(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a+b;

      SetZ(r);
      SetN(r);
      SetC(r);
      SetO( neg(a) && neg(b) && pos(r)  || pos(a) && pos(b) && neg(r) );
      aa= r & 0xffff;
     }
   pc++;
   sprintf(dis,"ADD");
}

void SUB(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a-b;

      SetZ(r);
      SetN(r);
      SetC(r);
      SetO(neg(a) && pos(b) && pos(r)  || pos(a) && neg(b) && neg(r));
      aa= r & 0xffff;
     }
   pc++;
   sprintf(dis,"SUB");
}

void MUL(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a*b;

      SetZ(r);
      SetN(r);
      aa= r & 0xffff;
     }
   pc++;
   sprintf(dis,"MUL");
}

void DIV(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= 0;

      if (b==0)
         SetO(1);
      else
         {
          r= a/b;
          SetO(0);
         }
      SetZ(r);
      SetN(r);
      aa= r & 0xffff;
     }
   pc++;
   sprintf(dis,"DIV");
}

void MOD(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= 0;

      if (b==0)
         SetO(1);
      else
         {
          r= a%b;
          SetO(0);
         }
      SetZ(r);
      SetN(r);
      aa= r & 0xffff;
     }
   pc++;
   sprintf(dis,"MOD");
}

void CMP(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a-b;

      SetZ(r);
      SetN(r);
      SetC(r);
      SetO(neg(a) && pos(b) && pos(r)  || pos(a) && neg(b) && neg(r));
     }
   pc++;
   sprintf(dis,"CMP");
}

void SHL(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT c= a<<b;

      SetZ(c);
      SetN(c);
      aa= c & 0xFFFF;
     }
   pc++;
   sprintf(dis,"SHL");
}

void SHR(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT c= a>>b;

      SetZ(c);
      SetN(c);
      aa= c & 0xFFFF;
     }
   pc++;
   sprintf(dis,"SHR");
}

void NEG(void)
{
   if (go)
     {
      int r= -aa;
      SetZ(r);
      SetN(r);
      aa= r & 0xFFFF;
     }
   pc++;
   sprintf(dis,"NEG");
}

void INC(void)
{
   if (go)
     {
      INT r= aa+1;

      SetZ(r);
      SetN(r);
      SetC(r);
      SetO( pos(aa) && neg(r) );
      aa= r & 0xFFFF;
     }
   pc++;
   sprintf(dis,"INC");
}

void DEC(void)
{
   if (go)
     {
      INT r= aa-1;

      SetZ(r);
      SetN(r);
      SetC(r);
      SetO( neg(aa) && pos(r) );
      aa= r & 0xFFFF;
     }
   pc++;
   sprintf(dis,"DEC");
}

void XCH(void)
{
  if (go)
     {
      INT r= aa;
      aa= iPop();
      iPush(r);
     }
  pc++;
  sprintf(dis, "XCH");
}


void AND(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a & b;

      SetZ(r);
      SetN(r);
      aa= r;
     }
   pc++;
   sprintf(dis,"AND");
}

void OR(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a | b;

      SetZ(r);
      SetN(r);
      aa= r;
     }
   pc++;
   sprintf(dis,"OR ");
}

void XOR(void)
{
   if (go)
     {
      INT a= iPop();
      INT b= aa;
      INT r= a ^ b;

      SetZ(r);
      SetN(r);
      aa= r;
     }
   pc++;
   sprintf(dis,"XOR");
}

void NOT(void)
{
   if (go)
     {
      INT r= ~aa;

      SetZ(r);
      SetN(r);
      aa= r & 0xFFFF;
     }
   pc++;
   sprintf(dis,"NOT");
}

void TST(void)
{
   if (go)
     {
      INT r= aa ? 1 : 0;
      SetZ(r);
      SetN(r);
      aa= r;
     }
   pc++;
   sprintf(dis,"TST");
}

void JEQ(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= Z ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JEQ 0x%04x", dest);
}

void JNE(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= !Z ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JNE 0x%04x", dest);
}

void JLE(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= Z || (N^O) ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JLE 0x%04x", dest);
}

void JGT(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= !(Z || (N^O)) ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JGT 0x%04x", dest);
}

void JGE(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= Z || !(N^O) ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JGE 0x%04x", dest);
}

void JLT(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= !Z && (N^O) ? dest : pc+3;
   else
      pc+= 3;
   sprintf(dis,"JLT 0x%04x", dest);
}

void JMP(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      pc= dest;
   else
      pc+= 3;
   sprintf(dis,"JMP 0x%04x", dest);
}

void JMPb(void)
{
   if (go)
      pc= bb;
   else
      pc++;
   sprintf(dis,"JMP [B]");
}

void JSR(void)
{
   INT  dest= ReadInt(pc+1);
   if (go)
      {
       iPush(pc+3);
       pc= dest;
      }
   else
      pc+= 3;
   sprintf(dis,"JSR 0x%04x", dest);
}

void JSRb(void)
{
   INT dest= bb;
   if (go)
      {
       iPush(pc+1);
       pc= dest;
      }
   else
      pc++;
   sprintf(dis,"JSR [B]");
}

void RTN(void)
{
   if (go)
      pc= iPop();
   else
      pc++;
   sprintf(dis,"RTN");
}

void IRQ(void)
{
   INT r= ReadInt(pc+1);
   sprintf(dis,"IRQ 0x%04x", r); 
   pc+= 3;
   if (go)
      {
       INT   x, y;
       INT   t, n, k;

       switch(r)
         {
          case 0:   go= 0;               /* stop */
                    break;
          case 1:                        /* print integer */ 
                    x= (0x8000 & aa) ? aa|0xFFFF0000 : aa & 0x0000FFFF;
                    printf(" %d", x);
                    break;
          case 2:                        /* print string */
                    for(x= bb; mem[x]; x++)
                       printf("%c", mem[x]);
                    break;
          case 3:                        /* read integer */ 
                    scanf(" %d", &x);
                    aa= x & 0xFFFF;
                    break;

          case 5:   printf("%c", aa & 0xFF );   /*  print char */
                    break;

          case 4:   printf("%s", (char*)&mem[bb]);
                    break;
                                        /*  switch search   */
          case 7:   n= iPop();          /*  number of cases */
                    t= iPop();          /*  table           */
                    x= iPop();          /*  expression      */
                    for (k=0; k<n; k++)
                       if ( x==ReadInt(t+4*k) )    /* case value */
                          break;
                    pc= ReadInt(t+4*k+2); 
         }
      }
}

void CLRa(void)
{
   if (go)
       aa= 0;
   pc++;
   sprintf(dis,"CLR A");
}

void CLRb(void)
{
   if (go)
       bb= 0;
   pc++;
   sprintf(dis,"CLR B");
}

void LDAi(void)
{
   int v= ReadInt(pc+1);
   if (go)
      aa= v;
   pc+= 3;
   sprintf(dis, "LDA #0x%04x",v);
}

void LDAd(void)
{
   int d= ReadInt(pc+1);
   if (go)
      aa= ReadInt(d);
   pc+= 3;
   sprintf(dis, "LDA [0x%04x]",d);
}

void LDAb(void)
{
   if (go)
      aa= ReadInt(bb);
   pc++;
   sprintf(dis, "LDA [B]");
}

void LDAbp(void)
{
   int v= ReadInt(pc+1);
   if (go)
      aa= ReadInt(bp+v);
   pc+= 3;
   sprintf(dis, "LDA [BP+0x%04x]",v);
}


void STAd(void)
{
   int d= ReadInt(pc+1);
   if (go)
      WriteInt(d, aa);
   pc+= 3;
   sprintf(dis, "STA [0x%04x]",d);
}

void STAb(void)
{
   if (go)
      WriteInt(bb, aa);
   pc++;
   sprintf(dis, "STA [B]");
}

void STAbp(void)
{
   int v= ReadInt(pc+1);
   if (go)
      WriteInt(bp+v, aa);
   pc+= 3;
   sprintf(dis, "STA [BP+0x%04x]",v);
}


void PSHa(void)
{
   if (go)
      iPush(aa);
   pc++;
   sprintf(dis,"PSH A");
}

void PSHb(void)
{
   if (go)
      iPush(bb);
   pc++;
   sprintf(dis,"PSH B");
}

void PSHbp(void)
{
   if (go)
      iPush(bp);
   pc++;
   sprintf(dis,"PSH BP");
}

void PSHv(void)
{
   int v= ReadInt(pc+1);
   if (go)
      PushV(v);
   pc+= 3;
   sprintf(dis,"PSH %d", v);
}

void POPa(void)
{
   if (go)
     {
      aa= iPop();
      SetZ(aa);
      SetN(aa);
     }
   pc++;
   sprintf(dis,"POP A");
}

void POPb(void)
{
   if (go)
     {
      bb= iPop();
      SetZ(bb);
      SetN(bb);
     }
   pc++;
   sprintf(dis,"POP B");
}


void POPbp(void)
{
   if (go)
      bp= iPop();
   pc++;
   sprintf(dis,"POP BP");
}


void POPv(void)
{
   int v= ReadInt(pc+1);
   if (go)
      PopV(v);
   pc+= 3;
   sprintf(dis,"POP %d", v);
}

void LBPi(void)
{
   int v= ReadInt(pc+1);
   if (go)
      bp= v; 
   pc+= 3;
   sprintf(dis,"LBP #0x%04x", v);
}

void LBPb(void)
{
   if (go)
      bp= bb;
   pc++;
   sprintf(dis,"LBP B");
}

void LBPsp(void)
{
   if (go)
      bp= sp;
   pc++;
   sprintf(dis,"LBP SP");
}

void LSPi(void)
{
   int v= ReadInt(pc+1);
   if (go)
      sp= v; 
   pc+=3;
   sprintf(dis,"LSP #0x%04x", v);
}

void LSPb(void)
{
   if (go)
      sp= bb;
   pc++;
   sprintf(dis,"LSP B");
}

void LSPbp(void)
{
   if (go)
      sp= bp;
   pc++;
   sprintf(dis,"LSP BP");
}

void LEAd(void)
{
   int r = ReadInt(pc+1);
   if (go)
      bb= r;
   pc+= 3;
   sprintf(dis,"LEA 0x%04x", r);
}

void LEAbp(void)
{
   int r = ReadInt(pc+1);
   if (go)
      bb= (bp+r) & 0xFFFF;
   pc+= 3;
   sprintf(dis,"LEA BP+0x%04x", r);
}

void LEAibp(void)
{
   int r = ReadInt(pc+1);
   if (go)
      bb=  ReadInt((bp+r) & 0xFFFF);
   pc+= 3;
   sprintf(dis,"LEA [BP+0x%04x]", r);
}

void ADBa(void)
{
   if (go)
      bb= (bb+aa) & 0xffff;
   pc++;
   sprintf(dis, "ADB A");
}


void (*inst[])(void) = {
/*         00    01    02    03    04    05    06    07     */
/*         08    09    0a    0b    0c    0d    0e    0f     */

/* 00 */  NOP,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,

/* 10 */  ADD,  SUB,  MUL,  DIV,  MOD,  CMP,  SHL,  SHR,
          NEG,  INC,  DEC,  XCH,  BAD,  BAD,  BAD,  BAD, 

/* 20 */  AND,   OR,  XOR,  NOT,  TST,  BAD,  BAD,  BAD, 
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 

/* 30 */  JEQ,  JNE,  JLE,  JGT,  JGE,  JLT,  BAD,  BAD, 
          JMP,  JMPb, JSR,  JSRb, RTN,  IRQ,  BAD,  BAD, 

/* 40 */  CLRa, CLRb, LDAi, LDAd, LDAb, LDAbp, BAD, BAD, 
          BAD,  BAD,  BAD,  STAd, STAb, STAbp, BAD, BAD, 

/* 50 */  PSHa,  PSHb, PSHbp,  PSHv,  BAD,  BAD, BAD,  BAD, 
          BAD,   BAD,    BAD,   BAD,  BAD,  BAD, BAD,  BAD, 

/* 60 */  POPa,  POPb, POPbp,  POPv,  BAD,  BAD, BAD,  BAD, 
          BAD,   BAD,    BAD,   BAD,  BAD,  BAD, BAD,  BAD, 

/* 70 */  LBPb,  LBPsp,  LBPi,  BAD,  BAD,  BAD,  BAD,  BAD, 
          LSPb,  LSPbp,  LSPi,  BAD,  BAD,  BAD,  BAD,  BAD, 

/* 80 */  LEAd,  LEAbp,  LEAibp, BAD,  BAD,  BAD,  BAD,  BAD, 
          ADBa,    BAD,  BAD,   BAD,  BAD,  BAD,  BAD,  BAD, 

/* 90 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 

/* a0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 

/* b0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD, 

/* c0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,

/* d0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,
    	  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,

/* e0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,

/* f0 */  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,
          BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD,  BAD

};


int readaddr(const char* line, int* a, int da)
{
   int n;
   const char* p;
 
   *a= da;
   for (p=line; *p!=0 && isspace(*p); p++)
        ;

   if (*p=='-' || *p=='\0')
      return 1;

   n= sscanf(p, "%x", a);
   return 1;
}

int status(int d)
{
   printf(" pc=%04x  bp=%04x  a=%04x  b=%04x  sp= %04x"
          " Flags: %c%c%c%c",
          pc, bp, aa, bb, sp,  
          N?'N':' ',Z?'Z':' ',O?'O':' ',C?'C':' ');

   if (d)
       printf("%s", dis);

   printf("\n");
}
 
int main(int narg, char* args[])
{
   FILE* code;
   int   init;
   int   quit;
   int   addr0;
   int   c, k, n;
   char* p;
   char  line[1024];

   init= 0;
   sp= 0;
   SetN(0);
   SetZ(0);
   SetO(0);
   SetC(0);
   bp= 0;
   quit= 0;

   printf("Virtual Machine 3        by Eduardo Viruena Silva\n\n");
   if (narg>1)
     {
      code= fopen(args[1], "rt");

      if (code==NULL)
        {
         Error("Invalid code file");
         return EXIT_FAILURE;
        }

      while ((c=fgetc(code)) != EOF)
          {
           int   add, val;
           switch (c)
   	      {
	       case '<':
	          fscanf(code,"%x %x", &init, &sp);
	          break;
	       case ' ':
	          fscanf(code,"%x %x", &add, &val);
	          mem[add]= val;
	          break;
	       case ';':
	          break;
	      }
	    while ((c=fgetc(code))!=EOF && c!='\n')
	      ;
           }
      fclose(code);
     }
   else
     printf("Warning- no program loaded\n");

   pc= MaxMem-8;
   mem[pc+0]= 0x3a;     // JSR
   mem[pc+1]= init%256; // main
   mem[pc+2]= init/256;
   mem[pc+3]= 0x3d;     // IRQ 0
   mem[pc+4]= 0;
   mem[pc+5]= 0;
   mem[pc+6]= 0x38;     // JMP
   mem[pc+7]= pc%256;	
   mem[pc+8]= pc/256;

   printf("type h for help\n\nvm> "); 
   do {
      line[0]= '\0';
      scanf("%[^\n]", line);
      scanf("%*c");
      for (p= line; *p!=0 && isspace(*p); p++)
          ;
      
      switch (tolower(*p++)) {
         case 'q':  quit= 1;
                    break;

         case 'h':  printf("q  quit\n");          
                    printf("h  help\n");
                    printf("g [addr]  execute.  From addr, if given\n");
                    printf("t [addr]  execute and disasembly\n");
                    printf("s [addr]  step.  Executes one instruction\n");
                    printf("p print status\n");
                    printf("u [addr] disassembly\n");
                    printf("d [addr] dump memory\n");
                    break;

         case 's':  readaddr(p, &pc, pc);
                    go= 1;
                    inst[mem[pc]]();
                    pc= pc % (MaxMem+1);
                    status(1);
                    go= 0;
                    break;

         case 'd':  readaddr(p, &pc, pc);
                    for (k=0; k<64; k++)
                       {
                        if (k%8==0)
                           printf("\n%04x", pc);
                        printf(" %02x", mem[pc++]);
                       }
                    printf("\n");
                    break;

         case 'u':  readaddr(p, &pc, pc);
                    for (k=0; k<16; k++)
                       {
						int oldpc= pc;
                        inst[mem[pc]]();
                        pc= pc % (MaxMem+1);
                        printf(" %04x %s\n", oldpc, dis); 
                       }
                    break;

         case 't':  readaddr(p, &pc, pc);
                    go= 1;
                    while (go)
                       {
                        inst[mem[pc]]();
                        pc= pc % (MaxMem+1);
                        status(1);
                       }
                    break;

         case 'g':  readaddr(p, &pc, pc);
                    go= 1;
                    while (go)
                       {
                        inst[mem[pc]]();
                        pc= pc % (MaxMem+1);
                       }
                    printf("\n");
                    status(0);
//                    fprintf(stderr, "DEBUG: go= %d\n", go);
                    break;

         case 'p':  status(0);
                    break;
        }
      printf("vm> "); 
   } while (!quit);

   printf("\n");
   return 0;
}
