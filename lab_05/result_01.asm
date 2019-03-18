.data
.text
.globl main
main:
	li $a0, 12
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	li $a0, 3
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	lw $t0, 8($sp)
	lw $t1, 4($sp)
	mult $t0, $t1
	mflo $a0
	addiu $sp, $sp, 8
	sw $a0 , 0($sp)
	addiu $sp , $sp , -4
	li $a0, 2
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	li $a0, 7
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	lw $t0, 8($sp)
	lw $t1, 4($sp)
	add $a0, $t0, $t1
	addiu $sp, $sp, 8
	sw $a0 , 0($sp)
	addiu $sp , $sp , -4
	lw $t0, 8($sp)
	lw $t1, 4($sp)
	div $t0, $t1
	mflo $a0
	addiu $sp, $sp, 8
	sw $a0 , 0($sp)
	addiu $sp , $sp , -4
	li $a0, 5
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	li $a0, 2
	sw $a0, 0($sp)
	addiu $sp, $sp, -4
	lw $t0, 8($sp)
	lw $t1, 4($sp)
	sub $a0, $t0, $t1
	addiu $sp, $sp, 8
	sw $a0 , 0($sp)
	addiu $sp , $sp , -4
	li $v0, 1
	syscall
	li $v0, 10
	syscall
