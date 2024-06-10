define i32 @ps3_random_number() {
    ret i32 42
}

define i32 @add_42(i32 %n) {
    %result = add i32 %n, 42
    ret i32 %result
}

define i32 @bytes_to_bits(i32 %n) {
    %result = mul i32 %n, 8
    ret i32 %result
}

define i32 @upper_32_bits(i64 %n) {
    %shifted = lshr i64 %n, 32
    %trunc = trunc i64 %shifted to i32
    ret i32 %trunc
}

define i32 @vm_operation(i1 %do_add, i32 %x, i32 %y) {
    %result_add = add i32 %x, %y
    %result_sub = sub i32 %x, %y
    %result = select i1 %do_add, i32 %result_add, i32 %result_sub
    ret i32 %result
}

; extern "C" void store_value(uint32_t n, uint32_t key)
declare void @store_value(i32 %n, i32 %key);

define void @pass_to_store_value(i32 %n) {
    %lowest_bit = trunc i32 %n to i1
    %key = select i1 %lowest_bit, i32 512, i32 99
    call void @store_value(i32 %n, i32 %key)
    ret void
}

