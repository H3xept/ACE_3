load $s1 0
load $s2 3
load $s3 1
loadl $s4 datastuff
load $s4 $s4

main_loop:
jump print_binary
add $s1 $s3
move $t1 $s1
add $t1 $s4
skc $t1
jump main_loop
halt

datastuff:
-512

print_binary:

#create stack frame
load $t1 1
store $sp $fp
move $fp $sp

add $sp $t1
store $sp $ra
add $sp $t1
store $sp $s1
add $sp $t1
store $sp $s2
add $sp $t1
store $sp $s3
add $sp $t1
store $sp $s4
add $sp $t1
store $sp $s5
add $sp $t1

#subroutine main body

move $s2 $s1
load $t1 1
load $s3 0

#count bits in number
print_binary_count_bits_loop:
shr $s2 $t1
add $s3 $t1
skc $s2
jump print_binary_done_bits
jump print_binary_count_bits_loop

print_binary_done_bits:

jump reverse_number

move $s1 $t1
load $t4 1
load $t3 -1

#print binary
print_binary_loop:
add $s3 $t3
skc $s3
jump print_binary_done
move $t2 $t1
and $t2 $t4
out $t2 0
shr $t1 $t4
jump print_binary_loop

print_binary_done:
out $t1 1

#deconstruct stack frame
load $t4 -1
add $sp $t4
load $s5 $sp
add $sp $t4
load $s4 $sp
add $sp $t4
load $s3 $sp
add $sp $t4
load $s2 $sp
add $sp $t4
load $s1 $sp
add $sp $t4
load $ra $sp
add $sp $t4
load $fp $sp

#jump with $ra
jump 0xfff


reverse_number:

#create stack frame
load $t1 1
store $sp $fp
move $fp $sp

add $sp $t1
store $sp $ra
add $sp $t1
store $sp $s1
add $sp $t1
store $sp $s2
add $sp $t1
store $sp $s3
add $sp $t1
store $sp $s4
add $sp $t1
store $sp $s5
add $sp $t1

#subroutine body
load $s5 0
load $s4 -15
load $t1 0
load $t3 1

#main loop
reverse_number_loop:
shl $t1 $t3
move $t2 $s1
and $t2 $t3
or $t1 $t2
shr $s1 $t3
add $s4 $t3
skc $s1
jump reverse_number_done
jump reverse_number_loop
reverse_number_done:
#deconstruct stack frame
load $t4 -1
add $sp $t4
load $s5 $sp
add $sp $t4
load $s4 $sp
add $sp $t4
load $s3 $sp
add $sp $t4
load $s2 $sp
add $sp $t4
load $s1 $sp
add $sp $t4
load $ra $sp
add $sp $t4
load $fp $sp

#jump with $ra
jump 0xfff