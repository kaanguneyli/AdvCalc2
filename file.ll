; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%s1 = or i32 4, 2
%s2 = sub i32 32, %s1
%s3 = shl i32 10, %s1
%s4 = lshr i32 10, %s2
%s0 = or i32 %s3, %s4
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s0 )
ret i32 0
}; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%s2 = sub i32 32, 2
%s3 = shl i32 10, 2
%s4 = lshr i32 10, %s2
%s1 = or i32 %s3, %s4
%s0 = or i32 4, %s1
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s0 )
ret i32 0
}; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%s2 = sub i32 32, 2
%s3 = shl i32 10, 2
%s4 = lshr i32 10, %s2
%s1 = or i32 %s3, %s4
%s0 = or i32 %s1, 4
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s0 )
ret i32 0
}; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%s2 = sub i32 32, 2
%s3 = shl i32 10, 2
%s4 = lshr i32 10, %s2
%s1 = or i32 %s3, %s4
%s0 = or i32 %s1, 4
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s0 )
ret i32 0
}