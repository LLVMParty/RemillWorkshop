define i32 @ps3_random_number() {
    ret i32 42;
}

define i32 @add_42(i32 %n) {
    %result = add i32 %n, 42
    ret i32 %result
}

define i32 @bytes_to_bits(i32 %n) {
    %result = mul i32 %n, 8
    ret i32 %result
}

declare void @store_value(i32 %n);

define void @pass_to_store_value(i32 %n) {
    %t = trunc i32 %n to i1
    %key = select i1 %t, i32 512, i32 99
    call void @store_value(i32 %n, i32 %key)
    ret void
}

define i32 @vm_operation(i1 %do_add, i32 %x, i32 %y) {
    br i1 %do_add, label %bb_add, label %bb_sub
bb_add:
    %result_add = add i32 %x, %y
    br label %bb_end
bb_sub:
    %result_sub = sub i32 %x, %y
    br label %bb_end
bb_end:
    %result = phi i32 [%result_add, %bb_add], [%result_sub, %bb_sub]
    ret i32 %result
}

define i32 @upper_32_bits(i64 %n) {
    %a = lshr i64 %n, 32
    %temp = trunc i64 %a to i32
    ret i32 %temp
}
