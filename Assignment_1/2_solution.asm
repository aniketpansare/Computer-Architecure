###########################################################################
		##Expression Evaluation - Problem Number 2 ###
		#By Aniket Pansare (ASU id:- 1205138918)
###########################################################################

.data			#Data Section to define variables

msg1: .asciiz "\nEnter value of u:"
msg2: .asciiz "\nEnter value of v:"
msg3: .asciiz "\nResult of 5uu-12uv+6vv := "


.text			#text section for the code

li $v0,4            
la $a0,msg1          #Print Message to enter value of u
syscall


li $v0,5
syscall		     #Take input from user in $t0=u
move $t0,$v0


li $v0,4
la $a0,msg2	     #Print Message to enter value of v
syscall


li $v0,5
syscall		     #Take input from user in $t1=v
move $t1,$v0

li $a0,5	     #Load arguments to be passed in $a0,$a1 and $a2 register, as $a<> register are used as standard for passing arguments
move $a1,$t0	     #pass 5,u,u to the subroutine
move $a2,$t0	     
jal subroutine       #call subroutine
move $s0,$v0	     #Store the output in register $s0



li $a0,12	     #Load arguments to be passed in $a0,$a1 and $a2 register
move $a1,$t0	     #pass 12,u,v to the subroutine
move $a2,$t1
jal subroutine	     #call subroutine
move $s1,$v0	     #Store the output in register $s1



li $a0,6	     #Load arguments to be passed in $a0,$a1 and $a2 register
move $a1,$t1	     #pass 6,v,v to the subroutine
move $a2,$t1
jal subroutine	     #call subroutine
move $s2,$v0	     #Store the output in register $s2




sub $s3,$s0,$s1      
add $s4,$s3,$s2      #calculate 5*u*u - 12*u*v + 6*v*v



li $v0,4
la $a0,msg3	     #Print Message before Result
syscall
move $a0,$s4 
li $v0,1	     #Print the Result
syscall


li $v0,10            #Exit the program
syscall

###################################################################################################################
		### Multiplication Subroutine taking 3 arguments and returning a Product###

####################################################################################################################

subroutine:
sw $ra,0($sp)        # Store return address on stack. 
subu $sp,$sp,4       # decrement stack pointer. 
sw $fp,0($sp)        # Store frame pointer on stack to preserve original 
addu $fp,$sp,8     # Initialize frame pointer to store arguments
sw $a0,0($fp)
sw $a1,4($fp)	     # Store the arguments using frame pointer
sw $a2,8($fp)
addu $fp,$fp,12      # Increment position of frame pointer after storing 3 Arguments



mul $t2,$a0,$a1
mul $t4,$t2,$a2      #Perform multiplication on input Arguments C*x*y
mfhi $t5

move $v0,$t4
move $v1,$t5



lw $fp,0($sp)       #Restore the frame pointer
addu $sp,$sp,4      # Increment stack pointer
lw $ra,0($sp)       #Restore return address
jr $ra
