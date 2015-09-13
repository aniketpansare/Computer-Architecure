###########################################################################
		##Lower case to Upper case - Problem Number 1 ###
		#By Aniket Pansare (ASU id:- 1205138918)
###########################################################################


.data					#data section to define variables
string: .asciiz "abcdefgh"
StringBefore: .asciiz "\nString Before (lowercase): "
StringAfter: .asciiz "\nString After (uppercase): "

.text					#code section for the code
main:
li $v0,4               #Print message to display string in lower case  ...($v0=4 syscall used for printing string in $a0)
la $a0,StringBefore
syscall

li $v0,4	    
la $a0,string	    #Print the string in lower case
syscall 


la $t1,string	       #Load address of string in register $t1
lb $t3,0($t1)	       #Load the first character of the string in register $t3


Loop:
beq $t3,$zero,End   #Check if the character is end of string then jump to label End
addi $t3,$t3,-32    #Subtract 32 i.e (0x20) from the character being referenced to convert to upper case
sb $t3,0($t1)	    #Store the uppercase character back in to memory location
addi $t1,$t1,1	    #Increment register $t1 to point to the next character
lb $t3,0($t1)	    #Initialize $t3 with new character pointer by $t1, to evaluate branch condition
jal Loop	    #Jump to label Loop and verify branch condition

End:
li $v0,4               #Print message to display string in lower case  ...($v0=4 syscall used for printing string in $a0)
la $a0,StringAfter
syscall

li $v0,4	    
la $a0,string	    #Print the string in upper case
syscall 

li $v0,10 	    # Exit the program
syscall
