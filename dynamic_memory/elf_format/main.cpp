// Section 19: .data.rel.ro PROGBITS WA
constexpr auto c_foo = "ACCU 2025";
static constexpr auto sc_foo = "ACCU 2025";

// Section 27: .bss NOBITS WA
int bar;
static int s_bar;

// Ill-formed
// const int c_bar;
// static const int sc_bar;

// Section 25: .data PROGBITS WA
int d = 42;
static int s_d = 42;
// Section 16: .rodata PROGBITS A
const int c_d = 42;
static const int sc_d = 42;

int main()
{
    // .text PROGBITS AX
    return 0;
}
