
__attribute__((noreturn,used)) void __stack_chk_fail(void) {
   while(1);
}

__attribute__((noreturn,used)) void __stack_chk_fail_local(void) {
    while(1);
}
