###########################################################################
		##Recursion - Problem Number 4###
		#By Aniket Pansare (ASU id:- 1205138918)
###########################################################################



.data
msg1: .asciiz "\nEnter value of m (for,m<=3):"
msg2: .asciiz "\nEnter value of n (for,n<=4):"
msg3: .asciiz "\nResult= "

.text

li $v0,4
la $a0,msg1            #Print Message to get value of m
syscall

li $v0,5
syscall			#Accept "m" as an input from user in register $t0
move $t0,$v0

li $v0,4
la $a0,msg2		#Print message to get value of "n"
syscall

li $v0,5
syscall			#Accept "n" as an input from user in register $t1
move $t1,$v0

addu $fp,$sp,4		#Initialize frame pointer to store arguments to the function
move $a0,$t0		
move $a1,$t1 		#Store Arguments in $a0 and $a1 before doing a function call... as $a<x> registers are standard for passing arguments.

jal A				#Call Subroutine A



li $v0,4
la $a0,msg3		#Print message before result i.e print "Result="
syscall			

li $v0,1
move $a0, $s0		#Fetch and print the return value from subroutine A... (Here, $v0 is set to 1 as we want to print integer).
syscall

li $v0,10
syscall			# Exit from program by calling system call 10


######################################################################################
									###Subroutine A ###
######################################################################################



A:
sw $ra,0($sp)        # Store return address on stack.
subu $sp,$sp,4       # decrement stack pointer. 
sw $fp,0($sp)         # Store frame pointer on stack which will point to the input arguments to the function.
subu $sp,$sp,4       # decrement stack pointer. 

sw $a0,0($fp)		#Store first argument to the function i.e $a0 at frame pointer location
sw $a1,4($fp)		#Increment $fp an store second argument to the function i.e $a1
addu $fp,$fp,8


beqz $a0, Firstcondition       ## Verify if(m==0), if it satisfies go and execute Firstcondition logic


bgtz $a0, step1                      		## Verify if(m>0) &&
j step2
step1: beqz $a1,  Secondcondition     ## verify (n==0), if both (m>0) && (n==0) satisfied go and exectue Secondcondtion logic
j step2                                             ## If conditions dont satisfy move to step2


step2:
bgtz $a0,step3              			##Verify if (m>0) &&
j End
step3: bgtz $a1,Thirdcondition	 	##verify(n>0), if both (m>0) && (n>0) satisfied go and execute Thirdcondition logic
j End							## if conditions dont satisfy jump to end




Firstcondition:					# If first condition satisfied execute this
addu $s0,$a1,1				# return n-1;   in register $s0
j End

Secondcondition:				#if second condition satisfied execute this
subu $a0,$a0,1				# return A(m-1,1) ;    decrement m and set n=1 and call subroutine A
li $a1,1
jal A							#call subroutine
j End


Thirdcondition:					#if third condition satisfied execute this
subu $a1,$a1,1				
jal A							# INNER call to the subroutine A
move $a1,$s0
subu $a0,$a0,1
jal A							#OUTER call to the subroutine A

End:
lw $a1,-4($fp)		#restore $a1 from the frame pointer
lw $a0,-8($fp)		#decrement and restore $a0 from the frame pointer
subu $fp,$fp,8		#decrement frame pointer
addu $sp,$sp,4      # Increment stack pointer
lw $fp,0($sp)       #Restore the frame pointer to the same value as it was at the start of function call
addu $sp,$sp,4      # Increment stack pointer
lw $ra,0($sp)       #Restore return address to the same value as it was at the start of function call
jr $ra		      #return to the lcation from where call was made.


#############END#################
