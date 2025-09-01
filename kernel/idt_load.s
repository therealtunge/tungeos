isr_stub_0:
	pushl $0
	call exception_handler
	iret
isr_stub_1:
	pushl $1
	call exception_handler
	iret
isr_stub_2:
	pushl $2
	call exception_handler
	iret
isr_stub_3:
	pushl $3
	call exception_handler
	iret
isr_stub_4:
	pushl $4
	call exception_handler
	iret
isr_stub_5:
	pushl $5
	call exception_handler
	iret
isr_stub_6:
	pushl $6
	call exception_handler
	iret
isr_stub_7:
	pushl $7
	call exception_handler
	iret
isr_stub_8:
	pushl $8
	call exception_handler
	iret
isr_stub_9:
	pushl $9
	call exception_handler
	iret
isr_stub_10:
	pushl $10
	call exception_handler
	iret
isr_stub_11:
	pushl $11
	call exception_handler
	iret
isr_stub_12:
	pushl $12
	call exception_handler
	iret
isr_stub_13:
	pushl $13
	call exception_handler
	iret
isr_stub_14:
	pushl $14
	call exception_handler
	iret
isr_stub_15:
	pushl $15
	call exception_handler
	iret
isr_stub_16:
	pushl $16
	call exception_handler
	iret
isr_stub_17:
	pushl $17
	call exception_handler
	iret
isr_stub_18:
	pushl $18
	call exception_handler
	iret
isr_stub_19:
	pushl $19
	call exception_handler
	iret
isr_stub_20:
	pushl $20
	call exception_handler
	iret
isr_stub_21:
	pushl $21
	call exception_handler
	iret
isr_stub_22:
	pushl $22
	call exception_handler
	iret
isr_stub_23:
	pushl $23
	call exception_handler
	iret
isr_stub_24:
	pushl $24
	call exception_handler
	iret
isr_stub_25:
	pushl $25
	call exception_handler
	iret
isr_stub_26:
	pushl $26
	call exception_handler
	iret
isr_stub_27:
	pushl $27
	call exception_handler
	iret
isr_stub_28:
	pushl $28
	call exception_handler
	iret
isr_stub_29:
	pushl $29
	call exception_handler
	iret
isr_stub_30:
	pushl $30
	call exception_handler
	iret
.global isr_stub_table
isr_stub_table:
	.long isr_stub_0
	.long isr_stub_1
	.long isr_stub_2
	.long isr_stub_3
	.long isr_stub_4
	.long isr_stub_5
	.long isr_stub_6
	.long isr_stub_7
	.long isr_stub_8
	.long isr_stub_9
	.long isr_stub_10
	.long isr_stub_11
	.long isr_stub_12
	.long isr_stub_13
	.long isr_stub_14
	.long isr_stub_15
	.long isr_stub_16
	.long isr_stub_17
	.long isr_stub_18
	.long isr_stub_19
	.long isr_stub_20
	.long isr_stub_21
	.long isr_stub_22
	.long isr_stub_23
	.long isr_stub_24
	.long isr_stub_25
	.long isr_stub_26
	.long isr_stub_27
	.long isr_stub_28
	.long isr_stub_29
	.long isr_stub_30
