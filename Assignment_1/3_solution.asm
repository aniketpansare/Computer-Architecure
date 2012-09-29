###########################################################################
		##Sum of Array - Problem Number 3 ###
		#By Aniket Pansare (ASU id:- 1205138918)
###########################################################################


.data
msg1: .asciiz "\nEnter Number of Elements in the Array"
msg2: .asciiz "\nSum of Elements:="
msg3: .asciiz "\nNext number:"
msg4: .asciiz "\n Sum= "

array: .word 0:20     #Declare an array of 20 integers
Sum: .word 0

.text

la $a0,msg1              #Print Message to enter elements in the array
li $v0,4
syscall

li $v0,5			 #Get number of elements in an array as input from user
syscall
move $s0,$v0

move $a0,$s0           # Load $a0 and $a1 register before calling the function 
la $a1,array

jal Sumofarray            # Call Sumofarray function ,  $a0 and $a1 represent function arguments


li $v0,10
syscall


##################################################################################
									## Definition SumOfArray function##
##################################################################################

Sumofarray:
sw $ra,0($sp)        # Store return address on stack.
subu $sp,$sp,4       # decrement stack pointer. 
sw $fp,0($sp)        # Store frame pointer on stack to preserve original position
subu $sp,$sp,4       # decrement stack pointer. 
addu $fp,$sp,12    # Initialize frame pointer to store function arguments

sw $a0,0($fp)        
sw $a1,4($fp)	     # Store the function arguments using frame pointer
addu $fp,$fp,8

lw $t1,-4($fp)
lw $t0,-8($fp)             # get arguments in a temporary variable for execution



loop: blez $t0,Continue            #Loop untill all the elements are accepted from user
li $v0,4
la $a0, msg3				#print message to get the next input element in the array
syscall

li $v0,5
syscall					#Take input from User 
sw $v0,0($t1)				

addi $t1,$t1,4			        #increment pointer to array in order to accept the next input
addi $t0,$t0,-1
jal loop					#Loop untill all the elemnts are not scanned
Continue:




la $a3, Sum				#Initialize $a3 to point the "Sum" variable
lw $t1,-4($fp)				#Reinitialize $t1 and $t0 from frame pointer to run the next loop
lw $t0,-8($fp)				#$t1- points to start of array   &  $t0 contains the number of elements in array

loop2:  blez $t0,Result		#Repeat the loop untill all elements in array are added
lw $a2,0($t1)

jal Psum					#loop and call Psum for eack element in the array.

addi $t1,$t1,4
addi $t0,$t0,-1
jal loop2				#End of loop, Jump back to check if there are still any elements in the array




Result:
la $a0,msg4              #Print Message before displaying sum
li $v0,4
syscall
li $v0,1				#Display the result of sum of the elements in array
la $t4,Sum
lw $a0,0($t4)
syscall

addu $sp,$sp,4      # Increment stack pointer
lw $fp,0($sp)       #Restore the frame pointer
addu $sp,$sp,4      # Increment stack pointer
lw $ra,0($sp)       #Restore return address
jr $ra


##################################################################################
									## Definition Psum  function##
##################################################################################

Psum:
sw $ra,0($sp)        # Store return address on stack.
subu $sp,$sp,4       # decrement stack pointer. 
sw $fp,0($sp)        # Store frame pointer on stack to preserve original 
subu $sp,$sp,4       # decrement stack pointer. 


lw $t2,0($a3)
add $t2,$t2,$a2                       #perform the addition of the current element in the array
sw $t2,0($a3)


addu $sp,$sp,4      # Increment stack pointer
lw $fp,0($sp)       #Restore the frame pointer
addu $sp,$sp,4      # Increment stack pointer
lw $ra,0($sp)       #Restore return address
jr $ra



########END########
