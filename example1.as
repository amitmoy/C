.extern testing
.extern ss
bbbb: .data 7,8,9
adad: .data 7  ,   -98	,	9
TEST:   .string "abcsss"   
stop
	add 	testing, 	 r4 
	add 	ss, 	 r4 
.entry TEST, bbbb



