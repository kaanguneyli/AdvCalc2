; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%x = alloca i32
store i32 1, i32* %x
%s1 = load i32, i32* %x
%s0 = add i32 3, %s1
%y = alloca i32
store i32 %s0, i32* %y
%s3 = load i32, i32* %y
%s5 = load i32, i32* %y
%s7 = load i32, i32* %y
%s8 = load i32, i32* %x
%s6 = mul i32 %s7, %s8
%s4 = mul i32 %s5, %s6
%s2 = mul i32 %s3, %s4
%z = alloca i32
store i32 %s2, i32* %z
%s9 = load i32, i32* %z
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s9 )
%s11 = load i32, i32* %x
%s12 = load i32, i32* %x
%s10 = xor i32 %s11, %s12
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s10 )
%s15 = load i32, i32* %y
%s16 = load i32, i32* %z
%s14 = add i32 %s15, %s16
%s18 = load i32, i32* %x
%s19 = load i32, i32* %x
%s17 = xor i32 %s18, %s19
%s13 = or i32 %s14, %s17
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s13 )
%s23 = load i32, i32* %y
%s24 = load i32, i32* %z
%s22 = add i32 %s23, %s24
%s21 = lshr i32 %s22, 1
%s26 = load i32, i32* %x
%s27 = load i32, i32* %x
%s25 = xor i32 %s26, %s27
%s20 = or i32 %s21, %s25
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s20 )
%s32 = load i32, i32* %y
%s33 = load i32, i32* %z
%s31 = add i32 %s32, %s33
%s30 = lshr i32 %s31, 1
%s35 = load i32, i32* %x
%s36 = load i32, i32* %x
%s34 = xor i32 %s35, %s36
%s29 = or i32 %s30, %s34
%s28 = shl i32 %s29, 1
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s28 )
ret i32 0
}