start:
	HALT
bootloader:
	LOAD $s1 1
	LOAD $s2 1 	
	LOAD $s3 2	
	LOAD $s4 6		
	MOVE $t1 $fr
	SHR $t1 $s3	
	AND $t1 $s2	
	SKC $t1		
	JUMP start
loop:			
	IN $t1		
	STORE $s1 $t1
	ADD $s1 $s2	
	MOVE $t1 $fr
	SHR $t1 $s3	
	AND $t1 $s2	
	SKC $t1		
	JUMP done	
	JUMP loop
done:
	ADD $s1 $s2
	MOVE $fp $s1
	MOVE $sp $s1
	JUMP start