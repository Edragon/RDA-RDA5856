
D:/rda5981/code/IoT-RDA5836_3.3/hex/flash_prog_host_5856T_nolcd_debug/flash_prog_host_5856T_nolcd_debug.elf:     file format elf32-littlemips


Disassembly of section .executor_ctx:

a1c000a0 <g_bootHstExecutorCtx>:
a1c000a0:	00000000 	nop
a1c000a4:	a1c00280 	sb	zero,640(t6)
	...

Disassembly of section .hal_boot_sector_struct:

a1c00000 <g_halBootSectorStruct>:
	...

Disassembly of section .boot_sector_reload_struct_ptr:

a1c00274 <g_bootSectorReloadStructPtr>:
a1c00274:	00000000 	nop

Disassembly of section .boot_sector_struct_ptr:

a1c00278 <g_bootSectorStructPtr>:
a1c00278:	00000000 	nop

Disassembly of section .fixptr:

a1c0027c <g_halFixedPointer>:
a1c0027c:	00000000 	nop

Disassembly of section .sramtext:

a1c00280 <boot_Sector>:
a1c00280:	3c05a1c0 	lui	a1,0xa1c0
a1c00284:	8ca300cc 	lw	v1,204(a1)
a1c00288:	27bdffe0 	addiu	sp,sp,-32
a1c0028c:	30620040 	andi	v0,v1,0x40
a1c00290:	afb00014 	sw	s0,20(sp)
a1c00294:	afbf001c 	sw	ra,28(sp)
a1c00298:	afb10018 	sw	s1,24(sp)
a1c0029c:	1040000b 	beqz	v0,a1c002cc <boot_Sector+0x4c>
a1c002a0:	00808021 	move	s0,a0
a1c002a4:	3c06a1c0 	lui	a2,0xa1c0
a1c002a8:	8cc20278 	lw	v0,632(a2)
a1c002ac:	00000000 	nop
a1c002b0:	30440003 	andi	a0,v0,0x3
a1c002b4:	14800005 	bnez	a0,a1c002cc <boot_Sector+0x4c>
a1c002b8:	3c085e40 	lui	t0,0x5e40
a1c002bc:	00488821 	addu	s1,v0,t0
a1c002c0:	3c070001 	lui	a3,0x1
a1c002c4:	087000b4 	j	a1c002d0 <boot_Sector+0x50>
a1c002c8:	0227882b 	sltu	s1,s1,a3
a1c002cc:	00008821 	move	s1,zero
a1c002d0:	3c03a1c0 	lui	v1,0xa1c0
a1c002d4:	8c620274 	lw	v0,628(v1)
a1c002d8:	00000000 	nop
a1c002dc:	30490003 	andi	t1,v0,0x3
a1c002e0:	1520002d 	bnez	t1,a1c00398 <boot_Sector+0x118>
a1c002e4:	3c1f5e40 	lui	ra,0x5e40
a1c002e8:	005fc021 	addu	t8,v0,ra
a1c002ec:	3c190001 	lui	t9,0x1
a1c002f0:	0319502b 	sltu	t2,t8,t9
a1c002f4:	11400028 	beqz	t2,a1c00398 <boot_Sector+0x118>
a1c002f8:	3c06b007 	lui	a2,0xb007
a1c002fc:	8c450000 	lw	a1,0(v0)
a1c00300:	34c45ec6 	ori	a0,a2,0x5ec6
a1c00304:	14a40024 	bne	a1,a0,a1c00398 <boot_Sector+0x118>
a1c00308:	3c094ff8 	lui	t1,0x4ff8
a1c0030c:	8c480004 	lw	t0,4(v0)
a1c00310:	3527a139 	ori	a3,t1,0xa139
a1c00314:	15070020 	bne	t0,a3,a1c00398 <boot_Sector+0x118>
a1c00318:	240a000c 	li	t2,12
a1c0031c:	ac400000 	sw	zero,0(v0)
a1c00320:	8c620274 	lw	v0,628(v1)
a1c00324:	00000000 	nop
a1c00328:	ac400004 	sw	zero,4(v0)
a1c0032c:	3c08a000 	lui	t0,0xa000
a1c00330:	00001021 	move	v0,zero
a1c00334:	24090003 	li	t1,3
a1c00338:	8c640274 	lw	a0,628(v1)
a1c0033c:	00003821 	move	a3,zero
a1c00340:	00800013 	mtlo	a0
a1c00344:	004a001c 	madd	v0,t2
a1c00348:	0000c012 	mflo	t8
a1c0034c:	8f1f000c 	lw	ra,12(t8)
a1c00350:	8f190008 	lw	t9,8(t8)
a1c00354:	03e83025 	or	a2,ra,t0
a1c00358:	8f040010 	lw	a0,16(t8)
a1c0035c:	087000de 	j	a1c00378 <boot_Sector+0xf8>
a1c00360:	03282825 	or	a1,t9,t0
a1c00364:	8cb80000 	lw	t8,0(a1)
a1c00368:	00000000 	nop
a1c0036c:	acd80000 	sw	t8,0(a2)
a1c00370:	24a50004 	addiu	a1,a1,4
a1c00374:	24c60004 	addiu	a2,a2,4
a1c00378:	00e4c82b 	sltu	t9,a3,a0
a1c0037c:	1720fff9 	bnez	t9,a1c00364 <boot_Sector+0xe4>
a1c00380:	24e70001 	addiu	a3,a3,1
a1c00384:	24420001 	addiu	v0,v0,1
a1c00388:	1449ffeb 	bne	v0,t1,a1c00338 <boot_Sector+0xb8>
a1c0038c:	00000000 	nop
a1c00390:	0c7800b0 	jal	a1e002c0 <boot_HstMonitorX>
a1c00394:	00000000 	nop
a1c00398:	74700128 	jalx	a1c004a0 <boot_SectorFixRom>
a1c0039c:	00000000 	nop
a1c003a0:	3c05a1a0 	lui	a1,0xa1a0
a1c003a4:	34a31014 	ori	v1,a1,0x1014
a1c003a8:	3c0aa1c0 	lui	t2,0xa1c0
a1c003ac:	12200011 	beqz	s1,a1c003f4 <boot_Sector+0x174>
a1c003b0:	ad430244 	sw	v1,580(t2)
a1c003b4:	3c09a1c0 	lui	t1,0xa1c0
a1c003b8:	8d280278 	lw	t0,632(t1)
a1c003bc:	3c074e6a 	lui	a3,0x4e6a
a1c003c0:	8d060000 	lw	a2,0(t0)
a1c003c4:	34f1821c 	ori	s1,a3,0x821c
a1c003c8:	14d1000a 	bne	a2,s1,a1c003f4 <boot_Sector+0x174>
a1c003cc:	ad000000 	sw	zero,0(t0)
a1c003d0:	3c18a1a3 	lui	t8,0xa1a3
a1c003d4:	241f00aa 	li	ra,170
a1c003d8:	3702f000 	ori	v0,t8,0xf000
a1c003dc:	24040056 	li	a0,86
a1c003e0:	ac5f0014 	sw	ra,20(v0)
a1c003e4:	74780140 	jalx	a1e00500 <mon_Event>
a1c003e8:	00000000 	nop
a1c003ec:	74700129 	jalx	a1c004a4 <boot_SectorEnterBootMonitor>
a1c003f0:	00000000 	nop
a1c003f4:	3c05a1a3 	lui	a1,0xa1a3
a1c003f8:	34a3f000 	ori	v1,a1,0xf000
a1c003fc:	3c0aa1c0 	lui	t2,0xa1c0
a1c00400:	3c19a1e0 	lui	t9,0xa1e0
a1c00404:	3c04a1e0 	lui	a0,0xa1e0
a1c00408:	ac600014 	sw	zero,20(v1)
a1c0040c:	27223fd0 	addiu	v0,t9,16336
a1c00410:	25430240 	addiu	v1,t2,576
a1c00414:	0870010b 	j	a1c0042c <boot_Sector+0x1ac>
a1c00418:	24843fe0 	addiu	a0,a0,16352
a1c0041c:	8c510000 	lw	s1,0(v0)
a1c00420:	24420004 	addiu	v0,v0,4
a1c00424:	ac710000 	sw	s1,0(v1)
a1c00428:	24630004 	addiu	v1,v1,4
a1c0042c:	0044302b 	sltu	a2,v0,a0
a1c00430:	14c0fffa 	bnez	a2,a1c0041c <boot_Sector+0x19c>
a1c00434:	3c08a1c0 	lui	t0,0xa1c0
a1c00438:	3c07a1c0 	lui	a3,0xa1c0
a1c0043c:	25020100 	addiu	v0,t0,256
a1c00440:	08700114 	j	a1c00450 <boot_Sector+0x1d0>
a1c00444:	24e30240 	addiu	v1,a3,576
a1c00448:	ac400000 	sw	zero,0(v0)
a1c0044c:	24420004 	addiu	v0,v0,4
a1c00450:	0043482b 	sltu	t1,v0,v1
a1c00454:	1520fffc 	bnez	t1,a1c00448 <boot_Sector+0x1c8>
a1c00458:	00000000 	nop
a1c0045c:	74700144 	jalx	a1c00510 <hal_SysSetupPLL>
a1c00460:	00000000 	nop
a1c00464:	74780d1d 	jalx	a1e03474 <hal_FlashCacheOff>
a1c00468:	00000000 	nop
a1c0046c:	74700467 	jalx	a1c0119c <memd_FlashRst>
a1c00470:	00000000 	nop
a1c00474:	74700130 	jalx	a1c004c0 <tgt_GetMemdFlashConfig>
a1c00478:	00000000 	nop
a1c0047c:	747004d3 	jalx	a1c0134c <memd_FlashOpen>
a1c00480:	00402021 	move	a0,v0
a1c00484:	0c700198 	jal	a1c00660 <boot_LoaderEnter>
a1c00488:	02002021 	move	a0,s0
a1c0048c:	8fbf001c 	lw	ra,28(sp)
a1c00490:	8fb10018 	lw	s1,24(sp)
a1c00494:	8fb00014 	lw	s0,20(sp)
a1c00498:	03e00008 	jr	ra
a1c0049c:	27bd0020 	addiu	sp,sp,32

a1c004a0 <boot_SectorFixRom>:
a1c004a0:	e820      	jr	ra
a1c004a2:	6500      	nop

a1c004a4 <boot_SectorEnterBootMonitor>:
a1c004a4:	b306      	lw	v1,a1c004bc <$LCFI0+0x10>
a1c004a6:	9b40      	lw	v0,0(v1)
a1c004a8:	6c02      	li	a0,2
a1c004aa:	63fd      	addiu	sp,-24

a1c004ac <$LCFI0>:
a1c004ac:	ea8d      	or	v0,a0
a1c004ae:	6205      	sw	ra,20(sp)
a1c004b0:	1b03 0251 	jal	a1e00944 <boot_Monitor>
a1c004b4:	db40      	sw	v0,0(v1)
a1c004b6:	9705      	lw	a3,20(sp)
a1c004b8:	ef00      	jr	a3
a1c004ba:	6303      	addiu	sp,24
a1c004bc:	00cc      	addiu	s0,sp,816
a1c004be:	a1c0      	lbu	a2,0(s1)

a1c004c0 <tgt_GetMemdFlashConfig>:
a1c004c0:	b202      	lw	v0,a1c004c8 <tgt_GetMemdFlashConfig+0x8>
a1c004c2:	e820      	jr	ra
a1c004c4:	6500      	nop
a1c004c6:	6500      	nop
a1c004c8:	04d8      	addiu	a0,sp,864
a1c004ca:	a1c0      	lbu	a2,0(s1)

a1c004cc <tgt_GetMemdRamConfig>:
a1c004cc:	b202      	lw	v0,a1c004d4 <tgt_GetMemdRamConfig+0x8>
a1c004ce:	e820      	jr	ra
a1c004d0:	6500      	nop
a1c004d2:	6500      	nop
a1c004d4:	04e0      	addiu	a0,sp,896
a1c004d6:	a1c0      	lbu	a2,0(s1)

a1c004d8 <g_tgtMemdFlashCfg>:
a1c004d8:	100a2206 	beq	zero,t2,a1c08cf4 <_sys_sram_bss_end+0x44>
a1c004dc:	00010011 	0x10011

a1c004e0 <g_tgtMemdRamCfg>:
a1c004e0:	11080108 	beq	t0,t0,a1c00904 <main+0xb8>
a1c004e4:	20000015 	addi	zero,zero,21

a1c004e8 <memd_RamOpen>:
a1c004e8:	63fd      	addiu	sp,-24

a1c004ea <$LCFI22>:
a1c004ea:	6205      	sw	ra,20(sp)
a1c004ec:	67a4      	move	a1,a0
a1c004ee:	2c01      	bnez	a0,a1c004f2 <$LCFI22+0x8>
a1c004f0:	e845      	break	2
a1c004f2:	1a03 0686 	jal	a1c01a18 <hal_EbcCsOpen>
a1c004f6:	6c01      	li	a0,1
a1c004f8:	b203      	lw	v0,a1c00504 <$LCFI22+0x1a>
a1c004fa:	6b00      	li	v1,0
a1c004fc:	da60      	sw	v1,0(v0)
a1c004fe:	9705      	lw	a3,20(sp)
a1c00500:	ef00      	jr	a3
a1c00502:	6303      	addiu	sp,24
a1c00504:	4410      	daddiu	s0,a0,0
a1c00506:	a1a0      	lbu	a1,0(s1)

a1c00508 <hal_HstSendEvent>:
a1c00508:	e820      	jr	ra
a1c0050a:	6a00      	li	v0,0

a1c0050c <hal_SysSetupNonePLL>:
a1c0050c:	e820      	jr	ra
a1c0050e:	6500      	nop

a1c00510 <hal_SysSetupPLL>:
a1c00510:	b30b      	lw	v1,a1c0053c <hal_SysSetupPLL+0x2c>
a1c00512:	b20c      	lw	v0,a1c00540 <hal_SysSetupPLL+0x30>
a1c00514:	6d01      	li	a1,1
a1c00516:	b40c      	lw	a0,a1c00544 <hal_SysSetupPLL+0x34>
a1c00518:	c4a0      	sb	a1,0(a0)
a1c0051a:	da60      	sw	v1,0(v0)
a1c0051c:	b209      	lw	v0,a1c00540 <hal_SysSetupPLL+0x30>
a1c0051e:	b70b      	lw	a3,a1c00548 <hal_SysSetupPLL+0x38>
a1c00520:	9ad2      	lw	a2,72(v0)
a1c00522:	eeec      	and	a2,a3
a1c00524:	26fb      	beqz	a2,a1c0051c <hal_SysSetupPLL+0xc>
a1c00526:	9ad1      	lw	a2,68(v0)
a1c00528:	b709      	lw	a3,a1c0054c <hal_SysSetupPLL+0x3c>
a1c0052a:	6c80      	li	a0,128
a1c0052c:	eeed      	or	a2,a3
a1c0052e:	dad1      	sw	a2,68(v0)
a1c00530:	9ab2      	lw	a1,72(v0)
a1c00532:	b308      	lw	v1,a1c00550 <hal_SysSetupPLL+0x40>
a1c00534:	ed8d      	or	a1,a0
a1c00536:	dab2      	sw	a1,72(v0)
a1c00538:	e820      	jr	ra
a1c0053a:	da60      	sw	v1,0(v0)
a1c0053c:	0001      	addiu	s0,sp,4
a1c0053e:	00a5      	addiu	s0,sp,660
a1c00540:	0000      	addiu	s0,sp,0
a1c00542:	a1a0      	lbu	a1,0(s1)
a1c00544:	0240      	addiu	v0,sp,256
a1c00546:	a1c0      	lbu	a2,0(s1)
a1c00548:	0000      	addiu	s0,sp,0
a1c0054a:	0100      	addiu	s1,sp,0
a1c0054c:	1000      	b	a1c0054e <hal_SysSetupPLL+0x3e>
a1c0054e:	1000      	b	a1c00550 <hal_SysSetupPLL+0x40>
a1c00550:	0000      	addiu	s0,sp,0
a1c00552:	00a5      	addiu	s0,sp,660

a1c00554 <hal_SysEnableClkOut>:
a1c00554:	b70d      	lw	a3,a1c00588 <hal_SysEnableClkOut+0x34>
a1c00556:	6a02      	li	v0,2
a1c00558:	9f66      	lw	v1,24(a3)
a1c0055a:	6eff      	li	a2,255
a1c0055c:	eb4d      	or	v1,v0
a1c0055e:	df66      	sw	v1,24(a3)
a1c00560:	ee8c      	and	a2,a0
a1c00562:	b30b      	lw	v1,a1c0058c <hal_SysEnableClkOut+0x38>
a1c00564:	b40b      	lw	a0,a1c00590 <hal_SysEnableClkOut+0x3c>
a1c00566:	f202 6d00 	li	a1,4608
a1c0056a:	db80      	sw	a0,0(v1)
a1c0056c:	dbb6      	sw	a1,88(v1)
a1c0056e:	9bef      	lw	a3,60(v1)
a1c00570:	f010 6a02 	li	v0,32770
a1c00574:	ef4d      	or	a3,v0
a1c00576:	b408      	lw	a0,a1c00594 <hal_SysEnableClkOut+0x40>
a1c00578:	b508      	lw	a1,a1c00598 <hal_SysEnableClkOut+0x44>
a1c0057a:	b209      	lw	v0,a1c0059c <hal_SysEnableClkOut+0x48>
a1c0057c:	4efe      	addiu	a2,-2
a1c0057e:	dbef      	sw	a3,60(v1)
a1c00580:	dbd8      	sw	a2,96(v1)
a1c00582:	dd84      	sw	a0,16(a1)
a1c00584:	e820      	jr	ra
a1c00586:	db40      	sw	v0,0(v1)
a1c00588:	6000      	bteqz	a1c0058a <hal_SysEnableClkOut+0x36>
a1c0058a:	a1a2      	lbu	a1,2(s1)
a1c0058c:	0000      	addiu	s0,sp,0
a1c0058e:	a1a0      	lbu	a1,0(s1)
a1c00590:	0001      	addiu	s0,sp,4
a1c00592:	00a5      	addiu	s0,sp,660
a1c00594:	003f      	addiu	s0,sp,252
a1c00596:	0010      	addiu	s0,sp,64
a1c00598:	d000      	sw	s0,0(sp)
a1c0059a:	a1a0      	lbu	a1,0(s1)
a1c0059c:	0000      	addiu	s0,sp,0
a1c0059e:	00a5      	addiu	s0,sp,660

a1c005a0 <hal_SysEnable8KSampleRate>:
a1c005a0:	b204      	lw	v0,a1c005b0 <hal_SysEnable8KSampleRate+0x10>
a1c005a2:	b505      	lw	a1,a1c005b4 <hal_SysEnable8KSampleRate+0x14>
a1c005a4:	b305      	lw	v1,a1c005b8 <hal_SysEnable8KSampleRate+0x18>
a1c005a6:	6c10      	li	a0,16
a1c005a8:	daa0      	sw	a1,0(v0)
a1c005aa:	da98      	sw	a0,96(v0)
a1c005ac:	e820      	jr	ra
a1c005ae:	da60      	sw	v1,0(v0)
a1c005b0:	0000      	addiu	s0,sp,0
a1c005b2:	a1a0      	lbu	a1,0(s1)
a1c005b4:	0001      	addiu	s0,sp,4
a1c005b6:	00a5      	addiu	s0,sp,660
a1c005b8:	0000      	addiu	s0,sp,0
a1c005ba:	00a5      	addiu	s0,sp,660

a1c005bc <hal_SysGetChipId>:
a1c005bc:	b20b      	lw	v0,a1c005e8 <hal_SysGetChipId+0x2c>
a1c005be:	9a40      	lw	v0,0(v0)
a1c005c0:	2c03      	bnez	a0,a1c005c8 <hal_SysGetChipId+0xc>
a1c005c2:	3342      	srl	v1,v0,8
a1c005c4:	e820      	jr	ra
a1c005c6:	3262      	srl	v0,v1,8
a1c005c8:	7401      	cmpi	a0,1
a1c005ca:	6106      	btnez	a1c005d8 <hal_SysGetChipId+0x1c>
a1c005cc:	f00e 6d00 	li	a1,28672
a1c005d0:	ed4c      	and	a1,v0
a1c005d2:	34a2      	srl	a0,a1,8
a1c005d4:	e820      	jr	ra
a1c005d6:	3292      	srl	v0,a0,4
a1c005d8:	7402      	cmpi	a0,2
a1c005da:	6103      	btnez	a1c005e2 <hal_SysGetChipId+0x26>
a1c005dc:	f7e1 6e1f 	li	a2,4095
a1c005e0:	eacc      	and	v0,a2
a1c005e2:	e820      	jr	ra
a1c005e4:	6500      	nop
a1c005e6:	6500      	nop
a1c005e8:	6000      	bteqz	a1c005ea <hal_SysGetChipId+0x2e>
a1c005ea:	a1a2      	lbu	a1,2(s1)

a1c005ec <hal_BootSectorValidateStruct>:
a1c005ec:	b306      	lw	v1,a1c00604 <hal_BootSectorValidateStruct+0x18>
a1c005ee:	b607      	lw	a2,a1c00608 <hal_BootSectorValidateStruct+0x1c>
a1c005f0:	b507      	lw	a1,a1c0060c <hal_BootSectorValidateStruct+0x20>
a1c005f2:	ddc0      	sw	a2,0(a1)
a1c005f4:	f0a0 9b40 	lw	v0,160(v1)
a1c005f8:	b406      	lw	a0,a1c00610 <hal_BootSectorValidateStruct+0x24>
a1c005fa:	ea8d      	or	v0,a0
a1c005fc:	f0a0 db40 	sw	v0,160(v1)
a1c00600:	e820      	jr	ra
a1c00602:	6500      	nop
a1c00604:	0000      	addiu	s0,sp,0
a1c00606:	a1a0      	lbu	a1,0(s1)
a1c00608:	0000      	addiu	s0,sp,0
a1c0060a:	a1c0      	lbu	a2,0(s1)
a1c0060c:	0278      	addiu	v0,sp,480
a1c0060e:	a1c0      	lbu	a2,0(s1)
a1c00610:	0000      	addiu	s0,sp,0
a1c00612:	0040      	addiu	s0,sp,256

a1c00614 <hal_BootSectorSetEbcConfig>:
a1c00614:	b306      	lw	v1,a1c0062c <$LCFI2+0x14>
a1c00616:	63fd      	addiu	sp,-24

a1c00618 <$LCFI2>:
a1c00618:	b206      	lw	v0,a1c00630 <$LCFI2+0x18>
a1c0061a:	6205      	sw	ra,20(sp)
a1c0061c:	da82      	sw	a0,8(v0)
a1c0061e:	daa3      	sw	a1,12(v0)
a1c00620:	1a03 017b 	jal	a1c005ec <hal_BootSectorValidateStruct>
a1c00624:	da61      	sw	v1,4(v0)
a1c00626:	9705      	lw	a3,20(sp)
a1c00628:	ef00      	jr	a3
a1c0062a:	6303      	addiu	sp,24
a1c0062c:	5ec7      	sltiu	a2,199
a1c0062e:	b007      	lw	s0,a1c00648 <hal_BootSectorSetEbcRamId>
a1c00630:	0000      	addiu	s0,sp,0
a1c00632:	a1c0      	lbu	a2,0(s1)

a1c00634 <hal_BootSectorSetEbcFreq>:
a1c00634:	b303      	lw	v1,a1c00640 <hal_BootSectorSetEbcFreq+0xc>
a1c00636:	b204      	lw	v0,a1c00644 <hal_BootSectorSetEbcFreq+0x10>
a1c00638:	da85      	sw	a0,20(v0)
a1c0063a:	e820      	jr	ra
a1c0063c:	da64      	sw	v1,16(v0)
a1c0063e:	6500      	nop
a1c00640:	0e28      	la	a2,a1c006e0 <boot_Loader+0x10>
a1c00642:	ebc0      	jalrc	v1
a1c00644:	0000      	addiu	s0,sp,0
a1c00646:	a1c0      	lbu	a2,0(s1)

a1c00648 <hal_BootSectorSetEbcRamId>:
a1c00648:	b202      	lw	v0,a1c00650 <hal_BootSectorSetEbcRamId+0x8>
a1c0064a:	e820      	jr	ra
a1c0064c:	c298      	sb	a0,24(v0)
a1c0064e:	6500      	nop
a1c00650:	0000      	addiu	s0,sp,0
a1c00652:	a1c0      	lbu	a2,0(s1)

a1c00654 <hal_BootSectorGetEbcRamId>:
a1c00654:	b202      	lw	v0,a1c0065c <hal_BootSectorGetEbcRamId+0x8>
a1c00656:	e820      	jr	ra
a1c00658:	a258      	lbu	v0,24(v0)
a1c0065a:	6500      	nop
a1c0065c:	0000      	addiu	s0,sp,0
a1c0065e:	a1c0      	lbu	a2,0(s1)

a1c00660 <boot_LoaderEnter>:
a1c00660:	3c06a1c1 	lui	a2,0xa1c1
a1c00664:	3c05a1c0 	lui	a1,0xa1c0
a1c00668:	24c3fffc 	addiu	v1,a2,-4
a1c0066c:	aca300c4 	sw	v1,196(a1)
a1c00670:	27bdffe8 	addiu	sp,sp,-24
a1c00674:	afbf0014 	sw	ra,20(sp)
a1c00678:	8ca200c4 	lw	v0,196(a1)
a1c0067c:	00000000 	nop
a1c00680:	0c7001b4 	jal	a1c006d0 <boot_Loader>
a1c00684:	0040e821 	move	sp,v0
a1c00688:	00000000 	nop
a1c0068c:	8fbf0014 	lw	ra,20(sp)
a1c00690:	00000000 	nop
a1c00694:	03e00008 	jr	ra
a1c00698:	27bd0018 	addiu	sp,sp,24

a1c0069c <boot_BreakBootMain>:
a1c0069c:	27bdffe8 	addiu	sp,sp,-24
a1c006a0:	afbf0014 	sw	ra,20(sp)
a1c006a4:	0001000d 	break	0x1
	...
a1c006b4:	00002021 	move	a0,zero
a1c006b8:	0c700213 	jal	a1c0084c <main>
a1c006bc:	00002821 	move	a1,zero
a1c006c0:	8fbf0014 	lw	ra,20(sp)
a1c006c4:	00000000 	nop
a1c006c8:	03e00008 	jr	ra
a1c006cc:	27bd0018 	addiu	sp,sp,24

a1c006d0 <boot_Loader>:
a1c006d0:	27bdffe0 	addiu	sp,sp,-32
a1c006d4:	3c08a1c0 	lui	t0,0xa1c0
a1c006d8:	3c07a1c0 	lui	a3,0xa1c0
a1c006dc:	3c06a1c0 	lui	a2,0xa1c0
a1c006e0:	3c03a1c0 	lui	v1,0xa1c0
a1c006e4:	3c020000 	lui	v0,0x0
a1c006e8:	3c050000 	lui	a1,0x0
a1c006ec:	ad0000b8 	sw	zero,184(t0)
a1c006f0:	afbf001c 	sw	ra,28(sp)
a1c006f4:	ace000c0 	sw	zero,192(a3)
a1c006f8:	acc000bc 	sw	zero,188(a2)
a1c006fc:	24630280 	addiu	v1,v1,640
a1c00700:	24420000 	addiu	v0,v0,0
a1c00704:	087001c7 	j	a1c0071c <boot_Loader+0x4c>
a1c00708:	24a50000 	addiu	a1,a1,0
a1c0070c:	8c490000 	lw	t1,0(v0)
a1c00710:	24420004 	addiu	v0,v0,4
a1c00714:	ac690000 	sw	t1,0(v1)
a1c00718:	24630004 	addiu	v1,v1,4
a1c0071c:	0045502b 	sltu	t2,v0,a1
a1c00720:	1540fffa 	bnez	t2,a1c0070c <boot_Loader+0x3c>
a1c00724:	3c1fa1c0 	lui	ra,0xa1c0
a1c00728:	3c190000 	lui	t9,0x0
a1c0072c:	3c180000 	lui	t8,0x0
a1c00730:	27e30280 	addiu	v1,ra,640
a1c00734:	27220000 	addiu	v0,t9,0
a1c00738:	087001d4 	j	a1c00750 <boot_Loader+0x80>
a1c0073c:	27050000 	addiu	a1,t8,0
a1c00740:	8c460000 	lw	a2,0(v0)
a1c00744:	24420004 	addiu	v0,v0,4
a1c00748:	ac660000 	sw	a2,0(v1)
a1c0074c:	24630004 	addiu	v1,v1,4
a1c00750:	0045382b 	sltu	a3,v0,a1
a1c00754:	14e0fffa 	bnez	a3,a1c00740 <boot_Loader+0x70>
a1c00758:	00000000 	nop
a1c0075c:	3c02a1c0 	lui	v0,0xa1c0
a1c00760:	3c05a1c1 	lui	a1,0xa1c1
a1c00764:	24422890 	addiu	v0,v0,10384
a1c00768:	087001de 	j	a1c00778 <boot_Loader+0xa8>
a1c0076c:	24a38cb0 	addiu	v1,a1,-29520
a1c00770:	ac400000 	sw	zero,0(v0)
a1c00774:	24420004 	addiu	v0,v0,4
a1c00778:	0043402b 	sltu	t0,v0,v1
a1c0077c:	1500fffc 	bnez	t0,a1c00770 <boot_Loader+0xa0>
a1c00780:	00000000 	nop
a1c00784:	74700420 	jalx	a1c01080 <COS_MemInit>
a1c00788:	afa40010 	sw	a0,16(sp)
a1c0078c:	74700421 	jalx	a1c01084 <tgt_Open>
a1c00790:	00000000 	nop
a1c00794:	747006e1 	jalx	a1c01b84 <hal_MapEngineOpen>
a1c00798:	00000000 	nop
a1c0079c:	3c19a1a0 	lui	t9,0xa1a0
a1c007a0:	24182f01 	li	t8,12033
a1c007a4:	372ad000 	ori	t2,t9,0xd000
a1c007a8:	ad580020 	sw	t8,32(t2)
a1c007ac:	3c09b4ea 	lui	t1,0xb4ea
a1c007b0:	8fa40010 	lw	a0,16(sp)
a1c007b4:	3523b007 	ori	v1,t1,0xb007
a1c007b8:	14830004 	bne	a0,v1,a1c007cc <boot_Loader+0xfc>
a1c007bc:	00002021 	move	a0,zero
a1c007c0:	8fbf001c 	lw	ra,28(sp)
a1c007c4:	087001a7 	j	a1c0069c <boot_BreakBootMain>
a1c007c8:	27bd0020 	addiu	sp,sp,32
a1c007cc:	0c700213 	jal	a1c0084c <main>
a1c007d0:	00002821 	move	a1,zero
a1c007d4:	8fbf001c 	lw	ra,28(sp)
a1c007d8:	00000000 	nop
a1c007dc:	03e00008 	jr	ra
a1c007e0:	27bd0020 	addiu	sp,sp,32

a1c007e4 <fp_CheckDataBuffer>:
a1c007e4:	3c02a1c0 	lui	v0,0xa1c0
a1c007e8:	3c08a1c0 	lui	t0,0xa1c0
a1c007ec:	3c07a1c0 	lui	a3,0xa1c0
a1c007f0:	3c06a1c0 	lui	a2,0xa1c0
a1c007f4:	2442253c 	addiu	v0,v0,9532
a1c007f8:	25082554 	addiu	t0,t0,9556
a1c007fc:	24e72554 	addiu	a3,a3,9556
a1c00800:	0870020d 	j	a1c00834 <fp_CheckDataBuffer+0x50>
a1c00804:	24c62564 	addiu	a2,a2,9572
a1c00808:	8c490000 	lw	t1,0(v0)
a1c0080c:	8c640000 	lw	a0,0(v1)
a1c00810:	24630004 	addiu	v1,v1,4
a1c00814:	ad240000 	sw	a0,0(t1)
a1c00818:	8d250000 	lw	a1,0(t1)
a1c0081c:	00000000 	nop
a1c00820:	14a40008 	bne	a1,a0,a1c00844 <fp_CheckDataBuffer+0x60>
a1c00824:	00000000 	nop
a1c00828:	1466fff7 	bne	v1,a2,a1c00808 <fp_CheckDataBuffer+0x24>
a1c0082c:	00000000 	nop
a1c00830:	24420004 	addiu	v0,v0,4
a1c00834:	1448fffc 	bne	v0,t0,a1c00828 <fp_CheckDataBuffer+0x44>
a1c00838:	00e01821 	move	v1,a3
a1c0083c:	03e00008 	jr	ra
a1c00840:	24020001 	li	v0,1
a1c00844:	03e00008 	jr	ra
a1c00848:	00001021 	move	v0,zero

a1c0084c <main>:
a1c0084c:	3c09a1c0 	lui	t1,0xa1c0
a1c00850:	25282750 	addiu	t0,t1,10064
a1c00854:	3c062000 	lui	a2,0x2000
a1c00858:	27bdff80 	addiu	sp,sp,-128
a1c0085c:	01062025 	or	a0,t0,a2
a1c00860:	3c07d9ef 	lui	a3,0xd9ef
a1c00864:	3c050632 	lui	a1,0x632
a1c00868:	afa40054 	sw	a0,84(sp)
a1c0086c:	34e30045 	ori	v1,a3,0x45
a1c00870:	34a5ea00 	ori	a1,a1,0xea00
a1c00874:	24040020 	li	a0,32
a1c00878:	00003021 	move	a2,zero
a1c0087c:	afbf007c 	sw	ra,124(sp)
a1c00880:	afb10078 	sw	s1,120(sp)
a1c00884:	afb00074 	sw	s0,116(sp)
a1c00888:	afa3003c 	sw	v1,60(sp)
a1c0088c:	afa00038 	sw	zero,56(sp)
a1c00890:	afa00040 	sw	zero,64(sp)
a1c00894:	74700890 	jalx	a1c02240 <hal_SysRequestFreq>
a1c00898:	afa00044 	sw	zero,68(sp)
a1c0089c:	8fa20054 	lw	v0,84(sp)
a1c008a0:	00000000 	nop
a1c008a4:	ac400000 	sw	zero,0(v0)
a1c008a8:	ac400014 	sw	zero,20(v0)
a1c008ac:	3c1b0040 	lui	k1,0x40
a1c008b0:	377bc001 	ori	k1,k1,0xc001
a1c008b4:	40806800 	mtc0	zero,$13
a1c008b8:	00000000 	nop
a1c008bc:	409b6000 	mtc0	k1,$12
a1c008c0:	74700130 	jalx	a1c004c0 <tgt_GetMemdFlashConfig>
a1c008c4:	241b0001 	li	k1,1
a1c008c8:	747004d3 	jalx	a1c0134c <memd_FlashOpen>
a1c008cc:	00402021 	move	a0,v0
a1c008d0:	27a40018 	addiu	a0,sp,24
a1c008d4:	27a5001c 	addiu	a1,sp,28
a1c008d8:	27a60020 	addiu	a2,sp,32
a1c008dc:	74700459 	jalx	a1c01164 <memd_FlashGetUserFieldInfo>
a1c008e0:	27a70024 	addiu	a3,sp,36
a1c008e4:	0c7001f9 	jal	a1c007e4 <fp_CheckDataBuffer>
a1c008e8:	00000000 	nop
a1c008ec:	14400021 	bnez	v0,a1c00974 <main+0x128>
a1c008f0:	3c0aa1c0 	lui	t2,0xa1c0
a1c008f4:	74780140 	jalx	a1e00500 <mon_Event>
a1c008f8:	240400c0 	li	a0,192
a1c008fc:	74700760 	jalx	a1c01d80 <hal_SysResetOut>
a1c00900:	24040001 	li	a0,1
a1c00904:	747005eb 	jalx	a1c017ac <pmd_EnableMemoryPower>
a1c00908:	00002021 	move	a0,zero
a1c0090c:	74780920 	jalx	a1e02480 <hal_TimDelay>
a1c00910:	34048000 	li	a0,0x8000
a1c00914:	747005eb 	jalx	a1c017ac <pmd_EnableMemoryPower>
a1c00918:	24040001 	li	a0,1
a1c0091c:	74780920 	jalx	a1e02480 <hal_TimDelay>
a1c00920:	2404000a 	li	a0,10
a1c00924:	74700760 	jalx	a1c01d80 <hal_SysResetOut>
a1c00928:	00002021 	move	a0,zero
a1c0092c:	74780920 	jalx	a1e02480 <hal_TimDelay>
a1c00930:	2404000a 	li	a0,10
a1c00934:	74700130 	jalx	a1c004c0 <tgt_GetMemdFlashConfig>
a1c00938:	00000000 	nop
a1c0093c:	747004d3 	jalx	a1c0134c <memd_FlashOpen>
a1c00940:	00402021 	move	a0,v0
a1c00944:	74700133 	jalx	a1c004cc <tgt_GetMemdRamConfig>
a1c00948:	00000000 	nop
a1c0094c:	7470013a 	jalx	a1c004e8 <memd_RamOpen>
a1c00950:	00402021 	move	a0,v0
a1c00954:	0c7001f9 	jal	a1c007e4 <fp_CheckDataBuffer>
a1c00958:	00000000 	nop
a1c0095c:	14400005 	bnez	v0,a1c00974 <main+0x128>
a1c00960:	3c0aa1c0 	lui	t2,0xa1c0
a1c00964:	74780140 	jalx	a1e00500 <mon_Event>
a1c00968:	240400ce 	li	a0,206
a1c0096c:	0870025b 	j	a1c0096c <main+0x120>
a1c00970:	00000000 	nop
a1c00974:	3c10a1c0 	lui	s0,0xa1c0
a1c00978:	2605273c 	addiu	a1,s0,10044
a1c0097c:	2546274c 	addiu	a2,t2,10060
a1c00980:	747006ea 	jalx	a1c01ba8 <hal_MapEngineRegisterModule>
a1c00984:	24040014 	li	a0,20
a1c00988:	74780140 	jalx	a1e00500 <mon_Event>
a1c0098c:	240400f0 	li	a0,240
a1c00990:	00008821 	move	s1,zero
a1c00994:	afa0004c 	sw	zero,76(sp)
a1c00998:	afa00048 	sw	zero,72(sp)
a1c0099c:	afa00050 	sw	zero,80(sp)
a1c009a0:	8fb90054 	lw	t9,84(sp)
a1c009a4:	24180014 	li	t8,20
a1c009a8:	03200013 	mtlo	t9
a1c009ac:	24030002 	li	v1,2
a1c009b0:	0238001c 	madd	s1,t8
a1c009b4:	00008012 	mflo	s0
a1c009b8:	8e020000 	lw	v0,0(s0)
a1c009bc:	241f0005 	li	ra,5
a1c009c0:	105f00ba 	beq	v0,ra,a1c00cac <main+0x460>
a1c009c4:	2c440006 	sltiu	a0,v0,6
a1c009c8:	10800012 	beqz	a0,a1c00a14 <main+0x1c8>
a1c009cc:	24040008 	li	a0,8
a1c009d0:	1043007d 	beq	v0,v1,a1c00bc8 <main+0x37c>
a1c009d4:	2c440003 	sltiu	a0,v0,3
a1c009d8:	10800008 	beqz	a0,a1c009fc <main+0x1b0>
a1c009dc:	24070003 	li	a3,3
a1c009e0:	1040fff5 	beqz	v0,a1c009b8 <main+0x16c>
a1c009e4:	00000000 	nop
a1c009e8:	24030001 	li	v1,1
a1c009ec:	1443017c 	bne	v0,v1,a1c00fe0 <main+0x794>
a1c009f0:	2407ff9d 	li	a3,-99
a1c009f4:	0870029a 	j	a1c00a68 <main+0x21c>
a1c009f8:	00000000 	nop
a1c009fc:	104700f6 	beq	v0,a3,a1c00dd8 <main+0x58c>
a1c00a00:	24050004 	li	a1,4
a1c00a04:	14450176 	bne	v0,a1,a1c00fe0 <main+0x794>
a1c00a08:	2407ff9d 	li	a3,-99
a1c00a0c:	08700298 	j	a1c00a60 <main+0x214>
a1c00a10:	27a30038 	addiu	v1,sp,56
a1c00a14:	104400f4 	beq	v0,a0,a1c00de8 <main+0x59c>
a1c00a18:	2c450009 	sltiu	a1,v0,9
a1c00a1c:	24460064 	addiu	a2,v0,100
a1c00a20:	10a00008 	beqz	a1,a1c00a44 <main+0x1f8>
a1c00a24:	2cc40004 	sltiu	a0,a2,4
a1c00a28:	24080006 	li	t0,6
a1c00a2c:	10480009 	beq	v0,t0,a1c00a54 <main+0x208>
a1c00a30:	24090007 	li	t1,7
a1c00a34:	1449016a 	bne	v0,t1,a1c00fe0 <main+0x794>
a1c00a38:	2407ff9d 	li	a3,-99
a1c00a3c:	087003ef 	j	a1c00fbc <main+0x770>
a1c00a40:	240aff9c 	li	t2,-100
a1c00a44:	10800166 	beqz	a0,a1c00fe0 <main+0x794>
a1c00a48:	2407ff9d 	li	a3,-99
a1c00a4c:	0870026e 	j	a1c009b8 <main+0x16c>
a1c00a50:	00000000 	nop
a1c00a54:	27a2003c 	addiu	v0,sp,60
a1c00a58:	0870030f 	j	a1c00c3c <main+0x3f0>
a1c00a5c:	00001821 	move	v1,zero
a1c00a60:	087003d4 	j	a1c00f50 <main+0x704>
a1c00a64:	00004021 	move	t0,zero
a1c00a68:	8fa50050 	lw	a1,80(sp)
a1c00a6c:	00000000 	nop
a1c00a70:	10a00005 	beqz	a1,a1c00a88 <main+0x23c>
a1c00a74:	00000000 	nop
a1c00a78:	8e020004 	lw	v0,4(s0)
a1c00a7c:	00000000 	nop
a1c00a80:	1445003a 	bne	v0,a1,a1c00b6c <main+0x320>
a1c00a84:	2402ff9c 	li	v0,-100
a1c00a88:	8e0a0010 	lw	t2,16(s0)
a1c00a8c:	00000000 	nop
a1c00a90:	0541015c 	bgez	t2,a1c01004 <main+0x7b8>
a1c00a94:	00000000 	nop
a1c00a98:	8e180008 	lw	t8,8(s0)
a1c00a9c:	27a4002c 	addiu	a0,sp,44
a1c00aa0:	afb8002c 	sw	t8,44(sp)
a1c00aa4:	8e1f000c 	lw	ra,12(s0)
a1c00aa8:	afa00034 	sw	zero,52(sp)
a1c00aac:	a7bf0030 	sh	ra,48(sp)
a1c00ab0:	74700653 	jalx	a1c0194c <hal_DmaFcsStart>
a1c00ab4:	00000000 	nop
a1c00ab8:	747005fd 	jalx	a1c017f4 <hal_DmaReadFcs>
a1c00abc:	27a40010 	addiu	a0,sp,16
a1c00ac0:	93aa0010 	lbu	t2,16(sp)
a1c00ac4:	93a20011 	lbu	v0,17(sp)
a1c00ac8:	000a4c00 	sll	t1,t2,0x10
a1c00acc:	00024200 	sll	t0,v0,0x8
a1c00ad0:	8e030010 	lw	v1,16(s0)
a1c00ad4:	93a40012 	lbu	a0,18(sp)
a1c00ad8:	3c0500ff 	lui	a1,0xff
a1c00adc:	01283025 	or	a2,t1,t0
a1c00ae0:	34a7ffff 	ori	a3,a1,0xffff
a1c00ae4:	00c4c025 	or	t8,a2,a0
a1c00ae8:	0067c824 	and	t9,v1,a3
a1c00aec:	13380005 	beq	t9,t8,a1c00b04 <main+0x2b8>
a1c00af0:	2419ff9e 	li	t9,-98
a1c00af4:	ae190000 	sw	t9,0(s0)
a1c00af8:	8e100004 	lw	s0,4(s0)
a1c00afc:	087002ec 	j	a1c00bb0 <main+0x364>
a1c00b00:	afb00050 	sw	s0,80(sp)
a1c00b04:	8e040004 	lw	a0,4(s0)
a1c00b08:	74700468 	jalx	a1c011a0 <memd_FlashGetGlobalAddress>
a1c00b0c:	00000000 	nop
a1c00b10:	3c032000 	lui	v1,0x2000
a1c00b14:	00432025 	or	a0,v0,v1
a1c00b18:	00001021 	move	v0,zero
a1c00b1c:	087002d4 	j	a1c00b50 <main+0x304>
a1c00b20:	2403ffff 	li	v1,-1
a1c00b24:	8c870000 	lw	a3,0(a0)
a1c00b28:	00000000 	nop
a1c00b2c:	10e30007 	beq	a3,v1,a1c00b4c <main+0x300>
a1c00b30:	24420004 	addiu	v0,v0,4
a1c00b34:	74780140 	jalx	a1e00500 <mon_Event>
a1c00b38:	00000000 	nop
a1c00b3c:	2405ff9f 	li	a1,-97
a1c00b40:	ae050000 	sw	a1,0(s0)
a1c00b44:	087002ec 	j	a1c00bb0 <main+0x364>
a1c00b48:	afa00050 	sw	zero,80(sp)
a1c00b4c:	24840004 	addiu	a0,a0,4
a1c00b50:	8e08000c 	lw	t0,12(s0)
a1c00b54:	00000000 	nop
a1c00b58:	0048302b 	sltu	a2,v0,t0
a1c00b5c:	14c0fff1 	bnez	a2,a1c00b24 <main+0x2d8>
a1c00b60:	00000000 	nop
a1c00b64:	08700401 	j	a1c01004 <main+0x7b8>
a1c00b68:	afa00050 	sw	zero,80(sp)
a1c00b6c:	ae020000 	sw	v0,0(s0)
a1c00b70:	087002ec 	j	a1c00bb0 <main+0x364>
a1c00b74:	00000000 	nop
a1c00b78:	8c49fffc 	lw	t1,-4(v0)
a1c00b7c:	8e0a0004 	lw	t2,4(s0)
a1c00b80:	00000000 	nop
a1c00b84:	15490003 	bne	t2,t1,a1c00b94 <main+0x348>
a1c00b88:	24420008 	addiu	v0,v0,8
a1c00b8c:	0064f804 	sllv	ra,a0,v1
a1c00b90:	00bf2825 	or	a1,a1,ra
a1c00b94:	24630001 	addiu	v1,v1,1
a1c00b98:	8c580000 	lw	t8,0(v0)
a1c00b9c:	00000000 	nop
a1c00ba0:	1700fff5 	bnez	t8,a1c00b78 <main+0x32c>
a1c00ba4:	00000000 	nop
a1c00ba8:	08700324 	j	a1c00c90 <main+0x444>
a1c00bac:	afa5004c 	sw	a1,76(sp)
a1c00bb0:	74780140 	jalx	a1e00500 <mon_Event>
a1c00bb4:	262400e0 	addiu	a0,s1,224
a1c00bb8:	8fa20048 	lw	v0,72(sp)
a1c00bbc:	3c040ef0 	lui	a0,0xef0
a1c00bc0:	0870030c 	j	a1c00c30 <main+0x3e4>
a1c00bc4:	00822823 	subu	a1,a0,v0
a1c00bc8:	8e030004 	lw	v1,4(s0)
a1c00bcc:	8fb90018 	lw	t9,24(sp)
a1c00bd0:	00000000 	nop
a1c00bd4:	14790006 	bne	v1,t9,a1c00bf0 <main+0x3a4>
a1c00bd8:	00000000 	nop
a1c00bdc:	8e040004 	lw	a0,4(s0)
a1c00be0:	8fa6001c 	lw	a2,28(sp)
a1c00be4:	8e070004 	lw	a3,4(s0)
a1c00be8:	087002fe 	j	a1c00bf8 <main+0x3ac>
a1c00bec:	00e62821 	addu	a1,a3,a2
a1c00bf0:	8e040004 	lw	a0,4(s0)
a1c00bf4:	00002821 	move	a1,zero
a1c00bf8:	74780cdc 	jalx	a1e03370 <memd_FlashErase>
a1c00bfc:	00000000 	nop
a1c00c00:	afa20048 	sw	v0,72(sp)
a1c00c04:	8fa50048 	lw	a1,72(sp)
a1c00c08:	00000000 	nop
a1c00c0c:	10a00021 	beqz	a1,a1c00c94 <main+0x448>
a1c00c10:	2402ff9c 	li	v0,-100
a1c00c14:	2409ff9d 	li	t1,-99
a1c00c18:	ae090000 	sw	t1,0(s0)
a1c00c1c:	74780140 	jalx	a1e00500 <mon_Event>
a1c00c20:	262400e0 	addiu	a0,s1,224
a1c00c24:	8fa80048 	lw	t0,72(sp)
a1c00c28:	3c100ee0 	lui	s0,0xee0
a1c00c2c:	02082823 	subu	a1,s0,t0
a1c00c30:	087003fd 	j	a1c00ff4 <main+0x7a8>
a1c00c34:	24040001 	li	a0,1
a1c00c38:	24630001 	addiu	v1,v1,1
a1c00c3c:	8c4a0000 	lw	t2,0(v0)
a1c00c40:	00000000 	nop
a1c00c44:	1540fffc 	bnez	t2,a1c00c38 <main+0x3ec>
a1c00c48:	24420008 	addiu	v0,v0,8
a1c00c4c:	8e040008 	lw	a0,8(s0)
a1c00c50:	3c182000 	lui	t8,0x2000
a1c00c54:	00981025 	or	v0,a0,t8
a1c00c58:	0003f8c0 	sll	ra,v1,0x3
a1c00c5c:	03e22821 	addu	a1,ra,v0
a1c00c60:	08700321 	j	a1c00c84 <main+0x438>
a1c00c64:	27a40038 	addiu	a0,sp,56
a1c00c68:	8c870000 	lw	a3,0(a0)
a1c00c6c:	00000000 	nop
a1c00c70:	ac470000 	sw	a3,0(v0)
a1c00c74:	8c990004 	lw	t9,4(a0)
a1c00c78:	24840008 	addiu	a0,a0,8
a1c00c7c:	ac590004 	sw	t9,4(v0)
a1c00c80:	24420008 	addiu	v0,v0,8
a1c00c84:	1445fff8 	bne	v0,a1,a1c00c68 <main+0x41c>
a1c00c88:	00000000 	nop
a1c00c8c:	ae03000c 	sw	v1,12(s0)
a1c00c90:	2402ff9c 	li	v0,-100
a1c00c94:	262400f0 	addiu	a0,s1,240
a1c00c98:	ae020000 	sw	v0,0(s0)
a1c00c9c:	74780140 	jalx	a1e00500 <mon_Event>
a1c00ca0:	3a310001 	xori	s1,s1,0x1
a1c00ca4:	08700268 	j	a1c009a0 <main+0x154>
a1c00ca8:	00000000 	nop
a1c00cac:	afa00014 	sw	zero,20(sp)
a1c00cb0:	8e050008 	lw	a1,8(s0)
a1c00cb4:	8e0a0008 	lw	t2,8(s0)
a1c00cb8:	8e090008 	lw	t1,8(s0)
a1c00cbc:	3c082000 	lui	t0,0x2000
a1c00cc0:	25430004 	addiu	v1,t2,4
a1c00cc4:	25260008 	addiu	a2,t1,8
a1c00cc8:	00683825 	or	a3,v1,t0
a1c00ccc:	00c83025 	or	a2,a2,t0
a1c00cd0:	00a82825 	or	a1,a1,t0
a1c00cd4:	00001821 	move	v1,zero
a1c00cd8:	0870036c 	j	a1c00db0 <main+0x564>
a1c00cdc:	2408ffff 	li	t0,-1
a1c00ce0:	8ca40000 	lw	a0,0(a1)
a1c00ce4:	afa30064 	sw	v1,100(sp)
a1c00ce8:	afa50058 	sw	a1,88(sp)
a1c00cec:	afa6005c 	sw	a2,92(sp)
a1c00cf0:	afa70060 	sw	a3,96(sp)
a1c00cf4:	74700468 	jalx	a1c011a0 <memd_FlashGetGlobalAddress>
a1c00cf8:	afa80068 	sw	t0,104(sp)
a1c00cfc:	3c042000 	lui	a0,0x2000
a1c00d00:	00443825 	or	a3,v0,a0
a1c00d04:	afa7002c 	sw	a3,44(sp)
a1c00d08:	8fa70060 	lw	a3,96(sp)
a1c00d0c:	8fa50058 	lw	a1,88(sp)
a1c00d10:	8ce20000 	lw	v0,0(a3)
a1c00d14:	afa00034 	sw	zero,52(sp)
a1c00d18:	a7a20030 	sh	v0,48(sp)
a1c00d1c:	8cb90000 	lw	t9,0(a1)
a1c00d20:	8fb80018 	lw	t8,24(sp)
a1c00d24:	8fa30064 	lw	v1,100(sp)
a1c00d28:	8fa6005c 	lw	a2,92(sp)
a1c00d2c:	8fa80068 	lw	t0,104(sp)
a1c00d30:	1338001e 	beq	t9,t8,a1c00dac <main+0x560>
a1c00d34:	27a4002c 	addiu	a0,sp,44
a1c00d38:	74700653 	jalx	a1c0194c <hal_DmaFcsStart>
a1c00d3c:	00000000 	nop
a1c00d40:	747005fd 	jalx	a1c017f4 <hal_DmaReadFcs>
a1c00d44:	27a40010 	addiu	a0,sp,16
a1c00d48:	93b80010 	lbu	t8,16(sp)
a1c00d4c:	93a90011 	lbu	t1,17(sp)
a1c00d50:	00183400 	sll	a2,t8,0x10
a1c00d54:	00092a00 	sll	a1,t1,0x8
a1c00d58:	00c51825 	or	v1,a2,a1
a1c00d5c:	8fa6005c 	lw	a2,92(sp)
a1c00d60:	93a80012 	lbu	t0,18(sp)
a1c00d64:	8cca0000 	lw	t2,0(a2)
a1c00d68:	00681025 	or	v0,v1,t0
a1c00d6c:	8fa50058 	lw	a1,88(sp)
a1c00d70:	8fa30064 	lw	v1,100(sp)
a1c00d74:	8fa70060 	lw	a3,96(sp)
a1c00d78:	8fa80068 	lw	t0,104(sp)
a1c00d7c:	11420008 	beq	t2,v0,a1c00da0 <main+0x554>
a1c00d80:	241fffff 	li	ra,-1
a1c00d84:	8fa40014 	lw	a0,20(sp)
a1c00d88:	00000000 	nop
a1c00d8c:	24990001 	addiu	t9,a0,1
a1c00d90:	afb90014 	sw	t9,20(sp)
a1c00d94:	151f0002 	bne	t0,ra,a1c00da0 <main+0x554>
a1c00d98:	acc20000 	sw	v0,0(a2)
a1c00d9c:	00604021 	move	t0,v1
a1c00da0:	24a5000c 	addiu	a1,a1,12
a1c00da4:	24e7000c 	addiu	a3,a3,12
a1c00da8:	24c6000c 	addiu	a2,a2,12
a1c00dac:	24630001 	addiu	v1,v1,1
a1c00db0:	8e0a000c 	lw	t2,12(s0)
a1c00db4:	00000000 	nop
a1c00db8:	006a102b 	sltu	v0,v1,t2
a1c00dbc:	1440ffc8 	bnez	v0,a1c00ce0 <main+0x494>
a1c00dc0:	00000000 	nop
a1c00dc4:	8fa70014 	lw	a3,20(sp)
a1c00dc8:	00000000 	nop
a1c00dcc:	10e0ffb1 	beqz	a3,a1c00c94 <main+0x448>
a1c00dd0:	2402ff9c 	li	v0,-100
a1c00dd4:	ae080008 	sw	t0,8(s0)
a1c00dd8:	2403ff9d 	li	v1,-99
a1c00ddc:	ae030000 	sw	v1,0(s0)
a1c00de0:	087003b4 	j	a1c00ed0 <main+0x684>
a1c00de4:	262400e0 	addiu	a0,s1,224
a1c00de8:	8e050004 	lw	a1,4(s0)
a1c00dec:	3c088000 	lui	t0,0x8000
a1c00df0:	00a81025 	or	v0,a1,t0
a1c00df4:	afa2002c 	sw	v0,44(sp)
a1c00df8:	90430003 	lbu	v1,3(v0)
a1c00dfc:	90470002 	lbu	a3,2(v0)
a1c00e00:	00035600 	sll	t2,v1,0x18
a1c00e04:	00072400 	sll	a0,a3,0x10
a1c00e08:	90580001 	lbu	t8,1(v0)
a1c00e0c:	90590000 	lbu	t9,0(v0)
a1c00e10:	0144f825 	or	ra,t2,a0
a1c00e14:	03f93025 	or	a2,ra,t9
a1c00e18:	00184a00 	sll	t1,t8,0x8
a1c00e1c:	00c92825 	or	a1,a2,t1
a1c00e20:	74700142 	jalx	a1c00508 <hal_HstSendEvent>
a1c00e24:	24040001 	li	a0,1
a1c00e28:	8fa2002c 	lw	v0,44(sp)
a1c00e2c:	240800ca 	li	t0,202
a1c00e30:	90450003 	lbu	a1,3(v0)
a1c00e34:	00000000 	nop
a1c00e38:	14a80023 	bne	a1,t0,a1c00ec8 <main+0x67c>
a1c00e3c:	2418ff9c 	li	t8,-100
a1c00e40:	90490002 	lbu	t1,2(v0)
a1c00e44:	2406001b 	li	a2,27
a1c00e48:	1526001f 	bne	t1,a2,a1c00ec8 <main+0x67c>
a1c00e4c:	00000000 	nop
a1c00e50:	90440001 	lbu	a0,1(v0)
a1c00e54:	904a0000 	lbu	t2,0(v0)
a1c00e58:	0004ca00 	sll	t9,a0,0x8
a1c00e5c:	032af825 	or	ra,t9,t2
a1c00e60:	2bf80204 	slti	t8,ra,516
a1c00e64:	1700ff8b 	bnez	t8,a1c00c94 <main+0x448>
a1c00e68:	2402ff9c 	li	v0,-100
a1c00e6c:	27a4002c 	addiu	a0,sp,44
a1c00e70:	24021360 	li	v0,4960
a1c00e74:	a7a20030 	sh	v0,48(sp)
a1c00e78:	74700653 	jalx	a1c0194c <hal_DmaFcsStart>
a1c00e7c:	afa00034 	sw	zero,52(sp)
a1c00e80:	747005fd 	jalx	a1c017f4 <hal_DmaReadFcs>
a1c00e84:	27a40010 	addiu	a0,sp,16
a1c00e88:	8fa2002c 	lw	v0,44(sp)
a1c00e8c:	93a30010 	lbu	v1,16(sp)
a1c00e90:	90471cf8 	lbu	a3,7416(v0)
a1c00e94:	00000000 	nop
a1c00e98:	1467000b 	bne	v1,a3,a1c00ec8 <main+0x67c>
a1c00e9c:	2418ff9c 	li	t8,-100
a1c00ea0:	93a50011 	lbu	a1,17(sp)
a1c00ea4:	90481cf9 	lbu	t0,7417(v0)
a1c00ea8:	00000000 	nop
a1c00eac:	14a80006 	bne	a1,t0,a1c00ec8 <main+0x67c>
a1c00eb0:	00000000 	nop
a1c00eb4:	90461cfa 	lbu	a2,7418(v0)
a1c00eb8:	93a90012 	lbu	t1,18(sp)
a1c00ebc:	00000000 	nop
a1c00ec0:	1126ff74 	beq	t1,a2,a1c00c94 <main+0x448>
a1c00ec4:	2402ff9c 	li	v0,-100
a1c00ec8:	ae180000 	sw	t8,0(s0)
a1c00ecc:	262400b0 	addiu	a0,s1,176
a1c00ed0:	74780140 	jalx	a1e00500 <mon_Event>
a1c00ed4:	00000000 	nop
a1c00ed8:	08700268 	j	a1c009a0 <main+0x154>
a1c00edc:	00000000 	nop
a1c00ee0:	10800019 	beqz	a0,a1c00f48 <main+0x6fc>
a1c00ee4:	0008c8c0 	sll	t9,t0,0x3
a1c00ee8:	27aa0038 	addiu	t2,sp,56
a1c00eec:	8c640000 	lw	a0,0(v1)
a1c00ef0:	0159f821 	addu	ra,t2,t9
a1c00ef4:	27e70004 	addiu	a3,ra,4
a1c00ef8:	24050004 	li	a1,4
a1c00efc:	27a60014 	addiu	a2,sp,20
a1c00f00:	afa30064 	sw	v1,100(sp)
a1c00f04:	74780c51 	jalx	a1e03144 <memd_FlashWrite>
a1c00f08:	afa80068 	sw	t0,104(sp)
a1c00f0c:	8fa30064 	lw	v1,100(sp)
a1c00f10:	1440001e 	bnez	v0,a1c00f8c <main+0x740>
a1c00f14:	00000000 	nop
a1c00f18:	8c640000 	lw	a0,0(v1)
a1c00f1c:	74700468 	jalx	a1c011a0 <memd_FlashGetGlobalAddress>
a1c00f20:	00000000 	nop
a1c00f24:	3c08a000 	lui	t0,0xa000
a1c00f28:	00481825 	or	v1,v0,t0
a1c00f2c:	8c640000 	lw	a0,0(v1)
a1c00f30:	8fa30064 	lw	v1,100(sp)
a1c00f34:	8fa80068 	lw	t0,104(sp)
a1c00f38:	8c670004 	lw	a3,4(v1)
a1c00f3c:	00000000 	nop
a1c00f40:	14870012 	bne	a0,a3,a1c00f8c <main+0x740>
a1c00f44:	2402ff81 	li	v0,-127
a1c00f48:	25080001 	addiu	t0,t0,1
a1c00f4c:	24630008 	addiu	v1,v1,8
a1c00f50:	8fa5004c 	lw	a1,76(sp)
a1c00f54:	24090001 	li	t1,1
a1c00f58:	01093004 	sllv	a2,t1,t0
a1c00f5c:	01051006 	srlv	v0,a1,t0
a1c00f60:	1440ffdf 	bnez	v0,a1c00ee0 <main+0x694>
a1c00f64:	00c52024 	and	a0,a2,a1
a1c00f68:	08700412 	j	a1c01048 <main+0x7fc>
a1c00f6c:	24022021 	li	v0,8225
a1c00f70:	2418ff9c 	li	t8,-100
a1c00f74:	262400f0 	addiu	a0,s1,240
a1c00f78:	ae180000 	sw	t8,0(s0)
a1c00f7c:	74780140 	jalx	a1e00500 <mon_Event>
a1c00f80:	3a310001 	xori	s1,s1,0x1
a1c00f84:	08700268 	j	a1c009a0 <main+0x154>
a1c00f88:	afa0004c 	sw	zero,76(sp)
a1c00f8c:	afa20048 	sw	v0,72(sp)
a1c00f90:	241fff9d 	li	ra,-99
a1c00f94:	ae1f0000 	sw	ra,0(s0)
a1c00f98:	74780140 	jalx	a1e00500 <mon_Event>
a1c00f9c:	262400e0 	addiu	a0,s1,224
a1c00fa0:	8fb90048 	lw	t9,72(sp)
a1c00fa4:	3c100ef0 	lui	s0,0xef0
a1c00fa8:	02192823 	subu	a1,s0,t9
a1c00fac:	74700142 	jalx	a1c00508 <hal_HstSendEvent>
a1c00fb0:	24040001 	li	a0,1
a1c00fb4:	08700268 	j	a1c009a0 <main+0x154>
a1c00fb8:	afa0004c 	sw	zero,76(sp)
a1c00fbc:	ae0a0000 	sw	t2,0(s0)
a1c00fc0:	74780140 	jalx	a1e00500 <mon_Event>
a1c00fc4:	262400f0 	addiu	a0,s1,240
a1c00fc8:	747006c2 	jalx	a1c01b08 <hal_HstWaitLastEventSent>
a1c00fcc:	00000000 	nop
a1c00fd0:	74700824 	jalx	a1c02090 <hal_SysRestart>
a1c00fd4:	00000000 	nop
a1c00fd8:	08700268 	j	a1c009a0 <main+0x154>
a1c00fdc:	00000000 	nop
a1c00fe0:	262400f0 	addiu	a0,s1,240
a1c00fe4:	74780140 	jalx	a1e00500 <mon_Event>
a1c00fe8:	ae070000 	sw	a3,0(s0)
a1c00fec:	24040001 	li	a0,1
a1c00ff0:	3c050eee 	lui	a1,0xeee
a1c00ff4:	74700142 	jalx	a1c00508 <hal_HstSendEvent>
a1c00ff8:	00000000 	nop
a1c00ffc:	08700268 	j	a1c009a0 <main+0x154>
a1c01000:	00000000 	nop
a1c01004:	8e040004 	lw	a0,4(s0)
a1c01008:	8e05000c 	lw	a1,12(s0)
a1c0100c:	8e070008 	lw	a3,8(s0)
a1c01010:	74780c51 	jalx	a1e03144 <memd_FlashWrite>
a1c01014:	27a60014 	addiu	a2,sp,20
a1c01018:	8fa30014 	lw	v1,20(sp)
a1c0101c:	afa20048 	sw	v0,72(sp)
a1c01020:	ae03000c 	sw	v1,12(s0)
a1c01024:	8fa40048 	lw	a0,72(sp)
a1c01028:	00000000 	nop
a1c0102c:	1480fecf 	bnez	a0,a1c00b6c <main+0x320>
a1c01030:	2402ff9d 	li	v0,-99
a1c01034:	27a2003c 	addiu	v0,sp,60
a1c01038:	00001821 	move	v1,zero
a1c0103c:	8fa5004c 	lw	a1,76(sp)
a1c01040:	087002e6 	j	a1c00b98 <main+0x34c>
a1c01044:	24040001 	li	a0,1
a1c01048:	afa20028 	sw	v0,40(sp)
a1c0104c:	8e08000c 	lw	t0,12(s0)
a1c01050:	24040004 	li	a0,4
a1c01054:	24050004 	li	a1,4
a1c01058:	27a60014 	addiu	a2,sp,20
a1c0105c:	27a70028 	addiu	a3,sp,40
a1c01060:	74780c51 	jalx	a1e03144 <memd_FlashWrite>
a1c01064:	afa80028 	sw	t0,40(sp)
a1c01068:	1040ffc1 	beqz	v0,a1c00f70 <main+0x724>
a1c0106c:	afa20048 	sw	v0,72(sp)
a1c01070:	087003e5 	j	a1c00f94 <main+0x748>
a1c01074:	241fff9d 	li	ra,-99

a1c01078 <hal_DbgAssert>:
a1c01078:	d501      	sw	a1,4(sp)
a1c0107a:	d602      	sw	a2,8(sp)
a1c0107c:	e820      	jr	ra
a1c0107e:	d703      	sw	a3,12(sp)

a1c01080 <COS_MemInit>:
a1c01080:	e820      	jr	ra
a1c01082:	6500      	nop

a1c01084 <tgt_Open>:
a1c01084:	b302      	lw	v1,a1c0108c <tgt_Open+0x8>
a1c01086:	b203      	lw	v0,a1c01090 <tgt_Open+0xc>
a1c01088:	e820      	jr	ra
a1c0108a:	da60      	sw	v1,0(v0)
a1c0108c:	2564      	beqz	a1,a1c01156 <$LCFI0+0xc0>
a1c0108e:	a1c0      	lbu	a2,0(s1)
a1c01090:	0230      	addiu	v0,sp,192
a1c01092:	a1c0      	lbu	a2,0(s1)

a1c01094 <memd_FlashGetSectorLimits>:
a1c01094:	63ff      	addiu	sp,-8

a1c01096 <$LCFI0>:
a1c01096:	d101      	sw	s1,4(sp)
a1c01098:	d000      	sw	s0,0(sp)
a1c0109a:	b332      	lw	v1,a1c01160 <$LCFI0+0xca>
a1c0109c:	9b42      	lw	v0,8(v1)
a1c0109e:	ec4a      	cmp	a0,v0
a1c010a0:	6105      	btnez	a1c010ac <$LCFI0+0x16>
a1c010a2:	dd80      	sw	a0,0(a1)
a1c010a4:	9b42      	lw	v0,8(v1)
a1c010a6:	de40      	sw	v0,0(a2)
a1c010a8:	6a00      	li	v0,0
a1c010aa:	1055      	b	a1c01156 <$LCFI0+0xc0>
a1c010ac:	ea83      	sltu	v0,a0
a1c010ae:	6106      	btnez	a1c010bc <$LCFI0+0x26>
a1c010b0:	9b01      	lw	s0,4(v1)
a1c010b2:	a340      	lbu	v0,0(v1)
a1c010b4:	67f0      	move	a3,s0
a1c010b6:	4aff      	addiu	v0,-1
a1c010b8:	6b00      	li	v1,0
a1c010ba:	1006      	b	a1c010c8 <$LCFI0+0x32>
a1c010bc:	6a00      	li	v0,0
a1c010be:	dd40      	sw	v0,0(a1)
a1c010c0:	de40      	sw	v0,0(a2)
a1c010c2:	e845      	break	2
a1c010c4:	1048      	b	a1c01156 <$LCFI0+0xc0>
a1c010c6:	4b01      	addiu	v1,1
a1c010c8:	9f27      	lw	s1,28(a3)
a1c010ca:	ec23      	sltu	a0,s1
a1c010cc:	6103      	btnez	a1c010d4 <$LCFI0+0x3e>
a1c010ce:	eb42      	slt	v1,v0
a1c010d0:	4f1c      	addiu	a3,28
a1c010d2:	61f9      	btnez	a1c010c6 <$LCFI0+0x30>
a1c010d4:	6a1c      	li	v0,28
a1c010d6:	eb58      	mult	v1,v0
a1c010d8:	ef12      	mflo	a3
a1c010da:	e0e9      	addu	v0,s0,a3
a1c010dc:	9a22      	lw	s1,8(v0)
a1c010de:	9a61      	lw	v1,4(v0)
a1c010e0:	9ae0      	lw	a3,0(v0)
a1c010e2:	e978      	mult	s1,v1
a1c010e4:	e812      	mflo	s0
a1c010e6:	e0e1      	addu	s0,a3
a1c010e8:	ec03      	sltu	a0,s0
a1c010ea:	6003      	bteqz	a1c010f2 <$LCFI0+0x5c>
a1c010ec:	6767      	move	v1,a3
a1c010ee:	6800      	li	s0,0
a1c010f0:	1019      	b	a1c01124 <$LCFI0+0x8e>
a1c010f2:	9ae4      	lw	a3,16(v0)
a1c010f4:	9a23      	lw	s1,12(v0)
a1c010f6:	ef38      	mult	a3,s1
a1c010f8:	eb12      	mflo	v1
a1c010fa:	e06d      	addu	v1,s0,v1
a1c010fc:	ec63      	sltu	a0,v1
a1c010fe:	6004      	bteqz	a1c01108 <$LCFI0+0x72>
a1c01100:	6770      	move	v1,s0
a1c01102:	67f0      	move	a3,s0
a1c01104:	6801      	li	s0,1
a1c01106:	100e      	b	a1c01124 <$LCFI0+0x8e>
a1c01108:	9a06      	lw	s0,24(v0)
a1c0110a:	9ae5      	lw	a3,20(v0)
a1c0110c:	e8f8      	mult	s0,a3
a1c0110e:	e912      	mflo	s1
a1c01110:	e321      	addu	s0,v1,s1
a1c01112:	ec03      	sltu	a0,s0
a1c01114:	6105      	btnez	a1c01120 <$LCFI0+0x8a>
a1c01116:	6a00      	li	v0,0
a1c01118:	dd40      	sw	v0,0(a1)
a1c0111a:	de40      	sw	v0,0(a2)
a1c0111c:	e845      	break	2
a1c0111e:	101b      	b	a1c01156 <$LCFI0+0xc0>
a1c01120:	67e3      	move	a3,v1
a1c01122:	6802      	li	s0,2
a1c01124:	310c      	sll	s1,s0,3
a1c01126:	e229      	addu	v0,s1
a1c01128:	9a42      	lw	v0,8(v0)
a1c0112a:	1001      	b	a1c0112e <$LCFI0+0x98>
a1c0112c:	6767      	move	v1,a3
a1c0112e:	e75d      	addu	a3,v0
a1c01130:	ece3      	sltu	a0,a3
a1c01132:	60fc      	bteqz	a1c0112c <$LCFI0+0x96>
a1c01134:	f7ff 681f 	li	s0,65535
a1c01138:	e88c      	and	s0,a0
a1c0113a:	200b      	beqz	s0,a1c01152 <$LCFI0+0xbc>
a1c0113c:	f002 6f00 	li	a3,4096
a1c01140:	ebeb      	neg	v1,a3
a1c01142:	eb8c      	and	v1,a0
a1c01144:	dd60      	sw	v1,0(a1)
a1c01146:	f002 4b00 	addiu	v1,4096
a1c0114a:	de60      	sw	v1,0(a2)
a1c0114c:	f002 6a00 	li	v0,4096
a1c01150:	1002      	b	a1c01156 <$LCFI0+0xc0>
a1c01152:	dd60      	sw	v1,0(a1)
a1c01154:	dee0      	sw	a3,0(a2)
a1c01156:	9101      	lw	s1,4(sp)
a1c01158:	9000      	lw	s0,0(sp)
a1c0115a:	e820      	jr	ra
a1c0115c:	6301      	addiu	sp,8
a1c0115e:	6500      	nop
a1c01160:	0158      	addiu	s1,sp,352
a1c01162:	a1c0      	lbu	a2,0(s1)

a1c01164 <memd_FlashGetUserFieldInfo>:
a1c01164:	63fa      	addiu	sp,-48

a1c01166 <$LCFI3>:
a1c01166:	d10a      	sw	s1,40(sp)
a1c01168:	d009      	sw	s0,36(sp)
a1c0116a:	620b      	sw	ra,44(sp)
a1c0116c:	b10b      	lw	s1,a1c01198 <$LCFI3+0x32>
a1c0116e:	6706      	move	s0,a2
a1c01170:	99c3      	lw	a2,12(s1)
a1c01172:	dcc0      	sw	a2,0(a0)
a1c01174:	9964      	lw	v1,16(s1)
a1c01176:	0605      	addiu	a2,sp,20
a1c01178:	dd60      	sw	v1,0(a1)
a1c0117a:	9c80      	lw	a0,0(a0)
a1c0117c:	0504      	addiu	a1,sp,16
a1c0117e:	1a03 0425 	jal	a1c01094 <memd_FlashGetSectorLimits>
a1c01182:	d706      	sw	a3,24(sp)
a1c01184:	d840      	sw	v0,0(s0)
a1c01186:	9706      	lw	a3,24(sp)
a1c01188:	9945      	lw	v0,20(s1)
a1c0118a:	df40      	sw	v0,0(a3)
a1c0118c:	970b      	lw	a3,44(sp)
a1c0118e:	910a      	lw	s1,40(sp)
a1c01190:	9009      	lw	s0,36(sp)
a1c01192:	ef00      	jr	a3
a1c01194:	6306      	addiu	sp,48
a1c01196:	6500      	nop
a1c01198:	0158      	addiu	s1,sp,352
a1c0119a:	a1c0      	lbu	a2,0(s1)

a1c0119c <memd_FlashRst>:
a1c0119c:	e820      	jr	ra
a1c0119e:	6500      	nop

a1c011a0 <memd_FlashGetGlobalAddress>:
a1c011a0:	b212      	lw	v0,a1c011e8 <$LCFI15+0x44>
a1c011a2:	63fd      	addiu	sp,-24

a1c011a4 <$LCFI15>:
a1c011a4:	ea83      	sltu	v0,a0
a1c011a6:	6205      	sw	ra,20(sp)
a1c011a8:	d004      	sw	s0,16(sp)
a1c011aa:	6001      	bteqz	a1c011ae <$LCFI15+0xa>
a1c011ac:	e845      	break	2
a1c011ae:	b310      	lw	v1,a1c011ec <$LCFI15+0x48>
a1c011b0:	eb8c      	and	v1,a0
a1c011b2:	2301      	beqz	v1,a1c011b6 <$LCFI15+0x12>
a1c011b4:	e845      	break	2
a1c011b6:	f500 3382 	srl	v1,a0,20
a1c011ba:	6d07      	li	a1,7
a1c011bc:	ebac      	and	v1,a1
a1c011be:	5303      	slti	v1,3
a1c011c0:	b20c      	lw	v0,a1c011f0 <$LCFI15+0x4c>
a1c011c2:	6003      	bteqz	a1c011ca <$LCFI15+0x26>
a1c011c4:	9a00      	lw	s0,0(v0)
a1c011c6:	e401      	addu	s0,a0,s0
a1c011c8:	1009      	b	a1c011dc <$LCFI15+0x38>
a1c011ca:	9aa0      	lw	a1,0(v0)
a1c011cc:	6a03      	li	v0,3
a1c011ce:	e26f      	subu	v1,v0,v1
a1c011d0:	e4b9      	addu	a2,a0,a1
a1c011d2:	f580 3760 	sll	a3,v1,22
a1c011d6:	1a03 0680 	jal	a1c01a00 <hal_EbcConfigRemap>
a1c011da:	e6e1      	addu	s0,a2,a3
a1c011dc:	9705      	lw	a3,20(sp)
a1c011de:	6750      	move	v0,s0
a1c011e0:	9004      	lw	s0,16(sp)
a1c011e2:	ef00      	jr	a3
a1c011e4:	6303      	addiu	sp,24
a1c011e6:	6500      	nop
a1c011e8:	ffff      	daddiu	a3,sp,124
a1c011ea:	000f      	addiu	s0,sp,60
a1c011ec:	0000      	addiu	s0,sp,0
a1c011ee:	e000      	daddu	s0,s0
a1c011f0:	013c      	addiu	s1,sp,240
a1c011f2:	a1c0      	lbu	a2,0(s1)

a1c011f4 <get_flash_ID>:
a1c011f4:	63fc      	addiu	sp,-32

a1c011f6 <$LCFI30>:
a1c011f6:	d006      	sw	s0,24(sp)
a1c011f8:	6704      	move	s0,a0
a1c011fa:	6c01      	li	a0,1
a1c011fc:	6207      	sw	ra,28(sp)
a1c011fe:	1b03 0d51 	jal	a1e03544 <hal_SpiFlashClearFifoData>
a1c01202:	67a4      	move	a1,a0
a1c01204:	67b0      	move	a1,s0
a1c01206:	6e00      	li	a2,0
a1c01208:	6f03      	li	a3,3
a1c0120a:	1b03 0d5b 	jal	a1e0356c <hal_SpiFlashSendCmd>
a1c0120e:	6c9f      	li	a0,159
a1c01210:	0404      	addiu	a0,sp,16
a1c01212:	1b03 0d45 	jal	a1e03514 <hal_SpiFlashGetRxData>
a1c01216:	6d03      	li	a1,3
a1c01218:	679d      	move	a0,sp
a1c0121a:	a4d0      	lbu	a2,16(a0)
a1c0121c:	a4b1      	lbu	a1,17(a0)
a1c0121e:	a472      	lbu	v1,18(a0)
a1c01220:	b204      	lw	v0,a1c01230 <$LCFI30+0x3a>
a1c01222:	dac0      	sw	a2,0(v0)
a1c01224:	daa1      	sw	a1,4(v0)
a1c01226:	da62      	sw	v1,8(v0)
a1c01228:	9707      	lw	a3,28(sp)
a1c0122a:	9006      	lw	s0,24(sp)
a1c0122c:	ef00      	jr	a3
a1c0122e:	6304      	addiu	sp,32
a1c01230:	8a6c      	lh	v1,24(v0)
a1c01232:	a1c0      	lbu	a2,0(s1)

a1c01234 <write_flash_status>:
a1c01234:	63fa      	addiu	sp,-48

a1c01236 <$LCFI91>:
a1c01236:	d009      	sw	s0,36(sp)
a1c01238:	620b      	sw	ra,44(sp)
a1c0123a:	d10a      	sw	s1,40(sp)
a1c0123c:	b31d      	lw	v1,a1c012b0 <$LCFI91+0x7a>
a1c0123e:	9b60      	lw	v1,0(v1)
a1c01240:	6aff      	li	v0,255
a1c01242:	73c2      	cmpi	v1,194
a1c01244:	6704      	move	s0,a0
a1c01246:	eaac      	and	v0,a1
a1c01248:	6002      	bteqz	a1c0124e <$LCFI91+0x18>
a1c0124a:	738c      	cmpi	v1,140
a1c0124c:	6106      	btnez	a1c0125a <$LCFI91+0x24>
a1c0124e:	679d      	move	a0,sp
a1c01250:	c450      	sb	v0,16(a0)
a1c01252:	6a00      	li	v0,0
a1c01254:	c451      	sb	v0,17(a0)
a1c01256:	6b01      	li	v1,1
a1c01258:	1005      	b	a1c01264 <$LCFI91+0x2e>
a1c0125a:	67bd      	move	a1,sp
a1c0125c:	6e00      	li	a2,0
a1c0125e:	c5d0      	sb	a2,16(a1)
a1c01260:	c551      	sb	v0,17(a1)
a1c01262:	6b02      	li	v1,2
a1c01264:	b414      	lw	a0,a1c012b4 <$LCFI91+0x7e>
a1c01266:	9c20      	lw	s1,0(a0)
a1c01268:	6c01      	li	a0,1
a1c0126a:	67a4      	move	a1,a0
a1c0126c:	6a02      	li	v0,2
a1c0126e:	e94c      	and	s1,v0
a1c01270:	1b03 0d51 	jal	a1e03544 <hal_SpiFlashClearFifoData>
a1c01274:	d306      	sw	v1,24(sp)
a1c01276:	6e00      	li	a2,0
a1c01278:	67e6      	move	a3,a2
a1c0127a:	6c06      	li	a0,6
a1c0127c:	1b03 0d5b 	jal	a1e0356c <hal_SpiFlashSendCmd>
a1c01280:	67b0      	move	a1,s0
a1c01282:	ef2b      	neg	a3,s1
a1c01284:	9106      	lw	s1,24(sp)
a1c01286:	f7c0 36e2 	srl	a2,a3,31
a1c0128a:	0404      	addiu	a0,sp,16
a1c0128c:	1b03 0d33 	jal	a1e034cc <hal_SpiFlashPushTxFifoData>
a1c01290:	67b1      	move	a1,s1
a1c01292:	6e00      	li	a2,0
a1c01294:	6c01      	li	a0,1
a1c01296:	67b0      	move	a1,s0
a1c01298:	1b03 0d5b 	jal	a1e0356c <hal_SpiFlashSendCmd>
a1c0129c:	67e6      	move	a3,a2
a1c0129e:	1b03 0c0d 	jal	a1e03034 <memd_FlashWriteInProgress>
a1c012a2:	6790      	move	a0,s0
a1c012a4:	2afc      	bnez	v0,a1c0129e <$LCFI91+0x68>
a1c012a6:	970b      	lw	a3,44(sp)
a1c012a8:	910a      	lw	s1,40(sp)
a1c012aa:	9009      	lw	s0,36(sp)
a1c012ac:	ef00      	jr	a3
a1c012ae:	6306      	addiu	sp,48
a1c012b0:	8a6c      	lh	v1,24(v0)
a1c012b2:	a1c0      	lbu	a2,0(s1)
a1c012b4:	88a4      	lh	a1,8(s0)
a1c012b6:	a1c0      	lbu	a2,0(s1)

a1c012b8 <spi_flash_quard_ctrl>:
a1c012b8:	63fb      	addiu	sp,-40

a1c012ba <$LCFI100>:
a1c012ba:	6bff      	li	v1,255
a1c012bc:	d007      	sw	s0,28(sp)
a1c012be:	6704      	move	s0,a0
a1c012c0:	e86c      	and	s0,v1
a1c012c2:	6209      	sw	ra,36(sp)
a1c012c4:	d108      	sw	s1,32(sp)
a1c012c6:	ebac      	and	v1,a1
a1c012c8:	6a00      	li	v0,0
a1c012ca:	2008      	beqz	s0,a1c012dc <$LCFI100+0x22>
a1c012cc:	b21b      	lw	v0,a1c01338 <$LCFI100+0x7e>
a1c012ce:	9a40      	lw	v0,0(v0)
a1c012d0:	72c2      	cmpi	v0,194
a1c012d2:	6003      	bteqz	a1c012da <$LCFI100+0x20>
a1c012d4:	728c      	cmpi	v0,140
a1c012d6:	6a02      	li	v0,2
a1c012d8:	6101      	btnez	a1c012dc <$LCFI100+0x22>
a1c012da:	6a40      	li	v0,64
a1c012dc:	7301      	cmpi	v1,1
a1c012de:	6102      	btnez	a1c012e4 <$LCFI100+0x2a>
a1c012e0:	b117      	lw	s1,a1c0133c <$LCFI100+0x82>
a1c012e2:	1004      	b	a1c012ec <$LCFI100+0x32>
a1c012e4:	7303      	cmpi	v1,3
a1c012e6:	6900      	li	s1,0
a1c012e8:	6101      	btnez	a1c012ec <$LCFI100+0x32>
a1c012ea:	b116      	lw	s1,a1c01340 <$LCFI100+0x86>
a1c012ec:	b513      	lw	a1,a1c01338 <$LCFI100+0x7e>
a1c012ee:	9d80      	lw	a0,0(a1)
a1c012f0:	741c      	cmpi	a0,28
a1c012f2:	600d      	bteqz	a1c0130e <$LCFI100+0x54>
a1c012f4:	67a2      	move	a1,v0
a1c012f6:	6c00      	li	a0,0
a1c012f8:	d204      	sw	v0,16(sp)
a1c012fa:	1a03 048d 	jal	a1c01234 <write_flash_status>
a1c012fe:	d305      	sw	v1,20(sp)
a1c01300:	9305      	lw	v1,20(sp)
a1c01302:	9204      	lw	v0,16(sp)
a1c01304:	2304      	beqz	v1,a1c0130e <$LCFI100+0x54>
a1c01306:	6791      	move	a0,s1
a1c01308:	1a03 048d 	jal	a1c01234 <write_flash_status>
a1c0130c:	67a2      	move	a1,v0
a1c0130e:	b60e      	lw	a2,a1c01344 <$LCFI100+0x8a>
a1c01310:	a680      	lbu	a0,0(a2)
a1c01312:	6e00      	li	a2,0
a1c01314:	67b1      	move	a1,s1
a1c01316:	1b03 0d5b 	jal	a1e0356c <hal_SpiFlashSendCmd>
a1c0131a:	67e6      	move	a3,a2
a1c0131c:	2003      	beqz	s0,a1c01324 <$LCFI100+0x6a>
a1c0131e:	6802      	li	s0,2
a1c01320:	b70a      	lw	a3,a1c01348 <$LCFI100+0x8e>
a1c01322:	df01      	sw	s0,4(a3)
a1c01324:	b409      	lw	a0,a1c01348 <$LCFI100+0x8e>
a1c01326:	1a03 06f1 	jal	a1c01bc4 <hal_SpiFlashInit>
a1c0132a:	6500      	nop
a1c0132c:	9709      	lw	a3,36(sp)
a1c0132e:	6a01      	li	v0,1
a1c01330:	9108      	lw	s1,32(sp)
a1c01332:	9007      	lw	s0,28(sp)
a1c01334:	ef00      	jr	a3
a1c01336:	6305      	addiu	sp,40
a1c01338:	8a6c      	lh	v1,24(v0)
a1c0133a:	a1c0      	lbu	a2,0(s1)
a1c0133c:	0000      	addiu	s0,sp,0
a1c0133e:	0040      	addiu	s0,sp,256
a1c01340:	0000      	addiu	s0,sp,0
a1c01342:	0080      	addiu	s0,sp,512
a1c01344:	024c      	addiu	v0,sp,304
a1c01346:	a1c0      	lbu	a2,0(s1)
a1c01348:	88a4      	lh	a1,8(s0)
a1c0134a:	a1c0      	lbu	a2,0(s1)

a1c0134c <memd_FlashOpen>:
a1c0134c:	63fc      	addiu	sp,-32

a1c0134e <$LCFI104>:
a1c0134e:	d005      	sw	s0,20(sp)
a1c01350:	6207      	sw	ra,28(sp)
a1c01352:	d106      	sw	s1,24(sp)
a1c01354:	6704      	move	s0,a0
a1c01356:	2c01      	bnez	a0,a1c0135a <$LCFI104+0xc>
a1c01358:	e845      	break	2
a1c0135a:	1a03 052d 	jal	a1c014b4 <memd_FlashSetLayout>
a1c0135e:	6500      	nop
a1c01360:	6e10      	li	a2,16
a1c01362:	b445      	lw	a0,a1c01474 <$LCFI104+0x126>
a1c01364:	1a03 0539 	jal	a1c014e4 <memset>
a1c01368:	6d00      	li	a1,0
a1c0136a:	b143      	lw	s1,a1c01474 <$LCFI104+0x126>
a1c0136c:	6c01      	li	a0,1
a1c0136e:	c189      	sb	a0,9(s1)
a1c01370:	c188      	sb	a0,8(s1)
a1c01372:	b442      	lw	a0,a1c01478 <$LCFI104+0x12a>
a1c01374:	1a03 0772 	jal	a1c01dc8 <hal_SysSetSpiFlashClock>
a1c01378:	6500      	nop
a1c0137a:	1a03 06f1 	jal	a1c01bc4 <hal_SpiFlashInit>
a1c0137e:	6791      	move	a0,s1
a1c01380:	1a03 047d 	jal	a1c011f4 <get_flash_ID>
a1c01384:	6c00      	li	a0,0
a1c01386:	6c00      	li	a0,0
a1c01388:	6a01      	li	v0,1
a1c0138a:	67a4      	move	a1,a0
a1c0138c:	1a03 04ae 	jal	a1c012b8 <spi_flash_quard_ctrl>
a1c01390:	d941      	sw	v0,4(s1)
a1c01392:	b43b      	lw	a0,a1c0147c <$LCFI104+0x12e>
a1c01394:	6e00      	li	a2,0
a1c01396:	1a03 0683 	jal	a1c01a0c <hal_EbcFlashOpen>
a1c0139a:	67b0      	move	a1,s0
a1c0139c:	b739      	lw	a3,a1c01480 <$LCFI104+0x132>
a1c0139e:	b53a      	lw	a1,a1c01484 <$LCFI104+0x136>
a1c013a0:	df40      	sw	v0,0(a3)
a1c013a2:	b03a      	lw	s0,a1c01488 <$LCFI104+0x13a>
a1c013a4:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c013a8:	6cff      	li	a0,255
a1c013aa:	98a0      	lw	a1,0(s0)
a1c013ac:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c013b0:	6cff      	li	a0,255
a1c013b2:	98a1      	lw	a1,4(s0)
a1c013b4:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c013b8:	6cff      	li	a0,255
a1c013ba:	98a2      	lw	a1,8(s0)
a1c013bc:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c013c0:	6cff      	li	a0,255
a1c013c2:	9840      	lw	v0,0(s0)
a1c013c4:	b632      	lw	a2,a1c0148c <$LCFI104+0x13e>
a1c013c6:	b533      	lw	a1,a1c01490 <$LCFI104+0x142>
a1c013c8:	b333      	lw	v1,a1c01494 <$LCFI104+0x146>
a1c013ca:	b434      	lw	a0,a1c01498 <$LCFI104+0x14a>
a1c013cc:	72c2      	cmpi	v0,194
a1c013ce:	652e      	move	t1,a2
a1c013d0:	650d      	move	t0,a1
a1c013d2:	b133      	lw	s1,a1c0149c <$LCFI104+0x14e>
a1c013d4:	b733      	lw	a3,a1c014a0 <$LCFI104+0x152>
a1c013d6:	b634      	lw	a2,a1c014a4 <$LCFI104+0x156>
a1c013d8:	b534      	lw	a1,a1c014a8 <$LCFI104+0x15a>
a1c013da:	653b      	move	t9,v1
a1c013dc:	65fc      	move	ra,a0
a1c013de:	6119      	btnez	a1c01412 <$LCFI104+0xc4>
a1c013e0:	6a80      	li	v0,128
a1c013e2:	6709      	move	s0,t1
a1c013e4:	d840      	sw	v0,0(s0)
a1c013e6:	6a50      	li	v0,80
a1c013e8:	e84b      	neg	s0,v0
a1c013ea:	c100      	sb	s0,0(s1)
a1c013ec:	690b      	li	s1,11
a1c013ee:	6778      	move	v1,t8
a1c013f0:	6788      	move	a0,t0
a1c013f2:	ea2b      	neg	v0,s1
a1c013f4:	c460      	sb	v1,0(a0)
a1c013f6:	c540      	sb	v0,0(a1)
a1c013f8:	6b30      	li	v1,48
a1c013fa:	6c35      	li	a0,53
a1c013fc:	6d67      	li	a1,103
a1c013fe:	c760      	sb	v1,0(a3)
a1c01400:	c680      	sb	a0,0(a2)
a1c01402:	ebab      	neg	v1,a1
a1c01404:	6e66      	li	a2,102
a1c01406:	67f9      	move	a3,t9
a1c01408:	671f      	move	s0,ra
a1c0140a:	c060      	sb	v1,0(s0)
a1c0140c:	c7c0      	sb	a2,0(a3)
a1c0140e:	6b38      	li	v1,56
a1c01410:	1023      	b	a1c01458 <$LCFI104+0x10a>
a1c01412:	6bff      	li	v1,255
a1c01414:	4b01      	addiu	v1,1
a1c01416:	6789      	move	a0,t1
a1c01418:	dc60      	sw	v1,0(a0)
a1c0141a:	6b00      	li	v1,0
a1c0141c:	6788      	move	a0,t0
a1c0141e:	c460      	sb	v1,0(a0)
a1c01420:	6c75      	li	a0,117
a1c01422:	c180      	sb	a0,0(s1)
a1c01424:	697a      	li	s1,122
a1c01426:	c720      	sb	s1,0(a3)
a1c01428:	6f38      	li	a3,56
a1c0142a:	c6e0      	sb	a3,0(a2)
a1c0142c:	6e01      	li	a2,1
a1c0142e:	ebcb      	neg	v1,a2
a1c01430:	6c67      	li	a0,103
a1c01432:	c560      	sb	v1,0(a1)
a1c01434:	e98b      	neg	s1,a0
a1c01436:	67f9      	move	a3,t9
a1c01438:	6e66      	li	a2,102
a1c0143a:	67bf      	move	a1,ra
a1c0143c:	72f8      	cmpi	v0,248
a1c0143e:	c720      	sb	s1,0(a3)
a1c01440:	c5c0      	sb	a2,0(a1)
a1c01442:	6103      	btnez	a1c0144a <$LCFI104+0xfc>
a1c01444:	9841      	lw	v0,4(s0)
a1c01446:	7242      	cmpi	v0,66
a1c01448:	1005      	b	a1c01454 <$LCFI104+0x106>
a1c0144a:	721c      	cmpi	v0,28
a1c0144c:	6004      	bteqz	a1c01456 <$LCFI104+0x108>
a1c0144e:	7201      	cmpi	v0,1
a1c01450:	6002      	bteqz	a1c01456 <$LCFI104+0x108>
a1c01452:	72e0      	cmpi	v0,224
a1c01454:	6103      	btnez	a1c0145c <$LCFI104+0x10e>
a1c01456:	6b02      	li	v1,2
a1c01458:	b015      	lw	s0,a1c014ac <$LCFI104+0x15e>
a1c0145a:	c060      	sb	v1,0(s0)
a1c0145c:	b60c      	lw	a2,a1c0148c <$LCFI104+0x13e>
a1c0145e:	9ea0      	lw	a1,0(a2)
a1c01460:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c01464:	6c01      	li	a0,1
a1c01466:	9707      	lw	a3,28(sp)
a1c01468:	b212      	lw	v0,a1c014b0 <$LCFI104+0x162>
a1c0146a:	9106      	lw	s1,24(sp)
a1c0146c:	9005      	lw	s0,20(sp)
a1c0146e:	ef00      	jr	a3
a1c01470:	6304      	addiu	sp,32
a1c01472:	6500      	nop
a1c01474:	88a4      	lh	a1,8(s0)
a1c01476:	a1c0      	lbu	a2,0(s1)
a1c01478:	7500      	cmpi	a1,0
a1c0147a:	0319      	addiu	v1,sp,100
a1c0147c:	17c0      	b	a1c013fe <$LCFI104+0xb0>
a1c0147e:	0253      	addiu	v0,sp,332
a1c01480:	013c      	addiu	s1,sp,240
a1c01482:	a1c0      	lbu	a2,0(s1)
a1c01484:	5666      	slti	a2,102
a1c01486:	5555      	slti	a1,85
a1c01488:	8a6c      	lh	v1,24(v0)
a1c0148a:	a1c0      	lbu	a2,0(s1)
a1c0148c:	0248      	addiu	v0,sp,288
a1c0148e:	a1c0      	lbu	a2,0(s1)
a1c01490:	024c      	addiu	v0,sp,304
a1c01492:	a1c0      	lbu	a2,0(s1)
a1c01494:	278c      	beqz	a3,a1c013ae <$LCFI104+0x60>
a1c01496:	a1c0      	lbu	a2,0(s1)
a1c01498:	278d      	beqz	a3,a1c013b4 <$LCFI104+0x66>
a1c0149a:	a1c0      	lbu	a2,0(s1)
a1c0149c:	2788      	beqz	a3,a1c013ae <$LCFI104+0x60>
a1c0149e:	a1c0      	lbu	a2,0(s1)
a1c014a0:	2789      	beqz	a3,a1c013b4 <$LCFI104+0x66>
a1c014a2:	a1c0      	lbu	a2,0(s1)
a1c014a4:	278a      	beqz	a3,a1c013ba <$LCFI104+0x6c>
a1c014a6:	a1c0      	lbu	a2,0(s1)
a1c014a8:	278b      	beqz	a3,a1c013c0 <$LCFI104+0x72>
a1c014aa:	a1c0      	lbu	a2,0(s1)
a1c014ac:	024d      	addiu	v0,sp,308
a1c014ae:	a1c0      	lbu	a2,0(s1)
a1c014b0:	0158      	addiu	s1,sp,352
a1c014b2:	a1c0      	lbu	a2,0(s1)

a1c014b4 <memd_FlashSetLayout>:
a1c014b4:	b208      	lw	v0,a1c014d4 <memd_FlashSetLayout+0x20>
a1c014b6:	f004 6b00 	li	v1,8192
a1c014ba:	b508      	lw	a1,a1c014d8 <memd_FlashSetLayout+0x24>
a1c014bc:	b408      	lw	a0,a1c014dc <memd_FlashSetLayout+0x28>
a1c014be:	6f01      	li	a3,1
a1c014c0:	b608      	lw	a2,a1c014e0 <memd_FlashSetLayout+0x2c>
a1c014c2:	da64      	sw	v1,16(v0)
a1c014c4:	f21c 4b00 	addiu	v1,-7680
a1c014c8:	c2e0      	sb	a3,0(v0)
a1c014ca:	dac1      	sw	a2,4(v0)
a1c014cc:	daa2      	sw	a1,8(v0)
a1c014ce:	da83      	sw	a0,12(v0)
a1c014d0:	e820      	jr	ra
a1c014d2:	da65      	sw	v1,20(v0)
a1c014d4:	0158      	addiu	s1,sp,352
a1c014d6:	a1c0      	lbu	a2,0(s1)
a1c014d8:	0000      	addiu	s0,sp,0
a1c014da:	0010      	addiu	s0,sp,64
a1c014dc:	c000      	sb	s0,0(s0)
a1c014de:	000f      	addiu	s0,sp,60
a1c014e0:	2588      	beqz	a1,a1c013f2 <$LCFI104+0xa4>
a1c014e2:	a1c0      	lbu	a2,0(s1)

a1c014e4 <memset>:
a1c014e4:	63ff      	addiu	sp,-8

a1c014e6 <$LCFI0>:
a1c014e6:	5e0c      	sltiu	a2,12
a1c014e8:	d101      	sw	s1,4(sp)
a1c014ea:	d000      	sw	s0,0(sp)
a1c014ec:	6744      	move	v0,a0
a1c014ee:	6764      	move	v1,a0
a1c014f0:	6104      	btnez	a1c014fa <$LCFI0+0x14>
a1c014f2:	1005      	b	a1c014fe <$LCFI0+0x18>
a1c014f4:	c3a0      	sb	a1,0(v1)
a1c014f6:	4eff      	addiu	a2,-1
a1c014f8:	4b01      	addiu	v1,1
a1c014fa:	2efc      	bnez	a2,a1c014f4 <$LCFI0+0xe>
a1c014fc:	1026      	b	a1c0154a <$LCFI0+0x64>
a1c014fe:	6cff      	li	a0,255
a1c01500:	ecac      	and	a0,a1
a1c01502:	67a4      	move	a1,a0
a1c01504:	2405      	beqz	a0,a1c01510 <$LCFI0+0x2a>
a1c01506:	3780      	sll	a3,a0,8
a1c01508:	ef8d      	or	a3,a0
a1c0150a:	35e0      	sll	a1,a3,8
a1c0150c:	35a0      	sll	a1,8
a1c0150e:	eded      	or	a1,a3
a1c01510:	6f03      	li	a3,3
a1c01512:	ef4c      	and	a3,v0
a1c01514:	270a      	beqz	a3,a1c0152a <$LCFI0+0x44>
a1c01516:	6b04      	li	v1,4
a1c01518:	e3ff      	subu	a3,v1,a3
a1c0151a:	6702      	move	s0,v0
a1c0151c:	6767      	move	v1,a3
a1c0151e:	4bff      	addiu	v1,-1
a1c01520:	c080      	sb	a0,0(s0)
a1c01522:	4801      	addiu	s0,1
a1c01524:	2bfc      	bnez	v1,a1c0151e <$LCFI0+0x38>
a1c01526:	e2ed      	addu	v1,v0,a3
a1c01528:	e6fb      	subu	a2,a3
a1c0152a:	31ca      	srl	s1,a2,2
a1c0152c:	6703      	move	s0,v1
a1c0152e:	67f1      	move	a3,s1
a1c01530:	4fff      	addiu	a3,-1
a1c01532:	d8a0      	sw	a1,0(s0)
a1c01534:	4804      	addiu	s0,4
a1c01536:	2ffc      	bnez	a3,a1c01530 <$LCFI0+0x4a>
a1c01538:	3128      	sll	s1,2
a1c0153a:	6d03      	li	a1,3
a1c0153c:	e32d      	addu	v1,s1
a1c0153e:	edcc      	and	a1,a2
a1c01540:	2504      	beqz	a1,a1c0154a <$LCFI0+0x64>
a1c01542:	c380      	sb	a0,0(v1)
a1c01544:	4dff      	addiu	a1,-1
a1c01546:	4b01      	addiu	v1,1
a1c01548:	17fb      	b	a1c01540 <$LCFI0+0x5a>
a1c0154a:	9101      	lw	s1,4(sp)
a1c0154c:	9000      	lw	s0,0(sp)
a1c0154e:	e820      	jr	ra
a1c01550:	6301      	addiu	sp,8
a1c01552:	6500      	nop

a1c01554 <pmd_OpalSpiWrite>:
a1c01554:	63fd      	addiu	sp,-24

a1c01556 <$LCFI44>:
a1c01556:	d004      	sw	s0,16(sp)
a1c01558:	f7ff 681f 	li	s0,65535
a1c0155c:	e8ac      	and	s0,a1
a1c0155e:	f1e0 6d1f 	li	a1,511
a1c01562:	ed8c      	and	a1,a0
a1c01564:	33a0      	sll	v1,a1,8
a1c01566:	3260      	sll	v0,v1,8
a1c01568:	6205      	sw	ra,20(sp)
a1c0156a:	e84d      	or	s0,v0
a1c0156c:	1a03 06cf 	jal	a1c01b3c <hal_IspiTxFifoAvail>
a1c01570:	6c00      	li	a0,0
a1c01572:	22fc      	beqz	v0,a1c0156c <$LCFI44+0x16>
a1c01574:	6c00      	li	a0,0
a1c01576:	67b0      	move	a1,s0
a1c01578:	1a03 06d3 	jal	a1c01b4c <hal_IspiSendData>
a1c0157c:	67c4      	move	a2,a0
a1c0157e:	22f6      	beqz	v0,a1c0156c <$LCFI44+0x16>
a1c01580:	1a03 06cb 	jal	a1c01b2c <hal_IspiTxFinished>
a1c01584:	6c00      	li	a0,0
a1c01586:	22fc      	beqz	v0,a1c01580 <$LCFI44+0x2a>
a1c01588:	9705      	lw	a3,20(sp)
a1c0158a:	9004      	lw	s0,16(sp)
a1c0158c:	ef00      	jr	a3
a1c0158e:	6303      	addiu	sp,24

a1c01590 <pmd_RDAWrite>:
a1c01590:	f7ff 6a1f 	li	v0,65535
a1c01594:	63fd      	addiu	sp,-24

a1c01596 <$LCFI61>:
a1c01596:	6205      	sw	ra,20(sp)
a1c01598:	1a03 0555 	jal	a1c01554 <pmd_OpalSpiWrite>
a1c0159c:	ed4c      	and	a1,v0
a1c0159e:	9705      	lw	a3,20(sp)
a1c015a0:	ef00      	jr	a3
a1c015a2:	6303      	addiu	sp,24

a1c015a4 <pmd_SetCoreVoltage>:
a1c015a4:	63fb      	addiu	sp,-40

a1c015a6 <$LCFI91>:
a1c015a6:	d108      	sw	s1,32(sp)
a1c015a8:	b11e      	lw	s1,a1c01620 <$LCFI91+0x7a>
a1c015aa:	e191      	addu	a0,s1,a0
a1c015ac:	d007      	sw	s0,28(sp)
a1c015ae:	6209      	sw	ra,36(sp)
a1c015b0:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c015b4:	c4a0      	sb	a1,0(a0)
a1c015b6:	a102      	lbu	s0,2(s1)
a1c015b8:	a181      	lbu	a0,1(s1)
a1c015ba:	a160      	lbu	v1,0(s1)
a1c015bc:	e883      	sltu	s0,a0
a1c015be:	6001      	bteqz	a1c015c2 <$LCFI91+0x1c>
a1c015c0:	6704      	move	s0,a0
a1c015c2:	b518      	lw	a1,a1c01620 <$LCFI91+0x7a>
a1c015c4:	a583      	lbu	a0,3(a1)
a1c015c6:	e883      	sltu	s0,a0
a1c015c8:	6001      	bteqz	a1c015cc <$LCFI91+0x26>
a1c015ca:	6704      	move	s0,a0
a1c015cc:	b615      	lw	a2,a1c01620 <$LCFI91+0x7a>
a1c015ce:	a684      	lbu	a0,4(a2)
a1c015d0:	e883      	sltu	s0,a0
a1c015d2:	6001      	bteqz	a1c015d6 <$LCFI91+0x30>
a1c015d4:	6704      	move	s0,a0
a1c015d6:	e863      	sltu	s0,v1
a1c015d8:	6001      	bteqz	a1c015dc <$LCFI91+0x36>
a1c015da:	6703      	move	s0,v1
a1c015dc:	b112      	lw	s1,a1c01624 <$LCFI91+0x7e>
a1c015de:	a160      	lbu	v1,0(s1)
a1c015e0:	eb0e      	xor	v1,s0
a1c015e2:	2315      	beqz	v1,a1c0160e <$LCFI91+0x68>
a1c015e4:	b311      	lw	v1,a1c01628 <$LCFI91+0x82>
a1c015e6:	e311      	addu	a0,v1,s0
a1c015e8:	a4e0      	lbu	a3,0(a0)
a1c015ea:	f7e1 6b1f 	li	v1,4095
a1c015ee:	35e0      	sll	a1,a3,8
a1c015f0:	b70f      	lw	a3,a1c0162c <$LCFI91+0x86>
a1c015f2:	afc0      	lhu	a2,0(a3)
a1c015f4:	35b0      	sll	a1,4
a1c015f6:	ebcc      	and	v1,a2
a1c015f8:	ed6d      	or	a1,v1
a1c015fa:	f7ff 6c1f 	li	a0,65535
a1c015fe:	ed8c      	and	a1,a0
a1c01600:	cfa0      	sh	a1,0(a3)
a1c01602:	6c2f      	li	a0,47
a1c01604:	1a03 0564 	jal	a1c01590 <pmd_RDAWrite>
a1c01608:	d204      	sw	v0,16(sp)
a1c0160a:	c100      	sb	s0,0(s1)
a1c0160c:	9204      	lw	v0,16(sp)
a1c0160e:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01612:	6782      	move	a0,v0
a1c01614:	9709      	lw	a3,36(sp)
a1c01616:	9108      	lw	s1,32(sp)
a1c01618:	9007      	lw	s0,28(sp)
a1c0161a:	ef00      	jr	a3
a1c0161c:	6305      	addiu	sp,40
a1c0161e:	6500      	nop
a1c01620:	88bc      	lh	a1,56(s0)
a1c01622:	a1c0      	lbu	a2,0(s1)
a1c01624:	88c4      	lh	a2,8(s0)
a1c01626:	a1c0      	lbu	a2,0(s1)
a1c01628:	88b8      	lh	a1,48(s0)
a1c0162a:	a1c0      	lbu	a2,0(s1)
a1c0162c:	2510      	beqz	a1,a1c0164e <$LCFI106+0x1a>
a1c0162e:	a1c0      	lbu	a2,0(s1)

a1c01630 <pmd_EnableOpalLdo>:
a1c01630:	6aff      	li	v0,255
a1c01632:	63f9      	addiu	sp,-56

a1c01634 <$LCFI106>:
a1c01634:	eaac      	and	v0,a1
a1c01636:	5e02      	sltiu	a2,2
a1c01638:	d10c      	sw	s1,48(sp)
a1c0163a:	d00b      	sw	s0,44(sp)
a1c0163c:	620d      	sw	ra,52(sp)
a1c0163e:	6704      	move	s0,a0
a1c01640:	6726      	move	s1,a2
a1c01642:	d711      	sw	a3,68(sp)
a1c01644:	d204      	sw	v0,16(sp)
a1c01646:	6104      	btnez	a1c01650 <$LCFI106+0x1c>
a1c01648:	b44e      	lw	a0,a1c01780 <$LCFI106+0x14c>
a1c0164a:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c0164e:	6500      	nop
a1c01650:	3402      	srl	a0,s0,8
a1c01652:	3382      	srl	v1,a0,8
a1c01654:	f002 7305 	cmpi	v1,4101
a1c01658:	6004      	bteqz	a1c01662 <$LCFI106+0x2e>
a1c0165a:	b44a      	lw	a0,a1c01780 <$LCFI106+0x14c>
a1c0165c:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c01660:	6500      	nop
a1c01662:	f7ff 6a1f 	li	v0,65535
a1c01666:	ea0c      	and	v0,s0
a1c01668:	f004 5a00 	sltiu	v0,8192
a1c0166c:	6105      	btnez	a1c01678 <$LCFI106+0x44>
a1c0166e:	b445      	lw	a0,a1c01780 <$LCFI106+0x14c>
a1c01670:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c01674:	d205      	sw	v0,20(sp)
a1c01676:	9205      	lw	v0,20(sp)
a1c01678:	b543      	lw	a1,a1c01784 <$LCFI106+0x150>
a1c0167a:	ed4c      	and	a1,v0
a1c0167c:	2506      	beqz	a1,a1c0168a <$LCFI106+0x56>
a1c0167e:	6b11      	li	v1,17
a1c01680:	b742      	lw	a3,a1c01788 <$LCFI106+0x154>
a1c01682:	ee6b      	neg	a2,v1
a1c01684:	ee4c      	and	a2,v0
a1c01686:	e8ec      	and	s0,a3
a1c01688:	e8cd      	or	s0,a2
a1c0168a:	f7ff 6a1f 	li	v0,65535
a1c0168e:	b440      	lw	a0,a1c0178c <$LCFI106+0x158>
a1c01690:	ea0c      	and	v0,s0
a1c01692:	ea8c      	and	v0,a0
a1c01694:	2202      	beqz	v0,a1c0169a <$LCFI106+0x66>
a1c01696:	b53f      	lw	a1,a1c01790 <$LCFI106+0x15c>
a1c01698:	e8ad      	or	s0,a1
a1c0169a:	f7ff 6a1f 	li	v0,65535
a1c0169e:	b63e      	lw	a2,a1c01794 <$LCFI106+0x160>
a1c016a0:	ea0c      	and	v0,s0
a1c016a2:	ee4c      	and	a2,v0
a1c016a4:	2608      	beqz	a2,a1c016b6 <$LCFI106+0x82>
a1c016a6:	9304      	lw	v1,16(sp)
a1c016a8:	2b06      	bnez	v1,a1c016b6 <$LCFI106+0x82>
a1c016aa:	b738      	lw	a3,a1c01788 <$LCFI106+0x154>
a1c016ac:	f6ff 4b1f 	addiu	v1,-257
a1c016b0:	eb4c      	and	v1,v0
a1c016b2:	e8ec      	and	s0,a3
a1c016b4:	e86d      	or	s0,v1
a1c016b6:	f7ff 6b1f 	li	v1,65535
a1c016ba:	eb0c      	and	v1,s0
a1c016bc:	235b      	beqz	v1,a1c01774 <$LCFI106+0x140>
a1c016be:	6703      	move	s0,v1
a1c016c0:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c016c4:	d305      	sw	v1,20(sp)
a1c016c6:	6c1a      	li	a0,26
a1c016c8:	e998      	mult	s1,a0
a1c016ca:	d208      	sw	v0,32(sp)
a1c016cc:	b233      	lw	v0,a1c01798 <$LCFI106+0x164>
a1c016ce:	eb12      	mflo	v1
a1c016d0:	6d00      	li	a1,0
a1c016d2:	e351      	addu	a0,v1,v0
a1c016d4:	9305      	lw	v1,20(sp)
a1c016d6:	6f01      	li	a3,1
a1c016d8:	67d0      	move	a2,s0
a1c016da:	eeec      	and	a2,a3
a1c016dc:	261c      	beqz	a2,a1c01716 <$LCFI106+0xe2>
a1c016de:	9204      	lw	v0,16(sp)
a1c016e0:	2209      	beqz	v0,a1c016f4 <$LCFI106+0xc0>
a1c016e2:	acc0      	lhu	a2,0(a0)
a1c016e4:	2603      	beqz	a2,a1c016ec <$LCFI106+0xb8>
a1c016e6:	ede4      	sllv	a3,a1
a1c016e8:	efef      	not	a3
a1c016ea:	ebec      	and	v1,a3
a1c016ec:	9211      	lw	v0,68(sp)
a1c016ee:	ee4d      	or	a2,v0
a1c016f0:	ccc0      	sh	a2,0(a0)
a1c016f2:	1011      	b	a1c01716 <$LCFI106+0xe2>
a1c016f4:	9211      	lw	v0,68(sp)
a1c016f6:	ee4f      	not	a2,v0
a1c016f8:	ac40      	lhu	v0,0(a0)
a1c016fa:	ee4c      	and	a2,v0
a1c016fc:	ccc0      	sh	a2,0(a0)
a1c016fe:	260b      	beqz	a2,a1c01716 <$LCFI106+0xe2>
a1c01700:	67c7      	move	a2,a3
a1c01702:	edc4      	sllv	a2,a1
a1c01704:	efcf      	not	a3,a2
a1c01706:	ef6c      	and	a3,v1
a1c01708:	32e0      	sll	v0,a3,8
a1c0170a:	3640      	sll	a2,v0,8
a1c0170c:	33c3      	sra	v1,a2,8
a1c0170e:	3363      	sra	v1,8
a1c01710:	f7ff 6f1f 	li	a3,65535
a1c01714:	ebec      	and	v1,a3
a1c01716:	3006      	srl	s0,1
a1c01718:	4c02      	addiu	a0,2
a1c0171a:	2002      	beqz	s0,a1c01720 <$LCFI106+0xec>
a1c0171c:	4d01      	addiu	a1,1
a1c0171e:	17db      	b	a1c016d6 <$LCFI106+0xa2>
a1c01720:	2326      	beqz	v1,a1c0176e <$LCFI106+0x13a>
a1c01722:	9004      	lw	s0,16(sp)
a1c01724:	b51e      	lw	a1,a1c0179c <$LCFI106+0x168>
a1c01726:	3424      	sll	a0,s1,1
a1c01728:	2805      	bnez	s0,a1c01734 <$LCFI106+0x100>
a1c0172a:	e595      	addu	a1,a0
a1c0172c:	ad40      	lhu	v0,0(a1)
a1c0172e:	ec6f      	not	a0,v1
a1c01730:	ec4c      	and	a0,v0
a1c01732:	1003      	b	a1c0173a <$LCFI106+0x106>
a1c01734:	e595      	addu	a1,a0
a1c01736:	ad80      	lhu	a0,0(a1)
a1c01738:	ec6d      	or	a0,v1
a1c0173a:	7101      	cmpi	s1,1
a1c0173c:	cd80      	sh	a0,0(a1)
a1c0173e:	6800      	li	s0,0
a1c01740:	6101      	btnez	a1c01744 <$LCFI106+0x110>
a1c01742:	6805      	li	s0,5
a1c01744:	b417      	lw	a0,a1c017a0 <$LCFI106+0x16c>
a1c01746:	a4a0      	lbu	a1,0(a0)
a1c01748:	2512      	beqz	a1,a1c0176e <$LCFI106+0x13a>
a1c0174a:	b215      	lw	v0,a1c0179c <$LCFI106+0x168>
a1c0174c:	3124      	sll	s1,1
a1c0174e:	e239      	addu	a2,v0,s1
a1c01750:	aea0      	lhu	a1,0(a2)
a1c01752:	4083      	addiu	a0,s0,3
a1c01754:	1a03 0564 	jal	a1c01590 <pmd_RDAWrite>
a1c01758:	d305      	sw	v1,20(sp)
a1c0175a:	b713      	lw	a3,a1c017a4 <$LCFI106+0x170>
a1c0175c:	9305      	lw	v1,20(sp)
a1c0175e:	ef6c      	and	a3,v1
a1c01760:	2706      	beqz	a3,a1c0176e <$LCFI106+0x13a>
a1c01762:	b512      	lw	a1,a1c017a8 <$LCFI106+0x174>
a1c01764:	e525      	addu	s1,a1,s1
a1c01766:	a9a0      	lhu	a1,0(s1)
a1c01768:	1a03 0564 	jal	a1c01590 <pmd_RDAWrite>
a1c0176c:	4084      	addiu	a0,s0,4
a1c0176e:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01772:	9408      	lw	a0,32(sp)
a1c01774:	970d      	lw	a3,52(sp)
a1c01776:	910c      	lw	s1,48(sp)
a1c01778:	900b      	lw	s0,44(sp)
a1c0177a:	ef00      	jr	a3
a1c0177c:	6307      	addiu	sp,56
a1c0177e:	6500      	nop
a1c01780:	25a4      	beqz	a1,a1c016ca <$LCFI106+0x96>
a1c01782:	a1c0      	lbu	a2,0(s1)
a1c01784:	0010      	addiu	s0,sp,64
a1c01786:	1005      	b	a1c01792 <$LCFI106+0x15e>
a1c01788:	0000      	addiu	s0,sp,0
a1c0178a:	ffff      	daddiu	a3,sp,124
a1c0178c:	0008      	addiu	s0,sp,32
a1c0178e:	1005      	b	a1c0179a <$LCFI106+0x166>
a1c01790:	0400      	addiu	a0,sp,0
a1c01792:	1005      	b	a1c0179e <$LCFI106+0x16a>
a1c01794:	0100      	addiu	s1,sp,0
a1c01796:	1005      	b	a1c017a2 <$LCFI106+0x16e>
a1c01798:	88c8      	lh	a2,16(s0)
a1c0179a:	a1c0      	lbu	a2,0(s1)
a1c0179c:	2794      	beqz	a3,a1c016c6 <$LCFI106+0x92>
a1c0179e:	a1c0      	lbu	a2,0(s1)
a1c017a0:	88b4      	lh	a1,40(s0)
a1c017a2:	a1c0      	lbu	a2,0(s1)
a1c017a4:	0004      	addiu	s0,sp,16
a1c017a6:	1005      	b	a1c017b2 <pmd_EnableMemoryPower+0x6>
a1c017a8:	2790      	beqz	a3,a1c016ca <$LCFI106+0x96>
a1c017aa:	a1c0      	lbu	a2,0(s1)

a1c017ac <pmd_EnableMemoryPower>:
a1c017ac:	6dff      	li	a1,255
a1c017ae:	ed8c      	and	a1,a0
a1c017b0:	b705      	lw	a3,a1c017c4 <$LCFI110+0xe>
a1c017b2:	b406      	lw	a0,a1c017c8 <$LCFI110+0x12>
a1c017b4:	63fd      	addiu	sp,-24

a1c017b6 <$LCFI110>:
a1c017b6:	6205      	sw	ra,20(sp)
a1c017b8:	1a03 058c 	jal	a1c01630 <pmd_EnableOpalLdo>
a1c017bc:	6e00      	li	a2,0
a1c017be:	9705      	lw	a3,20(sp)
a1c017c0:	ef00      	jr	a3
a1c017c2:	6303      	addiu	sp,24
a1c017c4:	0000      	addiu	s0,sp,0
a1c017c6:	8000      	lb	s0,0(s0)
a1c017c8:	1000      	b	a1c017ca <$LCFI110+0x14>
a1c017ca:	1005      	b	a1c017d6 <hal_DmaDone+0xa>

a1c017cc <hal_DmaDone>:
a1c017cc:	b408      	lw	a0,a1c017ec <hal_DmaDone+0x20>
a1c017ce:	6b04      	li	v1,4
a1c017d0:	9c41      	lw	v0,4(a0)
a1c017d2:	ea6c      	and	v0,v1
a1c017d4:	2a04      	bnez	v0,a1c017de <hal_DmaDone+0x12>
a1c017d6:	b607      	lw	a2,a1c017f0 <hal_DmaDone+0x24>
a1c017d8:	9ea0      	lw	a1,0(a2)
a1c017da:	6a00      	li	v0,0
a1c017dc:	2d04      	bnez	a1,a1c017e6 <hal_DmaDone+0x1a>
a1c017de:	6a00      	li	v0,0
a1c017e0:	b704      	lw	a3,a1c017f0 <hal_DmaDone+0x24>
a1c017e2:	df40      	sw	v0,0(a3)
a1c017e4:	6a01      	li	v0,1
a1c017e6:	e820      	jr	ra
a1c017e8:	6500      	nop
a1c017ea:	6500      	nop
a1c017ec:	8000      	lb	s0,0(s0)
a1c017ee:	a1a0      	lbu	a1,0(s1)
a1c017f0:	88fc      	lh	a3,56(s0)
a1c017f2:	a1c0      	lbu	a2,0(s1)

a1c017f4 <hal_DmaReadFcs>:
a1c017f4:	b604      	lw	a2,a1c01804 <hal_DmaReadFcs+0x10>
a1c017f6:	9eab      	lw	a1,44(a2)
a1c017f8:	33a2      	srl	v1,a1,8
a1c017fa:	3262      	srl	v0,v1,8
a1c017fc:	c4a0      	sb	a1,0(a0)
a1c017fe:	c461      	sb	v1,1(a0)
a1c01800:	e820      	jr	ra
a1c01802:	c442      	sb	v0,2(a0)
a1c01804:	8000      	lb	s0,0(s0)
a1c01806:	a1a0      	lbu	a1,0(s1)

a1c01808 <hal_DmaStart>:
a1c01808:	63fb      	addiu	sp,-40

a1c0180a <$LCFI8>:
a1c0180a:	d108      	sw	s1,32(sp)
a1c0180c:	b11e      	lw	s1,a1c01884 <$LCFI8+0x7a>
a1c0180e:	d007      	sw	s0,28(sp)
a1c01810:	6209      	sw	ra,36(sp)
a1c01812:	9961      	lw	v1,4(s1)
a1c01814:	6a01      	li	v0,1
a1c01816:	eb4c      	and	v1,v0
a1c01818:	6704      	move	s0,a0
a1c0181a:	2b2e      	bnez	v1,a1c01878 <$LCFI8+0x6e>
a1c0181c:	9980      	lw	a0,0(s1)
a1c0181e:	ec4c      	and	a0,v0
a1c01820:	242b      	beqz	a0,a1c01878 <$LCFI8+0x6e>
a1c01822:	b51a      	lw	a1,a1c01888 <$LCFI8+0x7e>
a1c01824:	6c11      	li	a0,17
a1c01826:	6e00      	li	a2,0
a1c01828:	1a03 0890 	jal	a1c02240 <hal_SysRequestFreq>
a1c0182c:	d204      	sw	v0,16(sp)
a1c0182e:	9604      	lw	a2,16(sp)
a1c01830:	b717      	lw	a3,a1c0188c <$LCFI8+0x82>
a1c01832:	dfc0      	sw	a2,0(a3)
a1c01834:	98a6      	lw	a1,24(s0)
a1c01836:	b217      	lw	v0,a1c01890 <$LCFI8+0x86>
a1c01838:	daa0      	sw	a1,0(v0)
a1c0183a:	9880      	lw	a0,0(s0)
a1c0183c:	b216      	lw	v0,a1c01894 <$LCFI8+0x8a>
a1c0183e:	d983      	sw	a0,12(s1)
a1c01840:	9861      	lw	v1,4(s0)
a1c01842:	d964      	sw	v1,16(s1)
a1c01844:	98e2      	lw	a3,8(s0)
a1c01846:	6b10      	li	v1,16
a1c01848:	d9e5      	sw	a3,20(s1)
a1c0184a:	98c3      	lw	a2,12(s0)
a1c0184c:	d9c7      	sw	a2,28(s1)
a1c0184e:	98a4      	lw	a1,16(s0)
a1c01850:	eaac      	and	v0,a1
a1c01852:	d946      	sw	v0,24(s1)
a1c01854:	9845      	lw	v0,20(s0)
a1c01856:	7203      	cmpi	v0,3
a1c01858:	6008      	bteqz	a1c0186a <$LCFI8+0x60>
a1c0185a:	7204      	cmpi	v0,4
a1c0185c:	6102      	btnez	a1c01862 <$LCFI8+0x58>
a1c0185e:	b30f      	lw	v1,a1c01898 <$LCFI8+0x8e>
a1c01860:	1004      	b	a1c0186a <$LCFI8+0x60>
a1c01862:	6803      	li	s0,3
a1c01864:	e84c      	and	s0,v0
a1c01866:	f500 3300 	sll	v1,s0,20
a1c0186a:	b107      	lw	s1,a1c01884 <$LCFI8+0x7a>
a1c0186c:	6c07      	li	a0,7
a1c0186e:	ec6d      	or	a0,v1
a1c01870:	d982      	sw	a0,8(s1)
a1c01872:	9942      	lw	v0,8(s1)
a1c01874:	6a00      	li	v0,0
a1c01876:	1001      	b	a1c0187a <$LCFI8+0x70>
a1c01878:	6a02      	li	v0,2
a1c0187a:	9709      	lw	a3,36(sp)
a1c0187c:	9108      	lw	s1,32(sp)
a1c0187e:	9007      	lw	s0,28(sp)
a1c01880:	ef00      	jr	a3
a1c01882:	6305      	addiu	sp,40
a1c01884:	8000      	lb	s0,0(s0)
a1c01886:	a1a0      	lbu	a1,0(s1)
a1c01888:	ea00      	jr	v0
a1c0188a:	0632      	addiu	a2,sp,200
a1c0188c:	88fc      	lh	a3,56(s0)
a1c0188e:	a1c0      	lbu	a2,0(s1)
a1c01890:	8904      	lh	s0,8(s1)
a1c01892:	a1c0      	lbu	a2,0(s1)
a1c01894:	ffff      	daddiu	a3,sp,124
a1c01896:	0003      	addiu	s0,sp,12
a1c01898:	0010      	addiu	s0,sp,64
a1c0189a:	0010      	addiu	s0,sp,64

a1c0189c <hal_DmaStopStd>:
a1c0189c:	63fb      	addiu	sp,-40

a1c0189e <$LCFI12>:
a1c0189e:	d007      	sw	s0,28(sp)
a1c018a0:	6209      	sw	ra,36(sp)
a1c018a2:	6704      	move	s0,a0
a1c018a4:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c018a8:	d108      	sw	s1,32(sp)
a1c018aa:	6782      	move	a0,v0
a1c018ac:	b224      	lw	v0,a1c0193c <$LCFI12+0x9e>
a1c018ae:	9a66      	lw	v1,24(v0)
a1c018b0:	f400 5b01 	sltiu	v1,1025
a1c018b4:	612c      	btnez	a1c0190e <$LCFI12+0x70>
a1c018b6:	9ac2      	lw	a2,8(v0)
a1c018b8:	6dff      	li	a1,255
a1c018ba:	4d01      	addiu	a1,1
a1c018bc:	eead      	or	a2,a1
a1c018be:	dac2      	sw	a2,8(v0)
a1c018c0:	1a03 05f3 	jal	a1c017cc <hal_DmaDone>
a1c018c4:	d404      	sw	a0,16(sp)
a1c018c6:	9404      	lw	a0,16(sp)
a1c018c8:	22fb      	beqz	v0,a1c018c0 <$LCFI12+0x22>
a1c018ca:	b21d      	lw	v0,a1c0193c <$LCFI12+0x9e>
a1c018cc:	9a26      	lw	s1,24(v0)
a1c018ce:	2905      	bnez	s1,a1c018da <$LCFI12+0x3c>
a1c018d0:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c018d4:	6500      	nop
a1c018d6:	d824      	sw	s1,16(s0)
a1c018d8:	1029      	b	a1c0192c <$LCFI12+0x8e>
a1c018da:	9ae3      	lw	a3,12(v0)
a1c018dc:	f7ff 691f 	li	s1,65535
a1c018e0:	d8e0      	sw	a3,0(s0)
a1c018e2:	9aa4      	lw	a1,16(v0)
a1c018e4:	d8a1      	sw	a1,4(s0)
a1c018e6:	9ac5      	lw	a2,20(v0)
a1c018e8:	b516      	lw	a1,a1c01940 <$LCFI12+0xa2>
a1c018ea:	d8c2      	sw	a2,8(s0)
a1c018ec:	9a67      	lw	v1,28(v0)
a1c018ee:	d863      	sw	v1,12(s0)
a1c018f0:	9ae6      	lw	a3,24(v0)
a1c018f2:	ef2c      	and	a3,s1
a1c018f4:	d8e4      	sw	a3,16(s0)
a1c018f6:	9dc0      	lw	a2,0(a1)
a1c018f8:	b113      	lw	s1,a1c01944 <$LCFI12+0xa6>
a1c018fa:	d8c5      	sw	a2,20(s0)
a1c018fc:	9960      	lw	v1,0(s1)
a1c018fe:	6f00      	li	a3,0
a1c01900:	d866      	sw	v1,24(s0)
a1c01902:	d9e0      	sw	a3,0(s1)
a1c01904:	dae2      	sw	a3,8(v0)
a1c01906:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c0190a:	6800      	li	s0,0
a1c0190c:	1010      	b	a1c0192e <$LCFI12+0x90>
a1c0190e:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01912:	6500      	nop
a1c01914:	6a00      	li	v0,0
a1c01916:	d844      	sw	v0,16(s0)
a1c01918:	d840      	sw	v0,0(s0)
a1c0191a:	d841      	sw	v0,4(s0)
a1c0191c:	d842      	sw	v0,8(s0)
a1c0191e:	d843      	sw	v0,12(s0)
a1c01920:	d845      	sw	v0,20(s0)
a1c01922:	d846      	sw	v0,24(s0)
a1c01924:	1a03 05f3 	jal	a1c017cc <hal_DmaDone>
a1c01928:	6500      	nop
a1c0192a:	22fc      	beqz	v0,a1c01924 <$LCFI12+0x86>
a1c0192c:	6800      	li	s0,0
a1c0192e:	b407      	lw	a0,a1c01948 <$LCFI12+0xaa>
a1c01930:	dc00      	sw	s0,0(a0)
a1c01932:	9709      	lw	a3,36(sp)
a1c01934:	9108      	lw	s1,32(sp)
a1c01936:	9007      	lw	s0,28(sp)
a1c01938:	ef00      	jr	a3
a1c0193a:	6305      	addiu	sp,40
a1c0193c:	8000      	lb	s0,0(s0)
a1c0193e:	a1a0      	lbu	a1,0(s1)
a1c01940:	8900      	lh	s0,0(s1)
a1c01942:	a1c0      	lbu	a2,0(s1)
a1c01944:	8904      	lh	s0,8(s1)
a1c01946:	a1c0      	lbu	a2,0(s1)
a1c01948:	88fc      	lh	a3,56(s0)
a1c0194a:	a1c0      	lbu	a2,0(s1)

a1c0194c <hal_DmaFcsStart>:
a1c0194c:	63f8      	addiu	sp,-64

a1c0194e <$LCFI16>:
a1c0194e:	d00d      	sw	s0,52(sp)
a1c01950:	620f      	sw	ra,60(sp)
a1c01952:	d10e      	sw	s1,56(sp)
a1c01954:	b225      	lw	v0,a1c019e8 <$LCFI16+0x9a>
a1c01956:	9a40      	lw	v0,0(v0)
a1c01958:	6b00      	li	v1,0
a1c0195a:	7201      	cmpi	v0,1
a1c0195c:	6704      	move	s0,a0
a1c0195e:	d304      	sw	v1,16(sp)
a1c01960:	d305      	sw	v1,20(sp)
a1c01962:	d306      	sw	v1,24(sp)
a1c01964:	d307      	sw	v1,28(sp)
a1c01966:	d308      	sw	v1,32(sp)
a1c01968:	d309      	sw	v1,36(sp)
a1c0196a:	d30a      	sw	v1,40(sp)
a1c0196c:	6004      	bteqz	a1c01976 <$LCFI16+0x28>
a1c0196e:	2206      	beqz	v0,a1c0197c <$LCFI16+0x2e>
a1c01970:	5a04      	sltiu	v0,4
a1c01972:	6004      	bteqz	a1c0197c <$LCFI16+0x2e>
a1c01974:	1033      	b	a1c019dc <$LCFI16+0x8e>
a1c01976:	1a03 0627 	jal	a1c0189c <hal_DmaStopStd>
a1c0197a:	0404      	addiu	a0,sp,16
a1c0197c:	b11c      	lw	s1,a1c019ec <$LCFI16+0x9e>
a1c0197e:	6d01      	li	a1,1
a1c01980:	9980      	lw	a0,0(s1)
a1c01982:	ecac      	and	a0,a1
a1c01984:	242b      	beqz	a0,a1c019dc <$LCFI16+0x8e>
a1c01986:	b51b      	lw	a1,a1c019f0 <$LCFI16+0xa2>
a1c01988:	6c11      	li	a0,17
a1c0198a:	1a03 0890 	jal	a1c02240 <hal_SysRequestFreq>
a1c0198e:	6e00      	li	a2,0
a1c01990:	6a03      	li	v0,3
a1c01992:	b716      	lw	a3,a1c019e8 <$LCFI16+0x9a>
a1c01994:	df40      	sw	v0,0(a3)
a1c01996:	98c2      	lw	a2,8(s0)
a1c01998:	b517      	lw	a1,a1c019f4 <$LCFI16+0xa6>
a1c0199a:	ddc0      	sw	a2,0(a1)
a1c0199c:	9880      	lw	a0,0(s0)
a1c0199e:	6b01      	li	v1,1
a1c019a0:	ea6b      	neg	v0,v1
a1c019a2:	d983      	sw	a0,12(s1)
a1c019a4:	d944      	sw	v0,16(s1)
a1c019a6:	a8e2      	lhu	a3,4(s0)
a1c019a8:	d9e6      	sw	a3,24(s1)
a1c019aa:	98c2      	lw	a2,8(s0)
a1c019ac:	2602      	beqz	a2,a1c019b2 <$LCFI16+0x64>
a1c019ae:	b213      	lw	v0,a1c019f8 <$LCFI16+0xaa>
a1c019b0:	1001      	b	a1c019b4 <$LCFI16+0x66>
a1c019b2:	b213      	lw	v0,a1c019fc <$LCFI16+0xae>
a1c019b4:	b00e      	lw	s0,a1c019ec <$LCFI16+0x9e>
a1c019b6:	d842      	sw	v0,8(s0)
a1c019b8:	1a03 05f3 	jal	a1c017cc <hal_DmaDone>
a1c019bc:	6500      	nop
a1c019be:	22fc      	beqz	v0,a1c019b8 <$LCFI16+0x6a>
a1c019c0:	9108      	lw	s1,32(sp)
a1c019c2:	2104      	beqz	s1,a1c019cc <$LCFI16+0x7e>
a1c019c4:	1a03 0602 	jal	a1c01808 <hal_DmaStart>
a1c019c8:	0404      	addiu	a0,sp,16
a1c019ca:	1006      	b	a1c019d8 <$LCFI16+0x8a>
a1c019cc:	6c11      	li	a0,17
a1c019ce:	f010 6d00 	li	a1,32768
a1c019d2:	1a03 0890 	jal	a1c02240 <hal_SysRequestFreq>
a1c019d6:	6e00      	li	a2,0
a1c019d8:	6a00      	li	v0,0
a1c019da:	1001      	b	a1c019de <$LCFI16+0x90>
a1c019dc:	6a02      	li	v0,2
a1c019de:	970f      	lw	a3,60(sp)
a1c019e0:	910e      	lw	s1,56(sp)
a1c019e2:	900d      	lw	s0,52(sp)
a1c019e4:	ef00      	jr	a3
a1c019e6:	6308      	addiu	sp,64
a1c019e8:	88fc      	lh	a3,56(s0)
a1c019ea:	a1c0      	lbu	a2,0(s1)
a1c019ec:	8000      	lb	s0,0(s0)
a1c019ee:	a1a0      	lbu	a1,0(s1)
a1c019f0:	ea00      	jr	v0
a1c019f2:	0632      	addiu	a2,sp,200
a1c019f4:	8904      	lh	s0,8(s1)
a1c019f6:	a1c0      	lbu	a2,0(s1)
a1c019f8:	0007      	addiu	s0,sp,28
a1c019fa:	0001      	addiu	s0,sp,4
a1c019fc:	0005      	addiu	s0,sp,20
a1c019fe:	0001      	addiu	s0,sp,4

a1c01a00 <hal_EbcConfigRemap>:
a1c01a00:	b202      	lw	v0,a1c01a08 <hal_EbcConfigRemap+0x8>
a1c01a02:	e820      	jr	ra
a1c01a04:	da82      	sw	a0,8(v0)
a1c01a06:	6500      	nop
a1c01a08:	0000      	addiu	s0,sp,0
a1c01a0a:	a18c      	lbu	a0,12(s1)

a1c01a0c <hal_EbcFlashOpen>:
a1c01a0c:	b202      	lw	v0,a1c01a14 <hal_EbcFlashOpen+0x8>
a1c01a0e:	e820      	jr	ra
a1c01a10:	6500      	nop
a1c01a12:	6500      	nop
a1c01a14:	0000      	addiu	s0,sp,0
a1c01a16:	a800      	lhu	s0,0(s0)

a1c01a18 <hal_EbcCsOpen>:
a1c01a18:	63fb      	addiu	sp,-40

a1c01a1a <$LCFI7>:
a1c01a1a:	d108      	sw	s1,32(sp)
a1c01a1c:	d007      	sw	s0,28(sp)
a1c01a1e:	d404      	sw	a0,16(sp)
a1c01a20:	6209      	sw	ra,36(sp)
a1c01a22:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c01a26:	6705      	move	s0,a1
a1c01a28:	9404      	lw	a0,16(sp)
a1c01a2a:	6722      	move	s1,v0
a1c01a2c:	b231      	lw	v0,a1c01af0 <$LCFI7+0xd6>
a1c01a2e:	5c05      	sltiu	a0,5
a1c01a30:	9a40      	lw	v0,0(v0)
a1c01a32:	9860      	lw	v1,0(s0)
a1c01a34:	9841      	lw	v0,4(s0)
a1c01a36:	610a      	btnez	a1c01a4c <$LCFI7+0x32>
a1c01a38:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01a3c:	6791      	move	a0,s1
a1c01a3e:	9709      	lw	a3,36(sp)
a1c01a40:	6800      	li	s0,0
a1c01a42:	6750      	move	v0,s0
a1c01a44:	9108      	lw	s1,32(sp)
a1c01a46:	9007      	lw	s0,28(sp)
a1c01a48:	ef00      	jr	a3
a1c01a4a:	6305      	addiu	sp,40
a1c01a4c:	3084      	sll	s0,a0,1
a1c01a4e:	0e04      	la	a2,a1c01a5c <$LCFI7+0x42>
a1c01a50:	e61d      	addu	a3,a2,s0
a1c01a52:	8f80      	lh	a0,0(a3)
a1c01a54:	e695      	addu	a1,a2,a0
a1c01a56:	ed00      	jr	a1
a1c01a58:	6500      	nop
a1c01a5a:	6500      	nop
a1c01a5c:	0049      	addiu	s0,sp,292
a1c01a5e:	004d      	addiu	s0,sp,308
a1c01a60:	0075      	addiu	s0,sp,468
a1c01a62:	000b      	addiu	s0,sp,44
a1c01a64:	0029      	addiu	s0,sp,164
a1c01a66:	b424      	lw	a0,a1c01af4 <$LCFI7+0xda>
a1c01a68:	b024      	lw	s0,a1c01af8 <$LCFI7+0xde>
a1c01a6a:	f400 dc6c 	sw	v1,1036(a0)
a1c01a6e:	f420 dc48 	sw	v0,1064(a0)
a1c01a72:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01a76:	6791      	move	a0,s1
a1c01a78:	9709      	lw	a3,36(sp)
a1c01a7a:	6750      	move	v0,s0
a1c01a7c:	9108      	lw	s1,32(sp)
a1c01a7e:	9007      	lw	s0,28(sp)
a1c01a80:	ef00      	jr	a3
a1c01a82:	6305      	addiu	sp,40
a1c01a84:	b71c      	lw	a3,a1c01af4 <$LCFI7+0xda>
a1c01a86:	6791      	move	a0,s1
a1c01a88:	f400 df6c 	sw	v1,1036(a3)
a1c01a8c:	f420 df50 	sw	v0,1072(a3)
a1c01a90:	b01b      	lw	s0,a1c01afc <$LCFI7+0xe2>
a1c01a92:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01a96:	6500      	nop
a1c01a98:	9709      	lw	a3,36(sp)
a1c01a9a:	6750      	move	v0,s0
a1c01a9c:	9108      	lw	s1,32(sp)
a1c01a9e:	9007      	lw	s0,28(sp)
a1c01aa0:	ef00      	jr	a3
a1c01aa2:	6305      	addiu	sp,40
a1c01aa4:	e845      	break	2
a1c01aa6:	17c8      	b	a1c01a38 <$LCFI7+0x1e>
a1c01aa8:	b613      	lw	a2,a1c01af4 <$LCFI7+0xda>
a1c01aaa:	6783      	move	a0,v1
a1c01aac:	67a2      	move	a1,v0
a1c01aae:	f400 de6c 	sw	v1,1036(a2)
a1c01ab2:	f400 de58 	sw	v0,1048(a2)
a1c01ab6:	1a03 0185 	jal	a1c00614 <hal_BootSectorSetEbcConfig>
a1c01aba:	6500      	nop
a1c01abc:	b011      	lw	s0,a1c01b00 <$LCFI7+0xe6>
a1c01abe:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01ac2:	6791      	move	a0,s1
a1c01ac4:	9709      	lw	a3,36(sp)
a1c01ac6:	6750      	move	v0,s0
a1c01ac8:	9108      	lw	s1,32(sp)
a1c01aca:	9007      	lw	s0,28(sp)
a1c01acc:	ef00      	jr	a3
a1c01ace:	6305      	addiu	sp,40
a1c01ad0:	b509      	lw	a1,a1c01af4 <$LCFI7+0xda>
a1c01ad2:	6791      	move	a0,s1
a1c01ad4:	f400 dd6c 	sw	v1,1036(a1)
a1c01ad8:	f420 dd40 	sw	v0,1056(a1)
a1c01adc:	b00a      	lw	s0,a1c01b04 <$LCFI7+0xea>
a1c01ade:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01ae2:	6500      	nop
a1c01ae4:	9709      	lw	a3,36(sp)
a1c01ae6:	6750      	move	v0,s0
a1c01ae8:	9108      	lw	s1,32(sp)
a1c01aea:	9007      	lw	s0,28(sp)
a1c01aec:	ef00      	jr	a3
a1c01aee:	6305      	addiu	sp,40
a1c01af0:	0000      	addiu	s0,sp,0
a1c01af2:	a18c      	lbu	a0,12(s1)
a1c01af4:	4000      	addiu	s0,s0,0
a1c01af6:	a1a0      	lbu	a1,0(s1)
a1c01af8:	0000      	addiu	s0,sp,0
a1c01afa:	a188      	lbu	a0,8(s1)
a1c01afc:	0000      	addiu	s0,sp,0
a1c01afe:	a100      	lbu	s0,0(s1)
a1c01b00:	0000      	addiu	s0,sp,0
a1c01b02:	a200      	lbu	s0,0(v0)
a1c01b04:	0000      	addiu	s0,sp,0
a1c01b06:	a180      	lbu	a0,0(s1)

a1c01b08 <hal_HstWaitLastEventSent>:
a1c01b08:	b407      	lw	a0,a1c01b24 <hal_HstWaitLastEventSent+0x1c>
a1c01b0a:	6b01      	li	v1,1
a1c01b0c:	9c42      	lw	v0,8(a0)
a1c01b0e:	ea6c      	and	v0,v1
a1c01b10:	22fb      	beqz	v0,a1c01b08 <hal_HstWaitLastEventSent>
a1c01b12:	b706      	lw	a3,a1c01b28 <hal_HstWaitLastEventSent+0x20>
a1c01b14:	f707 6e00 	li	a2,16128
a1c01b18:	9fa1      	lw	a1,4(a3)
a1c01b1a:	edcc      	and	a1,a2
a1c01b1c:	2dfa      	bnez	a1,a1c01b12 <hal_HstWaitLastEventSent+0xa>
a1c01b1e:	e820      	jr	ra
a1c01b20:	6500      	nop
a1c01b22:	6500      	nop
a1c01b24:	f000 a1a3 	lbu	a1,3(s1)
a1c01b28:	4000      	addiu	s0,s0,0
a1c01b2a:	a1a1      	lbu	a1,1(s1)

a1c01b2c <hal_IspiTxFinished>:
a1c01b2c:	63fd      	addiu	sp,-24

a1c01b2e <$LCFI19>:
a1c01b2e:	6205      	sw	ra,20(sp)
a1c01b30:	1b03 01d7 	jal	a1e0075c <boot_IspiTxFinished>
a1c01b34:	6500      	nop
a1c01b36:	9705      	lw	a3,20(sp)
a1c01b38:	ef00      	jr	a3
a1c01b3a:	6303      	addiu	sp,24

a1c01b3c <hal_IspiTxFifoAvail>:
a1c01b3c:	63fd      	addiu	sp,-24

a1c01b3e <$LCFI23>:
a1c01b3e:	6205      	sw	ra,20(sp)
a1c01b40:	1b03 01d0 	jal	a1e00740 <boot_IspiTxFifoAvail>
a1c01b44:	6500      	nop
a1c01b46:	9705      	lw	a3,20(sp)
a1c01b48:	ef00      	jr	a3
a1c01b4a:	6303      	addiu	sp,24

a1c01b4c <hal_IspiSendData>:
a1c01b4c:	63fd      	addiu	sp,-24

a1c01b4e <$LCFI27>:
a1c01b4e:	6205      	sw	ra,20(sp)
a1c01b50:	d004      	sw	s0,16(sp)
a1c01b52:	b20b      	lw	v0,a1c01b7c <$LCFI27+0x2e>
a1c01b54:	6bff      	li	v1,255
a1c01b56:	9a40      	lw	v0,0(v0)
a1c01b58:	ebcc      	and	v1,a2
a1c01b5a:	b60a      	lw	a2,a1c01b80 <$LCFI27+0x32>
a1c01b5c:	72ff      	cmpi	v0,255
a1c01b5e:	9e05      	lw	s0,20(a2)
a1c01b60:	6004      	bteqz	a1c01b6a <$LCFI27+0x1c>
a1c01b62:	ea8e      	xor	v0,a0
a1c01b64:	2202      	beqz	v0,a1c01b6a <$LCFI27+0x1c>
a1c01b66:	6a00      	li	v0,0
a1c01b68:	2b03      	bnez	v1,a1c01b70 <$LCFI27+0x22>
a1c01b6a:	1b03 01b9 	jal	a1e006e4 <boot_IspiSendData>
a1c01b6e:	67c3      	move	a2,v1
a1c01b70:	b304      	lw	v1,a1c01b80 <$LCFI27+0x32>
a1c01b72:	db05      	sw	s0,20(v1)
a1c01b74:	9705      	lw	a3,20(sp)
a1c01b76:	9004      	lw	s0,16(sp)
a1c01b78:	ef00      	jr	a3
a1c01b7a:	6303      	addiu	sp,24
a1c01b7c:	2518      	beqz	a1,a1c01bae <hal_MapEngineRegisterModule+0x6>
a1c01b7e:	a1c0      	lbu	a2,0(s1)
a1c01b80:	1000      	b	a1c01b82 <$LCFI27+0x34>
a1c01b82:	a1a0      	lbu	a1,0(s1)

a1c01b84 <hal_MapEngineOpen>:
a1c01b84:	b206      	lw	v0,a1c01b9c <hal_MapEngineOpen+0x18>
a1c01b86:	b407      	lw	a0,a1c01ba0 <hal_MapEngineOpen+0x1c>
a1c01b88:	6b00      	li	v1,0
a1c01b8a:	da60      	sw	v1,0(v0)
a1c01b8c:	da61      	sw	v1,4(v0)
a1c01b8e:	4a08      	addiu	v0,8
a1c01b90:	ea8a      	cmp	v0,a0
a1c01b92:	61fa      	btnez	a1c01b88 <hal_MapEngineOpen+0x4>
a1c01b94:	b402      	lw	a0,a1c01b9c <hal_MapEngineOpen+0x18>
a1c01b96:	b204      	lw	v0,a1c01ba4 <hal_MapEngineOpen+0x20>
a1c01b98:	e820      	jr	ra
a1c01b9a:	da80      	sw	a0,0(v0)
a1c01b9c:	8908      	lh	s0,16(s1)
a1c01b9e:	a1c0      	lbu	a2,0(s1)
a1c01ba0:	89d8      	lh	a2,48(s1)
a1c01ba2:	a1c0      	lbu	a2,0(s1)
a1c01ba4:	027c      	addiu	v0,sp,496
a1c01ba6:	a1c0      	lbu	a2,0(s1)

a1c01ba8 <hal_MapEngineRegisterModule>:
a1c01ba8:	5c1a      	sltiu	a0,26
a1c01baa:	6a05      	li	v0,5
a1c01bac:	6006      	bteqz	a1c01bba <hal_MapEngineRegisterModule+0x12>
a1c01bae:	b205      	lw	v0,a1c01bc0 <hal_MapEngineRegisterModule+0x18>
a1c01bb0:	338c      	sll	v1,a0,3
a1c01bb2:	e271      	addu	a0,v0,v1
a1c01bb4:	dca0      	sw	a1,0(a0)
a1c01bb6:	dcc1      	sw	a2,4(a0)
a1c01bb8:	6a00      	li	v0,0
a1c01bba:	e820      	jr	ra
a1c01bbc:	6500      	nop
a1c01bbe:	6500      	nop
a1c01bc0:	8908      	lh	s0,16(s1)
a1c01bc2:	a1c0      	lbu	a2,0(s1)

a1c01bc4 <hal_SpiFlashInit>:
a1c01bc4:	63fd      	addiu	sp,-24

a1c01bc6 <$LCFI5>:
a1c01bc6:	6205      	sw	ra,20(sp)
a1c01bc8:	d004      	sw	s0,16(sp)
a1c01bca:	9c41      	lw	v0,4(a0)
a1c01bcc:	6b00      	li	v1,0
a1c01bce:	4aff      	addiu	v0,-1
a1c01bd0:	5a02      	sltiu	v0,2
a1c01bd2:	6004      	bteqz	a1c01bdc <$LCFI5+0x16>
a1c01bd4:	b316      	lw	v1,a1c01c2c <$LCFI5+0x66>
a1c01bd6:	3548      	sll	a1,v0,2
a1c01bd8:	e3a9      	addu	v0,v1,a1
a1c01bda:	9a60      	lw	v1,0(v0)
a1c01bdc:	9cc0      	lw	a2,0(a0)
a1c01bde:	7602      	cmpi	a2,2
a1c01be0:	6102      	btnez	a1c01be6 <$LCFI5+0x20>
a1c01be2:	b714      	lw	a3,a1c01c30 <$LCFI5+0x6a>
a1c01be4:	ebed      	or	v1,a3
a1c01be6:	a409      	lbu	s0,9(a0)
a1c01be8:	6f07      	li	a3,7
a1c01bea:	a4c8      	lbu	a2,8(a0)
a1c01bec:	e8ec      	and	s0,a3
a1c01bee:	3010      	sll	s0,4
a1c01bf0:	35c0      	sll	a1,a2,8
a1c01bf2:	a44a      	lbu	v0,10(a0)
a1c01bf4:	e8ad      	or	s0,a1
a1c01bf6:	e86d      	or	s0,v1
a1c01bf8:	2202      	beqz	v0,a1c01bfe <$LCFI5+0x38>
a1c01bfa:	6b80      	li	v1,128
a1c01bfc:	e86d      	or	s0,v1
a1c01bfe:	a44b      	lbu	v0,11(a0)
a1c01c00:	2202      	beqz	v0,a1c01c06 <$LCFI5+0x40>
a1c01c02:	6d04      	li	a1,4
a1c01c04:	e8ad      	or	s0,a1
a1c01c06:	a4cc      	lbu	a2,12(a0)
a1c01c08:	2602      	beqz	a2,a1c01c0e <$LCFI5+0x48>
a1c01c0a:	6c02      	li	a0,2
a1c01c0c:	e88d      	or	s0,a0
a1c01c0e:	6c01      	li	a0,1
a1c01c10:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c01c14:	67b0      	move	a1,s0
a1c01c16:	b308      	lw	v1,a1c01c34 <$LCFI5+0x6e>
a1c01c18:	6c01      	li	a0,1
a1c01c1a:	9be3      	lw	a3,12(v1)
a1c01c1c:	ef8c      	and	a3,a0
a1c01c1e:	2ffb      	bnez	a3,a1c01c16 <$LCFI5+0x50>
a1c01c20:	db05      	sw	s0,20(v1)
a1c01c22:	9705      	lw	a3,20(sp)
a1c01c24:	6744      	move	v0,a0
a1c01c26:	9004      	lw	s0,16(sp)
a1c01c28:	ef00      	jr	a3
a1c01c2a:	6303      	addiu	sp,24
a1c01c2c:	25b0      	beqz	a1,a1c01b8e <hal_MapEngineOpen+0xa>
a1c01c2e:	a1c0      	lbu	a2,0(s1)
a1c01c30:	0000      	addiu	s0,sp,0
a1c01c32:	0001      	addiu	s0,sp,4
a1c01c34:	0000      	addiu	s0,sp,0
a1c01c36:	a1ac      	lbu	a1,12(s1)

a1c01c38 <hal_SysCallFreqChangeHandlers>:
a1c01c38:	63fb      	addiu	sp,-40

a1c01c3a <$LCFI0>:
a1c01c3a:	d108      	sw	s1,32(sp)
a1c01c3c:	d007      	sw	s0,28(sp)
a1c01c3e:	6209      	sw	ra,36(sp)
a1c01c40:	6764      	move	v1,a0
a1c01c42:	b009      	lw	s0,a1c01c64 <$LCFI0+0x2a>
a1c01c44:	b109      	lw	s1,a1c01c68 <$LCFI0+0x2e>
a1c01c46:	9840      	lw	v0,0(s0)
a1c01c48:	2204      	beqz	v0,a1c01c52 <$LCFI0+0x18>
a1c01c4a:	6783      	move	a0,v1
a1c01c4c:	ea40      	jalr	v0
a1c01c4e:	d304      	sw	v1,16(sp)
a1c01c50:	9304      	lw	v1,16(sp)
a1c01c52:	4804      	addiu	s0,4
a1c01c54:	e82a      	cmp	s0,s1
a1c01c56:	61f7      	btnez	a1c01c46 <$LCFI0+0xc>
a1c01c58:	9709      	lw	a3,36(sp)
a1c01c5a:	9108      	lw	s1,32(sp)
a1c01c5c:	9007      	lw	s0,28(sp)
a1c01c5e:	ef00      	jr	a3
a1c01c60:	6305      	addiu	sp,40
a1c01c62:	6500      	nop
a1c01c64:	89d8      	lh	a2,48(s1)
a1c01c66:	a1c0      	lbu	a2,0(s1)
a1c01c68:	8a68      	lh	v1,16(v0)
a1c01c6a:	a1c0      	lbu	a2,0(s1)

a1c01c6c <hal_SysGetFreq>:
a1c01c6c:	b202      	lw	v0,a1c01c74 <hal_SysGetFreq+0x8>
a1c01c6e:	e820      	jr	ra
a1c01c70:	9a40      	lw	v0,0(v0)
a1c01c72:	6500      	nop
a1c01c74:	2798      	beqz	a3,a1c01ba6 <hal_MapEngineOpen+0x22>
a1c01c76:	a1c0      	lbu	a2,0(s1)

a1c01c78 <hal_SysGetFrequencyConfiguration>:
a1c01c78:	6a00      	li	v0,0
a1c01c7a:	c640      	sb	v0,0(a2)
a1c01c7c:	b22c      	lw	v0,a1c01d2c <hal_SysGetFrequencyConfiguration+0xb4>
a1c01c7e:	6b05      	li	v1,5
a1c01c80:	ec4a      	cmp	a0,v0
a1c01c82:	604d      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01c84:	ea83      	sltu	v0,a0
a1c01c86:	611e      	btnez	a1c01cc4 <hal_SysGetFrequencyConfiguration+0x4c>
a1c01c88:	b22a      	lw	v0,a1c01d30 <hal_SysGetFrequencyConfiguration+0xb8>
a1c01c8a:	6b01      	li	v1,1
a1c01c8c:	ec4a      	cmp	a0,v0
a1c01c8e:	6047      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01c90:	ea83      	sltu	v0,a0
a1c01c92:	610b      	btnez	a1c01caa <hal_SysGetFrequencyConfiguration+0x32>
a1c01c94:	b328      	lw	v1,a1c01d34 <hal_SysGetFrequencyConfiguration+0xbc>
a1c01c96:	eb8e      	xor	v1,a0
a1c01c98:	233e      	beqz	v1,a1c01d16 <hal_SysGetFrequencyConfiguration+0x9e>
a1c01c9a:	b228      	lw	v0,a1c01d38 <hal_SysGetFrequencyConfiguration+0xc0>
a1c01c9c:	ec4a      	cmp	a0,v0
a1c01c9e:	603b      	bteqz	a1c01d16 <hal_SysGetFrequencyConfiguration+0x9e>
a1c01ca0:	f010 6e00 	li	a2,32768
a1c01ca4:	ecce      	xor	a0,a2
a1c01ca6:	2435      	beqz	a0,a1c01d12 <hal_SysGetFrequencyConfiguration+0x9a>
a1c01ca8:	103c      	b	a1c01d22 <hal_SysGetFrequencyConfiguration+0xaa>
a1c01caa:	b225      	lw	v0,a1c01d3c <hal_SysGetFrequencyConfiguration+0xc4>
a1c01cac:	6b03      	li	v1,3
a1c01cae:	ec4a      	cmp	a0,v0
a1c01cb0:	6036      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cb2:	b224      	lw	v0,a1c01d40 <hal_SysGetFrequencyConfiguration+0xc8>
a1c01cb4:	6b04      	li	v1,4
a1c01cb6:	ec4a      	cmp	a0,v0
a1c01cb8:	6032      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cba:	b223      	lw	v0,a1c01d44 <hal_SysGetFrequencyConfiguration+0xcc>
a1c01cbc:	6b02      	li	v1,2
a1c01cbe:	ec4e      	xor	a0,v0
a1c01cc0:	242e      	beqz	a0,a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cc2:	102f      	b	a1c01d22 <hal_SysGetFrequencyConfiguration+0xaa>
a1c01cc4:	b221      	lw	v0,a1c01d48 <hal_SysGetFrequencyConfiguration+0xd0>
a1c01cc6:	6b09      	li	v1,9
a1c01cc8:	ec4a      	cmp	a0,v0
a1c01cca:	6029      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01ccc:	ea83      	sltu	v0,a0
a1c01cce:	610d      	btnez	a1c01cea <hal_SysGetFrequencyConfiguration+0x72>
a1c01cd0:	b21f      	lw	v0,a1c01d4c <hal_SysGetFrequencyConfiguration+0xd4>
a1c01cd2:	6b07      	li	v1,7
a1c01cd4:	ec4a      	cmp	a0,v0
a1c01cd6:	6023      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cd8:	b21e      	lw	v0,a1c01d50 <hal_SysGetFrequencyConfiguration+0xd8>
a1c01cda:	6b08      	li	v1,8
a1c01cdc:	ec4a      	cmp	a0,v0
a1c01cde:	601f      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01ce0:	b21d      	lw	v0,a1c01d54 <hal_SysGetFrequencyConfiguration+0xdc>
a1c01ce2:	6b06      	li	v1,6
a1c01ce4:	ec4e      	xor	a0,v0
a1c01ce6:	241b      	beqz	a0,a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01ce8:	101c      	b	a1c01d22 <hal_SysGetFrequencyConfiguration+0xaa>
a1c01cea:	b21c      	lw	v0,a1c01d58 <hal_SysGetFrequencyConfiguration+0xe0>
a1c01cec:	6b0b      	li	v1,11
a1c01cee:	ec4a      	cmp	a0,v0
a1c01cf0:	6016      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cf2:	ea83      	sltu	v0,a0
a1c01cf4:	6105      	btnez	a1c01d00 <hal_SysGetFrequencyConfiguration+0x88>
a1c01cf6:	b21a      	lw	v0,a1c01d5c <hal_SysGetFrequencyConfiguration+0xe4>
a1c01cf8:	6b0a      	li	v1,10
a1c01cfa:	ec4e      	xor	a0,v0
a1c01cfc:	2410      	beqz	a0,a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01cfe:	1011      	b	a1c01d22 <hal_SysGetFrequencyConfiguration+0xaa>
a1c01d00:	b218      	lw	v0,a1c01d60 <hal_SysGetFrequencyConfiguration+0xe8>
a1c01d02:	6b0c      	li	v1,12
a1c01d04:	ec4a      	cmp	a0,v0
a1c01d06:	600b      	bteqz	a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01d08:	b217      	lw	v0,a1c01d64 <hal_SysGetFrequencyConfiguration+0xec>
a1c01d0a:	6b0d      	li	v1,13
a1c01d0c:	ec4e      	xor	a0,v0
a1c01d0e:	2407      	beqz	a0,a1c01d1e <hal_SysGetFrequencyConfiguration+0xa6>
a1c01d10:	1008      	b	a1c01d22 <hal_SysGetFrequencyConfiguration+0xaa>
a1c01d12:	e820      	jr	ra
a1c01d14:	dd80      	sw	a0,0(a1)
a1c01d16:	6c00      	li	a0,0
a1c01d18:	b208      	lw	v0,a1c01d38 <hal_SysGetFrequencyConfiguration+0xc0>
a1c01d1a:	e820      	jr	ra
a1c01d1c:	dd80      	sw	a0,0(a1)
a1c01d1e:	e820      	jr	ra
a1c01d20:	dd60      	sw	v1,0(a1)
a1c01d22:	6f00      	li	a3,0
a1c01d24:	b205      	lw	v0,a1c01d38 <hal_SysGetFrequencyConfiguration+0xc0>
a1c01d26:	e820      	jr	ra
a1c01d28:	dde0      	sw	a3,0(a1)
a1c01d2a:	6500      	nop
a1c01d2c:	ea00      	jr	v0
a1c01d2e:	0632      	addiu	a2,sp,200
a1c01d30:	17c0      	b	a1c01cb2 <hal_SysGetFrequencyConfiguration+0x3a>
a1c01d32:	0253      	addiu	v0,sp,332
a1c01d34:	5d40      	sltiu	a1,64
a1c01d36:	00c6      	addiu	s0,sp,792
a1c01d38:	ba80      	lwu	a0,0(v0)
a1c01d3a:	018c      	addiu	s1,sp,560
a1c01d3c:	2f80      	bnez	a3,a1c01c3e <$LCFI0+0x4>
a1c01d3e:	04a6      	addiu	a0,sp,664
a1c01d40:	3649      	dsll	a2,v0,2
a1c01d42:	0550      	addiu	a1,sp,320
a1c01d44:	7500      	cmpi	a1,0
a1c01d46:	0319      	addiu	v1,sp,100
a1c01d48:	5f00      	sltiu	a3,0
a1c01d4a:	094c      	la	s1,a1c01e78 <$LCFI11+0xae>
a1c01d4c:	4c00      	addiu	a0,0
a1c01d4e:	0770      	addiu	a3,sp,448
a1c01d50:	e2ab      	subu	v0,a1
a1c01d52:	0843      	la	s0,a1c01e5c <$LCFI11+0x92>
a1c01d54:	2dd1      	bnez	a1,a1c01cf8 <hal_SysGetFrequencyConfiguration+0x80>
a1c01d56:	06c3      	addiu	a2,sp,780
a1c01d58:	d400      	sw	a0,0(sp)
a1c01d5a:	0c65      	la	a0,a1c01eec <$LCFI20+0xa>
a1c01d5c:	6c92      	li	a0,146
a1c01d5e:	0aa0      	la	v0,a1c01fdc <$LCFI28+0x8e>
a1c01d60:	9800      	lw	s0,0(s0)
a1c01d62:	0ee0      	la	a2,a1c020e0 <$LCFI52+0x16>
a1c01d64:	be00      	lwu	s0,0(a2)
a1c01d66:	1298      	b	a1c02298 <$LCFI56+0x56>

a1c01d68 <hal_SysExitCriticalSection>:
a1c01d68:	b302      	lw	v1,a1c01d70 <hal_SysExitCriticalSection+0x8>
a1c01d6a:	9b40      	lw	v0,0(v1)
a1c01d6c:	e820      	jr	ra
a1c01d6e:	da80      	sw	a0,0(v0)
a1c01d70:	0244      	addiu	v0,sp,272
a1c01d72:	a1c0      	lbu	a2,0(s1)

a1c01d74 <hal_SysEnterCriticalSection>:
a1c01d74:	b302      	lw	v1,a1c01d7c <hal_SysEnterCriticalSection+0x8>
a1c01d76:	9b40      	lw	v0,0(v1)
a1c01d78:	e820      	jr	ra
a1c01d7a:	9a40      	lw	v0,0(v0)
a1c01d7c:	0244      	addiu	v0,sp,272
a1c01d7e:	a1c0      	lbu	a2,0(s1)

a1c01d80 <hal_SysResetOut>:
a1c01d80:	6aff      	li	v0,255
a1c01d82:	63fc      	addiu	sp,-32

a1c01d84 <$LCFI7>:
a1c01d84:	ea8c      	and	v0,a0
a1c01d86:	d106      	sw	s1,24(sp)
a1c01d88:	d005      	sw	s0,20(sp)
a1c01d8a:	6207      	sw	ra,28(sp)
a1c01d8c:	b00b      	lw	s0,a1c01db8 <$LCFI7+0x34>
a1c01d8e:	b10c      	lw	s1,a1c01dbc <$LCFI7+0x38>
a1c01d90:	2a02      	bnez	v0,a1c01d96 <$LCFI7+0x12>
a1c01d92:	d822      	sw	s1,8(s0)
a1c01d94:	100b      	b	a1c01dac <$LCFI7+0x28>
a1c01d96:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c01d9a:	6500      	nop
a1c01d9c:	b409      	lw	a0,a1c01dc0 <$LCFI7+0x3c>
a1c01d9e:	b30a      	lw	v1,a1c01dc4 <$LCFI7+0x40>
a1c01da0:	d880      	sw	a0,0(s0)
a1c01da2:	d821      	sw	s1,4(s0)
a1c01da4:	6782      	move	a0,v0
a1c01da6:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01daa:	d860      	sw	v1,0(s0)
a1c01dac:	9707      	lw	a3,28(sp)
a1c01dae:	9106      	lw	s1,24(sp)
a1c01db0:	9005      	lw	s0,20(sp)
a1c01db2:	ef00      	jr	a3
a1c01db4:	6304      	addiu	sp,32
a1c01db6:	6500      	nop
a1c01db8:	0000      	addiu	s0,sp,0
a1c01dba:	a1a0      	lbu	a1,0(s1)
a1c01dbc:	0000      	addiu	s0,sp,0
a1c01dbe:	4000      	addiu	s0,s0,0
a1c01dc0:	0001      	addiu	s0,sp,4
a1c01dc2:	00a5      	addiu	s0,sp,660
a1c01dc4:	0000      	addiu	s0,sp,0
a1c01dc6:	00a5      	addiu	s0,sp,660

a1c01dc8 <hal_SysSetSpiFlashClock>:
a1c01dc8:	63fc      	addiu	sp,-32

a1c01dca <$LCFI11>:
a1c01dca:	d106      	sw	s1,24(sp)
a1c01dcc:	d005      	sw	s0,20(sp)
a1c01dce:	6207      	sw	ra,28(sp)
a1c01dd0:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c01dd4:	6704      	move	s0,a0
a1c01dd6:	b32b      	lw	v1,a1c01e80 <$LCFI11+0xb6>
a1c01dd8:	6c0f      	li	a0,15
a1c01dda:	4b7c      	addiu	v1,124
a1c01ddc:	9b6c      	lw	v1,48(v1)
a1c01dde:	6900      	li	s1,0
a1c01de0:	eb8c      	and	v1,a0
a1c01de2:	5b0e      	sltiu	v1,14
a1c01de4:	6004      	bteqz	a1c01dee <$LCFI11+0x24>
a1c01de6:	3668      	sll	a2,v1,2
a1c01de8:	b727      	lw	a3,a1c01e84 <$LCFI11+0xba>
a1c01dea:	e7d5      	addu	a1,a3,a2
a1c01dec:	9d20      	lw	s1,0(a1)
a1c01dee:	b427      	lw	a0,a1c01e88 <$LCFI11+0xbe>
a1c01df0:	6b07      	li	v1,7
a1c01df2:	e88a      	cmp	s0,a0
a1c01df4:	6037      	bteqz	a1c01e64 <$LCFI11+0x9a>
a1c01df6:	ec03      	sltu	a0,s0
a1c01df8:	611a      	btnez	a1c01e2e <$LCFI11+0x64>
a1c01dfa:	b425      	lw	a0,a1c01e8c <$LCFI11+0xc2>
a1c01dfc:	6b03      	li	v1,3
a1c01dfe:	e88a      	cmp	s0,a0
a1c01e00:	6031      	bteqz	a1c01e64 <$LCFI11+0x9a>
a1c01e02:	ec03      	sltu	a0,s0
a1c01e04:	6108      	btnez	a1c01e16 <$LCFI11+0x4c>
a1c01e06:	b423      	lw	a0,a1c01e90 <$LCFI11+0xc6>
a1c01e08:	6b01      	li	v1,1
a1c01e0a:	ec0e      	xor	a0,s0
a1c01e0c:	242b      	beqz	a0,a1c01e64 <$LCFI11+0x9a>
a1c01e0e:	b322      	lw	v1,a1c01e94 <$LCFI11+0xca>
a1c01e10:	e86e      	xor	s0,v1
a1c01e12:	6b02      	li	v1,2
a1c01e14:	1025      	b	a1c01e60 <$LCFI11+0x96>
a1c01e16:	b521      	lw	a1,a1c01e98 <$LCFI11+0xce>
a1c01e18:	6b05      	li	v1,5
a1c01e1a:	ed0e      	xor	a1,s0
a1c01e1c:	2523      	beqz	a1,a1c01e64 <$LCFI11+0x9a>
a1c01e1e:	b620      	lw	a2,a1c01e9c <$LCFI11+0xd2>
a1c01e20:	6b06      	li	v1,6
a1c01e22:	ee0e      	xor	a2,s0
a1c01e24:	261f      	beqz	a2,a1c01e64 <$LCFI11+0x9a>
a1c01e26:	b71f      	lw	a3,a1c01ea0 <$LCFI11+0xd6>
a1c01e28:	6b04      	li	v1,4
a1c01e2a:	e8ee      	xor	s0,a3
a1c01e2c:	1019      	b	a1c01e60 <$LCFI11+0x96>
a1c01e2e:	b41e      	lw	a0,a1c01ea4 <$LCFI11+0xda>
a1c01e30:	6b0a      	li	v1,10
a1c01e32:	e88a      	cmp	s0,a0
a1c01e34:	6017      	bteqz	a1c01e64 <$LCFI11+0x9a>
a1c01e36:	ec03      	sltu	a0,s0
a1c01e38:	6108      	btnez	a1c01e4a <$LCFI11+0x80>
a1c01e3a:	b41c      	lw	a0,a1c01ea8 <$LCFI11+0xde>
a1c01e3c:	6b08      	li	v1,8
a1c01e3e:	ec0e      	xor	a0,s0
a1c01e40:	2411      	beqz	a0,a1c01e64 <$LCFI11+0x9a>
a1c01e42:	b31b      	lw	v1,a1c01eac <$LCFI11+0xe2>
a1c01e44:	e86e      	xor	s0,v1
a1c01e46:	6b09      	li	v1,9
a1c01e48:	100b      	b	a1c01e60 <$LCFI11+0x96>
a1c01e4a:	b51a      	lw	a1,a1c01eb0 <$LCFI11+0xe6>
a1c01e4c:	6b0c      	li	v1,12
a1c01e4e:	ed0e      	xor	a1,s0
a1c01e50:	2509      	beqz	a1,a1c01e64 <$LCFI11+0x9a>
a1c01e52:	b619      	lw	a2,a1c01eb4 <$LCFI11+0xea>
a1c01e54:	6b0d      	li	v1,13
a1c01e56:	ee0e      	xor	a2,s0
a1c01e58:	2605      	beqz	a2,a1c01e64 <$LCFI11+0x9a>
a1c01e5a:	b718      	lw	a3,a1c01eb8 <$LCFI11+0xee>
a1c01e5c:	6b0b      	li	v1,11
a1c01e5e:	e8ee      	xor	s0,a3
a1c01e60:	2001      	beqz	s0,a1c01e64 <$LCFI11+0x9a>
a1c01e62:	6b00      	li	v1,0
a1c01e64:	b007      	lw	s0,a1c01e80 <$LCFI11+0xb6>
a1c01e66:	6782      	move	a0,v0
a1c01e68:	f0a0 d86c 	sw	v1,172(s0)
a1c01e6c:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01e70:	6500      	nop
a1c01e72:	9707      	lw	a3,28(sp)
a1c01e74:	6751      	move	v0,s1
a1c01e76:	9106      	lw	s1,24(sp)
a1c01e78:	9005      	lw	s0,20(sp)
a1c01e7a:	ef00      	jr	a3
a1c01e7c:	6304      	addiu	sp,32
a1c01e7e:	6500      	nop
a1c01e80:	0000      	addiu	s0,sp,0
a1c01e82:	a1a0      	lbu	a1,0(s1)
a1c01e84:	26a4      	beqz	a2,a1c01dce <$LCFI11+0x4>
a1c01e86:	a1c0      	lbu	a2,0(s1)
a1c01e88:	4c00      	addiu	a0,0
a1c01e8a:	0770      	addiu	a3,sp,448
a1c01e8c:	2f80      	bnez	a3,a1c01d8e <$LCFI7+0xa>
a1c01e8e:	04a6      	addiu	a0,sp,664
a1c01e90:	17c0      	b	a1c01e12 <$LCFI11+0x48>
a1c01e92:	0253      	addiu	v0,sp,332
a1c01e94:	7500      	cmpi	a1,0
a1c01e96:	0319      	addiu	v1,sp,100
a1c01e98:	ea00      	jr	v0
a1c01e9a:	0632      	addiu	a2,sp,200
a1c01e9c:	2dd1      	bnez	a1,a1c01e40 <$LCFI11+0x76>
a1c01e9e:	06c3      	addiu	a2,sp,780
a1c01ea0:	3649      	dsll	a2,v0,2
a1c01ea2:	0550      	addiu	a1,sp,320
a1c01ea4:	6c92      	li	a0,146
a1c01ea6:	0aa0      	la	v0,a1c02124 <$LCFI52+0x5a>
a1c01ea8:	e2ab      	subu	v0,a1
a1c01eaa:	0843      	la	s0,a1c01fb4 <$LCFI28+0x66>
a1c01eac:	5f00      	sltiu	a3,0
a1c01eae:	094c      	la	s1,a1c01fdc <$LCFI28+0x8e>
a1c01eb0:	9800      	lw	s0,0(s0)
a1c01eb2:	0ee0      	la	a2,a1c02230 <$LCFI52+0x166>
a1c01eb4:	be00      	lwu	s0,0(a2)
a1c01eb6:	1298      	b	a1c023e8 <$LCFI23+0xe>
a1c01eb8:	d400      	sw	a0,0(sp)
a1c01eba:	0c65      	la	a0,a1c0204c <$LCFI32+0x60>

a1c01ebc <hal_SysSoftReset>:
a1c01ebc:	63fd      	addiu	sp,-24

a1c01ebe <$LCFI18>:
a1c01ebe:	6205      	sw	ra,20(sp)
a1c01ec0:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c01ec4:	6500      	nop
a1c01ec6:	b204      	lw	v0,a1c01ed4 <$LCFI18+0x16>
a1c01ec8:	b404      	lw	a0,a1c01ed8 <$LCFI18+0x1a>
a1c01eca:	b305      	lw	v1,a1c01edc <$LCFI18+0x1e>
a1c01ecc:	da80      	sw	a0,0(v0)
a1c01ece:	da61      	sw	v1,4(v0)
a1c01ed0:	17ff      	b	a1c01ed0 <$LCFI18+0x12>
a1c01ed2:	6500      	nop
a1c01ed4:	0000      	addiu	s0,sp,0
a1c01ed6:	a1a0      	lbu	a1,0(s1)
a1c01ed8:	0001      	addiu	s0,sp,4
a1c01eda:	00a5      	addiu	s0,sp,660
a1c01edc:	0000      	addiu	s0,sp,0
a1c01ede:	8000      	lb	s0,0(s0)

a1c01ee0 <hal_SysChangeSystemClockByOneStep>:
a1c01ee0:	63fc      	addiu	sp,-32

a1c01ee2 <$LCFI20>:
a1c01ee2:	0505      	addiu	a1,sp,20
a1c01ee4:	d006      	sw	s0,24(sp)
a1c01ee6:	6207      	sw	ra,28(sp)
a1c01ee8:	1a03 071e 	jal	a1c01c78 <hal_SysGetFrequencyConfiguration>
a1c01eec:	0604      	addiu	a2,sp,16
a1c01eee:	6702      	move	s0,v0
a1c01ef0:	b213      	lw	v0,a1c01f3c <$LCFI20+0x5a>
a1c01ef2:	9a40      	lw	v0,0(v0)
a1c01ef4:	ea0a      	cmp	v0,s0
a1c01ef6:	601d      	bteqz	a1c01f32 <$LCFI20+0x50>
a1c01ef8:	ea03      	sltu	v0,s0
a1c01efa:	6003      	bteqz	a1c01f02 <$LCFI20+0x20>
a1c01efc:	1a03 070e 	jal	a1c01c38 <hal_SysCallFreqChangeHandlers>
a1c01f00:	6790      	move	a0,s0
a1c01f02:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c01f06:	6500      	nop
a1c01f08:	b60e      	lw	a2,a1c01f40 <$LCFI20+0x5e>
a1c01f0a:	b50f      	lw	a1,a1c01f44 <$LCFI20+0x62>
a1c01f0c:	6c0f      	li	a0,15
a1c01f0e:	dea0      	sw	a1,0(a2)
a1c01f10:	9705      	lw	a3,20(sp)
a1c01f12:	6b10      	li	v1,16
a1c01f14:	ef8c      	and	a3,a0
a1c01f16:	b50d      	lw	a1,a1c01f48 <$LCFI20+0x66>
a1c01f18:	ef6d      	or	a3,v1
a1c01f1a:	def3      	sw	a3,76(a2)
a1c01f1c:	6782      	move	a0,v0
a1c01f1e:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c01f22:	dea0      	sw	a1,0(a2)
a1c01f24:	b406      	lw	a0,a1c01f3c <$LCFI20+0x5a>
a1c01f26:	9c60      	lw	v1,0(a0)
a1c01f28:	e863      	sltu	s0,v1
a1c01f2a:	6003      	bteqz	a1c01f32 <$LCFI20+0x50>
a1c01f2c:	1a03 070e 	jal	a1c01c38 <hal_SysCallFreqChangeHandlers>
a1c01f30:	6790      	move	a0,s0
a1c01f32:	9707      	lw	a3,28(sp)
a1c01f34:	9006      	lw	s0,24(sp)
a1c01f36:	ef00      	jr	a3
a1c01f38:	6304      	addiu	sp,32
a1c01f3a:	6500      	nop
a1c01f3c:	2798      	beqz	a3,a1c01e6e <$LCFI11+0xa4>
a1c01f3e:	a1c0      	lbu	a2,0(s1)
a1c01f40:	0000      	addiu	s0,sp,0
a1c01f42:	a1a0      	lbu	a1,0(s1)
a1c01f44:	0001      	addiu	s0,sp,4
a1c01f46:	00a5      	addiu	s0,sp,660
a1c01f48:	0000      	addiu	s0,sp,0
a1c01f4a:	00a5      	addiu	s0,sp,660

a1c01f4c <hal_SysFSF2SF>:
a1c01f4c:	63fd      	addiu	sp,-24

a1c01f4e <$LCFI28>:
a1c01f4e:	5c0c      	sltiu	a0,12
a1c01f50:	6205      	sw	ra,20(sp)
a1c01f52:	67a4      	move	a1,a0
a1c01f54:	602a      	bteqz	a1c01faa <$LCFI28+0x5c>
a1c01f56:	0c04      	la	a0,a1c01f64 <$LCFI28+0x16>
a1c01f58:	37a4      	sll	a3,a1,1
a1c01f5a:	e4f9      	addu	a2,a0,a3
a1c01f5c:	8e60      	lh	v1,0(a2)
a1c01f5e:	e469      	addu	v0,a0,v1
a1c01f60:	ea00      	jr	v0
a1c01f62:	6500      	nop
a1c01f64:	001d      	addiu	s0,sp,116
a1c01f66:	0019      	addiu	s0,sp,100
a1c01f68:	0023      	addiu	s0,sp,140
a1c01f6a:	0027      	addiu	s0,sp,156
a1c01f6c:	002b      	addiu	s0,sp,172
a1c01f6e:	002f      	addiu	s0,sp,188
a1c01f70:	0033      	addiu	s0,sp,204
a1c01f72:	0047      	addiu	s0,sp,284
a1c01f74:	0037      	addiu	s0,sp,220
a1c01f76:	003b      	addiu	s0,sp,236
a1c01f78:	003f      	addiu	s0,sp,252
a1c01f7a:	0043      	addiu	s0,sp,268
a1c01f7c:	b210      	lw	v0,a1c01fbc <$LCFI28+0x6e>
a1c01f7e:	101a      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f80:	f010 6a00 	li	v0,32768
a1c01f84:	1017      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f86:	b20f      	lw	v0,a1c01fc0 <$LCFI28+0x72>
a1c01f88:	1015      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f8a:	b20f      	lw	v0,a1c01fc4 <$LCFI28+0x76>
a1c01f8c:	1013      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f8e:	b20f      	lw	v0,a1c01fc8 <$LCFI28+0x7a>
a1c01f90:	1011      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f92:	b20f      	lw	v0,a1c01fcc <$LCFI28+0x7e>
a1c01f94:	100f      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f96:	b20f      	lw	v0,a1c01fd0 <$LCFI28+0x82>
a1c01f98:	100d      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f9a:	b20f      	lw	v0,a1c01fd4 <$LCFI28+0x86>
a1c01f9c:	100b      	b	a1c01fb4 <$LCFI28+0x66>
a1c01f9e:	b20f      	lw	v0,a1c01fd8 <$LCFI28+0x8a>
a1c01fa0:	1009      	b	a1c01fb4 <$LCFI28+0x66>
a1c01fa2:	b20f      	lw	v0,a1c01fdc <$LCFI28+0x8e>
a1c01fa4:	1007      	b	a1c01fb4 <$LCFI28+0x66>
a1c01fa6:	b20f      	lw	v0,a1c01fe0 <$LCFI28+0x92>
a1c01fa8:	1005      	b	a1c01fb4 <$LCFI28+0x66>
a1c01faa:	b40f      	lw	a0,a1c01fe4 <$LCFI28+0x96>
a1c01fac:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c01fb0:	6500      	nop
a1c01fb2:	6a00      	li	v0,0
a1c01fb4:	9705      	lw	a3,20(sp)
a1c01fb6:	ef00      	jr	a3
a1c01fb8:	6303      	addiu	sp,24
a1c01fba:	6500      	nop
a1c01fbc:	5d40      	sltiu	a1,64
a1c01fbe:	00c6      	addiu	s0,sp,792
a1c01fc0:	ba80      	lwu	a0,0(v0)
a1c01fc2:	018c      	addiu	s1,sp,560
a1c01fc4:	17c0      	b	a1c01f46 <$LCFI20+0x64>
a1c01fc6:	0253      	addiu	v0,sp,332
a1c01fc8:	7500      	cmpi	a1,0
a1c01fca:	0319      	addiu	v1,sp,100
a1c01fcc:	2f80      	bnez	a3,a1c01ece <$LCFI18+0x10>
a1c01fce:	04a6      	addiu	a0,sp,664
a1c01fd0:	ea00      	jr	v0
a1c01fd2:	0632      	addiu	a2,sp,200
a1c01fd4:	5f00      	sltiu	a3,0
a1c01fd6:	094c      	la	s1,a1c02104 <$LCFI52+0x3a>
a1c01fd8:	d400      	sw	a0,0(sp)
a1c01fda:	0c65      	la	a0,a1c0216c <$LCFI52+0xa2>
a1c01fdc:	9800      	lw	s0,0(s0)
a1c01fde:	0ee0      	la	a2,a1c0235c <$LCFI19+0x26>
a1c01fe0:	be00      	lwu	s0,0(a2)
a1c01fe2:	1298      	b	a1c02514 <g_pmdCalibSetting2>
a1c01fe4:	25d4      	beqz	a1,a1c01f8e <$LCFI28+0x40>
a1c01fe6:	a1c0      	lbu	a2,0(s1)

a1c01fe8 <hal_SysSF2FSF>:
a1c01fe8:	b31f      	lw	v1,a1c02064 <$LCFI32+0x78>
a1c01fea:	63fd      	addiu	sp,-24

a1c01fec <$LCFI32>:
a1c01fec:	ec6a      	cmp	a0,v1
a1c01fee:	6205      	sw	ra,20(sp)
a1c01ff0:	67a4      	move	a1,a0
a1c01ff2:	6a05      	li	v0,5
a1c01ff4:	6033      	bteqz	a1c0205c <$LCFI32+0x70>
a1c01ff6:	eb83      	sltu	v1,a0
a1c01ff8:	6116      	btnez	a1c02026 <$LCFI32+0x3a>
a1c01ffa:	b31c      	lw	v1,a1c02068 <$LCFI32+0x7c>
a1c01ffc:	6a02      	li	v0,2
a1c01ffe:	ec6a      	cmp	a0,v1
a1c02000:	602d      	bteqz	a1c0205c <$LCFI32+0x70>
a1c02002:	eb83      	sltu	v1,a0
a1c02004:	6108      	btnez	a1c02016 <$LCFI32+0x2a>
a1c02006:	f010 7400 	cmpi	a0,32768
a1c0200a:	6a00      	li	v0,0
a1c0200c:	6027      	bteqz	a1c0205c <$LCFI32+0x70>
a1c0200e:	b318      	lw	v1,a1c0206c <$LCFI32+0x80>
a1c02010:	6a01      	li	v0,1
a1c02012:	eb8e      	xor	v1,a0
a1c02014:	101d      	b	a1c02050 <$LCFI32+0x64>
a1c02016:	b317      	lw	v1,a1c02070 <$LCFI32+0x84>
a1c02018:	6a03      	li	v0,3
a1c0201a:	eb8e      	xor	v1,a0
a1c0201c:	231f      	beqz	v1,a1c0205c <$LCFI32+0x70>
a1c0201e:	b316      	lw	v1,a1c02074 <$LCFI32+0x88>
a1c02020:	6a04      	li	v0,4
a1c02022:	eb8e      	xor	v1,a0
a1c02024:	1015      	b	a1c02050 <$LCFI32+0x64>
a1c02026:	b315      	lw	v1,a1c02078 <$LCFI32+0x8c>
a1c02028:	6a09      	li	v0,9
a1c0202a:	ec6a      	cmp	a0,v1
a1c0202c:	6017      	bteqz	a1c0205c <$LCFI32+0x70>
a1c0202e:	eb83      	sltu	v1,a0
a1c02030:	6108      	btnez	a1c02042 <$LCFI32+0x56>
a1c02032:	b413      	lw	a0,a1c0207c <$LCFI32+0x90>
a1c02034:	6a06      	li	v0,6
a1c02036:	ecae      	xor	a0,a1
a1c02038:	2411      	beqz	a0,a1c0205c <$LCFI32+0x70>
a1c0203a:	b312      	lw	v1,a1c02080 <$LCFI32+0x94>
a1c0203c:	6a08      	li	v0,8
a1c0203e:	ebae      	xor	v1,a1
a1c02040:	1007      	b	a1c02050 <$LCFI32+0x64>
a1c02042:	b611      	lw	a2,a1c02084 <$LCFI32+0x98>
a1c02044:	6a0a      	li	v0,10
a1c02046:	ee8e      	xor	a2,a0
a1c02048:	2609      	beqz	a2,a1c0205c <$LCFI32+0x70>
a1c0204a:	b310      	lw	v1,a1c02088 <$LCFI32+0x9c>
a1c0204c:	6a0b      	li	v0,11
a1c0204e:	eb8e      	xor	v1,a0
a1c02050:	2305      	beqz	v1,a1c0205c <$LCFI32+0x70>
a1c02052:	b40f      	lw	a0,a1c0208c <$LCFI32+0xa0>
a1c02054:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c02058:	6500      	nop
a1c0205a:	6a00      	li	v0,0
a1c0205c:	9705      	lw	a3,20(sp)
a1c0205e:	ef00      	jr	a3
a1c02060:	6303      	addiu	sp,24
a1c02062:	6500      	nop
a1c02064:	2f80      	bnez	a3,a1c01f66 <$LCFI28+0x18>
a1c02066:	04a6      	addiu	a0,sp,664
a1c02068:	ba80      	lwu	a0,0(v0)
a1c0206a:	018c      	addiu	s1,sp,560
a1c0206c:	5d40      	sltiu	a1,64
a1c0206e:	00c6      	addiu	s0,sp,792
a1c02070:	17c0      	b	a1c01ff2 <$LCFI32+0x6>
a1c02072:	0253      	addiu	v0,sp,332
a1c02074:	7500      	cmpi	a1,0
a1c02076:	0319      	addiu	v1,sp,100
a1c02078:	d400      	sw	a0,0(sp)
a1c0207a:	0c65      	la	a0,a1c0220c <$LCFI52+0x142>
a1c0207c:	ea00      	jr	v0
a1c0207e:	0632      	addiu	a2,sp,200
a1c02080:	5f00      	sltiu	a3,0
a1c02082:	094c      	la	s1,a1c021b0 <$LCFI52+0xe6>
a1c02084:	9800      	lw	s0,0(s0)
a1c02086:	0ee0      	la	a2,a1c02404 <$LCFI23+0x2a>
a1c02088:	be00      	lwu	s0,0(a2)
a1c0208a:	1298      	b	a1c025bc <CSWTCH.2+0xc>
a1c0208c:	25fc      	beqz	a1,a1c02086 <$LCFI32+0x9a>
a1c0208e:	a1c0      	lbu	a2,0(s1)

a1c02090 <hal_SysRestart>:
a1c02090:	63fd      	addiu	sp,-24

a1c02092 <$LCFI46>:
a1c02092:	d004      	sw	s0,16(sp)
a1c02094:	6205      	sw	ra,20(sp)
a1c02096:	1b03 0d76 	jal	a1e035d8 <hal_TimGetUpTime>
a1c0209a:	6500      	nop
a1c0209c:	6c01      	li	a0,1
a1c0209e:	1a03 092b 	jal	a1c024ac <hal_UsbReset>
a1c020a2:	6702      	move	s0,v0
a1c020a4:	1b03 0d76 	jal	a1e035d8 <hal_TimGetUpTime>
a1c020a8:	6500      	nop
a1c020aa:	f743 4060 	addiu	v1,s0,8000
a1c020ae:	ea63      	sltu	v0,v1
a1c020b0:	61f9      	btnez	a1c020a4 <$LCFI46+0x12>
a1c020b2:	1a03 092b 	jal	a1c024ac <hal_UsbReset>
a1c020b6:	6c00      	li	a0,0
a1c020b8:	1a03 07af 	jal	a1c01ebc <hal_SysSoftReset>
a1c020bc:	6500      	nop
a1c020be:	9705      	lw	a3,20(sp)
a1c020c0:	9004      	lw	s0,16(sp)
a1c020c2:	ef00      	jr	a3
a1c020c4:	6303      	addiu	sp,24
a1c020c6:	6500      	nop

a1c020c8 <hal_SysUpdateSystemFrequency>:
a1c020c8:	63fa      	addiu	sp,-48

a1c020ca <$LCFI52>:
a1c020ca:	620b      	sw	ra,44(sp)
a1c020cc:	d10a      	sw	s1,40(sp)
a1c020ce:	d009      	sw	s0,36(sp)
a1c020d0:	6c0b      	li	a0,11
a1c020d2:	1001      	b	a1c020d6 <$LCFI52+0xc>
a1c020d4:	4cff      	addiu	a0,-1
a1c020d6:	b557      	lw	a1,a1c02230 <$LCFI52+0x166>
a1c020d8:	e4ad      	addu	v1,a0,a1
a1c020da:	a340      	lbu	v0,0(v1)
a1c020dc:	2a01      	bnez	v0,a1c020e0 <$LCFI52+0x16>
a1c020de:	2cfa      	bnez	a0,a1c020d4 <$LCFI52+0xa>
a1c020e0:	1a03 07d3 	jal	a1c01f4c <hal_SysFSF2SF>
a1c020e4:	6500      	nop
a1c020e6:	6782      	move	a0,v0
a1c020e8:	0505      	addiu	a1,sp,20
a1c020ea:	b153      	lw	s1,a1c02234 <$LCFI52+0x16a>
a1c020ec:	1a03 071e 	jal	a1c01c78 <hal_SysGetFrequencyConfiguration>
a1c020f0:	0604      	addiu	a2,sp,16
a1c020f2:	9980      	lw	a0,0(s1)
a1c020f4:	6702      	move	s0,v0
a1c020f6:	ec4e      	xor	a0,v0
a1c020f8:	f080 2414 	beqz	a0,a1c02224 <$LCFI52+0x15a>
a1c020fc:	1a03 0933 	jal	a1c024cc <hal_LpsClockSysChangeAllowed>
a1c02100:	6782      	move	a0,v0
a1c02102:	f080 220f 	beqz	v0,a1c02224 <$LCFI52+0x15a>
a1c02106:	99c0      	lw	a2,0(s1)
a1c02108:	b24c      	lw	v0,a1c02238 <$LCFI52+0x16e>
a1c0210a:	eac3      	sltu	v0,a2
a1c0210c:	6106      	btnez	a1c0211a <$LCFI52+0x50>
a1c0210e:	ea03      	sltu	v0,s0
a1c02110:	6004      	bteqz	a1c0211a <$LCFI52+0x50>
a1c02112:	6c00      	li	a0,0
a1c02114:	1a03 0569 	jal	a1c015a4 <pmd_SetCoreVoltage>
a1c02118:	6d02      	li	a1,2
a1c0211a:	b747      	lw	a3,a1c02234 <$LCFI52+0x16a>
a1c0211c:	9f60      	lw	v1,0(a3)
a1c0211e:	e863      	sltu	s0,v1
a1c02120:	6103      	btnez	a1c02128 <$LCFI52+0x5e>
a1c02122:	b447      	lw	a0,a1c0223c <$LCFI52+0x172>
a1c02124:	6a00      	li	v0,0
a1c02126:	1038      	b	a1c02198 <$LCFI52+0xce>
a1c02128:	b245      	lw	v0,a1c0223c <$LCFI52+0x172>
a1c0212a:	6900      	li	s1,0
a1c0212c:	9aa0      	lw	a1,0(v0)
a1c0212e:	4a04      	addiu	v0,4
a1c02130:	e8a3      	sltu	s0,a1
a1c02132:	6103      	btnez	a1c0213a <$LCFI52+0x70>
a1c02134:	4901      	addiu	s1,1
a1c02136:	7106      	cmpi	s1,6
a1c02138:	61f9      	btnez	a1c0212c <$LCFI52+0x62>
a1c0213a:	b441      	lw	a0,a1c0223c <$LCFI52+0x172>
a1c0213c:	3728      	sll	a3,s1,2
a1c0213e:	e4f9      	addu	a2,a0,a3
a1c02140:	9e40      	lw	v0,0(a2)
a1c02142:	ea63      	sltu	v0,v1
a1c02144:	605f      	bteqz	a1c02204 <$LCFI52+0x13a>
a1c02146:	4141      	addiu	v0,s1,1
a1c02148:	3548      	sll	a1,v0,2
a1c0214a:	e591      	addu	a0,a1,a0
a1c0214c:	100c      	b	a1c02166 <$LCFI52+0x9c>
a1c0214e:	9ca0      	lw	a1,0(a0)
a1c02150:	4c04      	addiu	a0,4
a1c02152:	ed63      	sltu	a1,v1
a1c02154:	6107      	btnez	a1c02164 <$LCFI52+0x9a>
a1c02156:	ebae      	xor	v1,a1
a1c02158:	2b08      	bnez	v1,a1c0216a <$LCFI52+0xa0>
a1c0215a:	428f      	addiu	a0,v0,-1
a1c0215c:	ec2e      	xor	a0,s1
a1c0215e:	6b01      	li	v1,1
a1c02160:	2c04      	bnez	a0,a1c0216a <$LCFI52+0xa0>
a1c02162:	1004      	b	a1c0216c <$LCFI52+0xa2>
a1c02164:	4a01      	addiu	v0,1
a1c02166:	5a06      	sltiu	v0,6
a1c02168:	61f2      	btnez	a1c0214e <$LCFI52+0x84>
a1c0216a:	6b00      	li	v1,0
a1c0216c:	4aff      	addiu	v0,-1
a1c0216e:	e942      	slt	s1,v0
a1c02170:	6101      	btnez	a1c02174 <$LCFI52+0xaa>
a1c02172:	2348      	beqz	v1,a1c02204 <$LCFI52+0x13a>
a1c02174:	3648      	sll	a2,v0,2
a1c02176:	b332      	lw	v1,a1c0223c <$LCFI52+0x172>
a1c02178:	e66d      	addu	v1,a2,v1
a1c0217a:	1009      	b	a1c0218e <$LCFI52+0xc4>
a1c0217c:	9b80      	lw	a0,0(v1)
a1c0217e:	d206      	sw	v0,24(sp)
a1c02180:	1a03 07b8 	jal	a1c01ee0 <hal_SysChangeSystemClockByOneStep>
a1c02184:	d307      	sw	v1,28(sp)
a1c02186:	9206      	lw	v0,24(sp)
a1c02188:	9307      	lw	v1,28(sp)
a1c0218a:	4aff      	addiu	v0,-1
a1c0218c:	4bfc      	addiu	v1,-4
a1c0218e:	ea22      	slt	v0,s1
a1c02190:	6139      	btnez	a1c02204 <$LCFI52+0x13a>
a1c02192:	5a06      	sltiu	v0,6
a1c02194:	61f3      	btnez	a1c0217c <$LCFI52+0xb2>
a1c02196:	1036      	b	a1c02204 <$LCFI52+0x13a>
a1c02198:	9c20      	lw	s1,0(a0)
a1c0219a:	4c04      	addiu	a0,4
a1c0219c:	eb23      	sltu	v1,s1
a1c0219e:	6103      	btnez	a1c021a6 <$LCFI52+0xdc>
a1c021a0:	4a01      	addiu	v0,1
a1c021a2:	7206      	cmpi	v0,6
a1c021a4:	61f9      	btnez	a1c02198 <$LCFI52+0xce>
a1c021a6:	b426      	lw	a0,a1c0223c <$LCFI52+0x172>
a1c021a8:	3348      	sll	v1,v0,2
a1c021aa:	e475      	addu	a1,a0,v1
a1c021ac:	9de0      	lw	a3,0(a1)
a1c021ae:	6722      	move	s1,v0
a1c021b0:	ef03      	sltu	a3,s0
a1c021b2:	6028      	bteqz	a1c02204 <$LCFI52+0x13a>
a1c021b4:	4261      	addiu	v1,v0,1
a1c021b6:	3668      	sll	a2,v1,2
a1c021b8:	e691      	addu	a0,a2,a0
a1c021ba:	100c      	b	a1c021d4 <$LCFI52+0x10a>
a1c021bc:	9ca0      	lw	a1,0(a0)
a1c021be:	4c04      	addiu	a0,4
a1c021c0:	ed03      	sltu	a1,s0
a1c021c2:	6107      	btnez	a1c021d2 <$LCFI52+0x108>
a1c021c4:	ed0e      	xor	a1,s0
a1c021c6:	2d08      	bnez	a1,a1c021d8 <$LCFI52+0x10e>
a1c021c8:	43ef      	addiu	a3,v1,-1
a1c021ca:	ef4e      	xor	a3,v0
a1c021cc:	6c01      	li	a0,1
a1c021ce:	2f04      	bnez	a3,a1c021d8 <$LCFI52+0x10e>
a1c021d0:	1004      	b	a1c021da <$LCFI52+0x110>
a1c021d2:	4b01      	addiu	v1,1
a1c021d4:	5b06      	sltiu	v1,6
a1c021d6:	61f2      	btnez	a1c021bc <$LCFI52+0xf2>
a1c021d8:	6c00      	li	a0,0
a1c021da:	4bff      	addiu	v1,-1
a1c021dc:	ea62      	slt	v0,v1
a1c021de:	6101      	btnez	a1c021e2 <$LCFI52+0x118>
a1c021e0:	2411      	beqz	a0,a1c02204 <$LCFI52+0x13a>
a1c021e2:	3248      	sll	v0,2
a1c021e4:	b416      	lw	a0,a1c0223c <$LCFI52+0x172>
a1c021e6:	e289      	addu	v0,a0
a1c021e8:	1009      	b	a1c021fc <$LCFI52+0x132>
a1c021ea:	9a80      	lw	a0,0(v0)
a1c021ec:	d206      	sw	v0,24(sp)
a1c021ee:	1a03 07b8 	jal	a1c01ee0 <hal_SysChangeSystemClockByOneStep>
a1c021f2:	d307      	sw	v1,28(sp)
a1c021f4:	9206      	lw	v0,24(sp)
a1c021f6:	9307      	lw	v1,28(sp)
a1c021f8:	4901      	addiu	s1,1
a1c021fa:	4a04      	addiu	v0,4
a1c021fc:	eb22      	slt	v1,s1
a1c021fe:	6102      	btnez	a1c02204 <$LCFI52+0x13a>
a1c02200:	5906      	sltiu	s1,6
a1c02202:	61f3      	btnez	a1c021ea <$LCFI52+0x120>
a1c02204:	1a03 07b8 	jal	a1c01ee0 <hal_SysChangeSystemClockByOneStep>
a1c02208:	6790      	move	a0,s0
a1c0220a:	b50b      	lw	a1,a1c02234 <$LCFI52+0x16a>
a1c0220c:	9d20      	lw	s1,0(a1)
a1c0220e:	b20b      	lw	v0,a1c02238 <$LCFI52+0x16e>
a1c02210:	ea23      	sltu	v0,s1
a1c02212:	6006      	bteqz	a1c02220 <$LCFI52+0x156>
a1c02214:	ea03      	sltu	v0,s0
a1c02216:	6104      	btnez	a1c02220 <$LCFI52+0x156>
a1c02218:	6c00      	li	a0,0
a1c0221a:	1a03 0569 	jal	a1c015a4 <pmd_SetCoreVoltage>
a1c0221e:	67a4      	move	a1,a0
a1c02220:	b305      	lw	v1,a1c02234 <$LCFI52+0x16a>
a1c02222:	db00      	sw	s0,0(v1)
a1c02224:	970b      	lw	a3,44(sp)
a1c02226:	910a      	lw	s1,40(sp)
a1c02228:	9009      	lw	s0,36(sp)
a1c0222a:	ef00      	jr	a3
a1c0222c:	6306      	addiu	sp,48
a1c0222e:	6500      	nop
a1c02230:	8ca0      	lh	a1,0(a0)
a1c02232:	a1c0      	lbu	a2,0(s1)
a1c02234:	2798      	beqz	a3,a1c02166 <$LCFI52+0x9c>
a1c02236:	a1c0      	lbu	a2,0(s1)
a1c02238:	ea00      	jr	v0
a1c0223a:	0632      	addiu	a2,sp,200
a1c0223c:	268c      	beqz	a2,a1c02156 <$LCFI52+0x8c>
a1c0223e:	a1c0      	lbu	a2,0(s1)

a1c02240 <hal_SysRequestFreq>:
a1c02240:	63fb      	addiu	sp,-40

a1c02242 <$LCFI56>:
a1c02242:	d108      	sw	s1,32(sp)
a1c02244:	d007      	sw	s0,28(sp)
a1c02246:	d504      	sw	a1,16(sp)
a1c02248:	6704      	move	s0,a0
a1c0224a:	6209      	sw	ra,36(sp)
a1c0224c:	1a03 075d 	jal	a1c01d74 <hal_SysEnterCriticalSection>
a1c02250:	d60c      	sw	a2,48(sp)
a1c02252:	6722      	move	s1,v0
a1c02254:	9504      	lw	a1,16(sp)
a1c02256:	b213      	lw	v0,a1c022a0 <$LCFI56+0x5e>
a1c02258:	6785      	move	a0,a1
a1c0225a:	eaa3      	sltu	v0,a1
a1c0225c:	6001      	bteqz	a1c02260 <$LCFI56+0x1e>
a1c0225e:	6782      	move	a0,v0
a1c02260:	1a03 07fa 	jal	a1c01fe8 <hal_SysSF2FSF>
a1c02264:	6500      	nop
a1c02266:	b310      	lw	v1,a1c022a4 <$LCFI56+0x62>
a1c02268:	e07d      	addu	a3,s0,v1
a1c0226a:	a7a0      	lbu	a1,0(a3)
a1c0226c:	b40f      	lw	a0,a1c022a8 <$LCFI56+0x66>
a1c0226e:	e4b5      	addu	a1,a0,a1
a1c02270:	a5c0      	lbu	a2,0(a1)
a1c02272:	3308      	sll	v1,s0,2
a1c02274:	4eff      	addiu	a2,-1
a1c02276:	c5c0      	sb	a2,0(a1)
a1c02278:	e459      	addu	a2,a0,v0
a1c0227a:	a6a0      	lbu	a1,0(a2)
a1c0227c:	b40c      	lw	a0,a1c022ac <$LCFI56+0x6a>
a1c0227e:	4d01      	addiu	a1,1
a1c02280:	c6a0      	sb	a1,0(a2)
a1c02282:	c740      	sb	v0,0(a3)
a1c02284:	920c      	lw	v0,48(sp)
a1c02286:	e461      	addu	s0,a0,v1
a1c02288:	1a03 0832 	jal	a1c020c8 <hal_SysUpdateSystemFrequency>
a1c0228c:	d840      	sw	v0,0(s0)
a1c0228e:	1a03 075a 	jal	a1c01d68 <hal_SysExitCriticalSection>
a1c02292:	6791      	move	a0,s1
a1c02294:	9709      	lw	a3,36(sp)
a1c02296:	9108      	lw	s1,32(sp)
a1c02298:	9007      	lw	s0,28(sp)
a1c0229a:	ef00      	jr	a3
a1c0229c:	6305      	addiu	sp,40
a1c0229e:	6500      	nop
a1c022a0:	ea00      	jr	v0
a1c022a2:	0632      	addiu	a2,sp,200
a1c022a4:	8c78      	lh	v1,48(a0)
a1c022a6:	a1c0      	lbu	a2,0(s1)
a1c022a8:	8ca0      	lh	a1,0(a0)
a1c022aa:	a1c0      	lbu	a2,0(s1)
a1c022ac:	89d8      	lh	a2,48(s1)
a1c022ae:	a1c0      	lbu	a2,0(s1)

a1c022b0 <hal_UsbResetTransfert>:
a1c022b0:	b208      	lw	v0,a1c022d0 <hal_UsbResetTransfert+0x20>
a1c022b2:	b409      	lw	a0,a1c022d4 <hal_UsbResetTransfert+0x24>
a1c022b4:	6e01      	li	a2,1
a1c022b6:	6b00      	li	v1,0
a1c022b8:	6f01      	li	a3,1
a1c022ba:	edcb      	neg	a1,a2
a1c022bc:	c2e0      	sb	a3,0(v0)
a1c022be:	daa1      	sw	a1,4(v0)
a1c022c0:	da62      	sw	v1,8(v0)
a1c022c2:	da63      	sw	v1,12(v0)
a1c022c4:	4a10      	addiu	v0,16
a1c022c6:	ea8a      	cmp	v0,a0
a1c022c8:	61f5      	btnez	a1c022b4 <hal_UsbResetTransfert+0x4>
a1c022ca:	e820      	jr	ra
a1c022cc:	6500      	nop
a1c022ce:	6500      	nop
a1c022d0:	27e8      	beqz	a3,a1c022a2 <$LCFI56+0x60>
a1c022d2:	a1c0      	lbu	a2,0(s1)
a1c022d4:	2848      	bnez	s0,a1c02366 <$LCFI19+0x30>
a1c022d6:	a1c0      	lbu	a2,0(s1)

a1c022d8 <hal_UsbDisableEp>:
a1c022d8:	63fc      	addiu	sp,-32

a1c022da <$LCFI15>:
a1c022da:	d106      	sw	s1,24(sp)
a1c022dc:	69ff      	li	s1,255
a1c022de:	d005      	sw	s0,20(sp)
a1c022e0:	e98c      	and	s1,a0
a1c022e2:	687f      	li	s0,127
a1c022e4:	e82c      	and	s0,s1
a1c022e6:	5810      	sltiu	s0,16
a1c022e8:	6207      	sw	ra,28(sp)
a1c022ea:	6104      	btnez	a1c022f4 <$LCFI15+0x1a>
a1c022ec:	b410      	lw	a0,a1c0232c <$LCFI15+0x52>
a1c022ee:	1a03 041e 	jal	a1c01078 <hal_DbgAssert>
a1c022f2:	6500      	nop
a1c022f4:	313e      	srl	s1,7
a1c022f6:	b30f      	lw	v1,a1c02330 <$LCFI15+0x56>
a1c022f8:	4044      	addiu	v0,s0,4
a1c022fa:	2902      	bnez	s1,a1c02300 <$LCFI15+0x26>
a1c022fc:	4047      	addiu	v0,s0,7
a1c022fe:	4a01      	addiu	v0,1
a1c02300:	3550      	sll	a1,v0,4
a1c02302:	e3a9      	addu	v0,v1,a1
a1c02304:	6c01      	li	a0,1
a1c02306:	6b00      	li	v1,0
a1c02308:	c28c      	sb	a0,12(v0)
a1c0230a:	da64      	sw	v1,16(v0)
a1c0230c:	da65      	sw	v1,20(v0)
a1c0230e:	da66      	sw	v1,24(v0)
a1c02310:	2007      	beqz	s0,a1c02320 <$LCFI15+0x46>
a1c02312:	b708      	lw	a3,a1c02330 <$LCFI15+0x56>
a1c02314:	6901      	li	s1,1
a1c02316:	e824      	sllv	s1,s0
a1c02318:	9f0a      	lw	s0,40(a3)
a1c0231a:	ee2f      	not	a2,s1
a1c0231c:	ee0c      	and	a2,s0
a1c0231e:	dfca      	sw	a2,40(a3)
a1c02320:	9707      	lw	a3,28(sp)
a1c02322:	9106      	lw	s1,24(sp)
a1c02324:	9005      	lw	s0,20(sp)
a1c02326:	ef00      	jr	a3
a1c02328:	6304      	addiu	sp,32
a1c0232a:	6500      	nop
a1c0232c:	26dc      	beqz	a2,a1c022e6 <$LCFI15+0xc>
a1c0232e:	a1c0      	lbu	a2,0(s1)
a1c02330:	279c      	beqz	a3,a1c0226a <$LCFI56+0x28>
a1c02332:	a1c0      	lbu	a2,0(s1)

a1c02334 <hal_UsbClrConfig>:
a1c02334:	63fc      	addiu	sp,-32

a1c02336 <$LCFI19>:
a1c02336:	6207      	sw	ra,28(sp)
a1c02338:	d106      	sw	s1,24(sp)
a1c0233a:	d005      	sw	s0,20(sp)
a1c0233c:	b226      	lw	v0,a1c023d4 <$LCFI19+0x9e>
a1c0233e:	9a61      	lw	v1,4(v0)
a1c02340:	2327      	beqz	v1,a1c02390 <$LCFI19+0x5a>
a1c02342:	f0a0 a25f 	lbu	v0,191(v0)
a1c02346:	72ff      	cmpi	v0,255
a1c02348:	601e      	bteqz	a1c02386 <$LCFI19+0x50>
a1c0234a:	9b63      	lw	v1,12(v1)
a1c0234c:	3448      	sll	a0,v0,2
a1c0234e:	e389      	addu	v0,v1,a0
a1c02350:	9a40      	lw	v0,0(v0)
a1c02352:	2219      	beqz	v0,a1c02386 <$LCFI19+0x50>
a1c02354:	9aa2      	lw	a1,8(v0)
a1c02356:	6800      	li	s0,0
a1c02358:	2d09      	bnez	a1,a1c0236c <$LCFI19+0x36>
a1c0235a:	1015      	b	a1c02386 <$LCFI19+0x50>
a1c0235c:	9a43      	lw	v0,12(v0)
a1c0235e:	2203      	beqz	v0,a1c02366 <$LCFI19+0x30>
a1c02360:	6c05      	li	a0,5
a1c02362:	ea40      	jalr	v0
a1c02364:	6d00      	li	a1,0
a1c02366:	4801      	addiu	s0,1
a1c02368:	6eff      	li	a2,255
a1c0236a:	e8cc      	and	s0,a2
a1c0236c:	b41a      	lw	a0,a1c023d4 <$LCFI19+0x9e>
a1c0236e:	9c21      	lw	s1,4(a0)
a1c02370:	f0a0 a4ff 	lbu	a3,191(a0)
a1c02374:	9963      	lw	v1,12(s1)
a1c02376:	36e8      	sll	a2,a3,2
a1c02378:	e3d5      	addu	a1,v1,a2
a1c0237a:	9d40      	lw	v0,0(a1)
a1c0237c:	3108      	sll	s1,s0,2
a1c0237e:	9a82      	lw	a0,8(v0)
a1c02380:	e43d      	addu	a3,a0,s1
a1c02382:	9f40      	lw	v0,0(a3)
a1c02384:	2aeb      	bnez	v0,a1c0235c <$LCFI19+0x26>
a1c02386:	6d01      	li	a1,1
a1c02388:	eaab      	neg	v0,a1
a1c0238a:	b013      	lw	s0,a1c023d4 <$LCFI19+0x9e>
a1c0238c:	f0a0 c05f 	sb	v0,191(s0)
a1c02390:	6900      	li	s1,0
a1c02392:	b011      	lw	s0,a1c023d4 <$LCFI19+0x9e>
a1c02394:	d822      	sw	s1,8(s0)
a1c02396:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c0239a:	6c81      	li	a0,129
a1c0239c:	d823      	sw	s1,12(s0)
a1c0239e:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023a2:	6c82      	li	a0,130
a1c023a4:	d824      	sw	s1,16(s0)
a1c023a6:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023aa:	6c83      	li	a0,131
a1c023ac:	d825      	sw	s1,20(s0)
a1c023ae:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023b2:	6c01      	li	a0,1
a1c023b4:	d826      	sw	s1,24(s0)
a1c023b6:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023ba:	6c02      	li	a0,2
a1c023bc:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023c0:	6c80      	li	a0,128
a1c023c2:	1a03 08b6 	jal	a1c022d8 <hal_UsbDisableEp>
a1c023c6:	6791      	move	a0,s1
a1c023c8:	9707      	lw	a3,28(sp)
a1c023ca:	9106      	lw	s1,24(sp)
a1c023cc:	9005      	lw	s0,20(sp)
a1c023ce:	ef00      	jr	a3
a1c023d0:	6304      	addiu	sp,32
a1c023d2:	6500      	nop
a1c023d4:	279c      	beqz	a3,a1c0230e <$LCFI15+0x34>
a1c023d6:	a1c0      	lbu	a2,0(s1)

a1c023d8 <hal_UsbInit>:
a1c023d8:	63fd      	addiu	sp,-24

a1c023da <$LCFI23>:
a1c023da:	6c01      	li	a0,1
a1c023dc:	6d00      	li	a1,0
a1c023de:	d004      	sw	s0,16(sp)
a1c023e0:	eb8b      	neg	v1,a0
a1c023e2:	b00a      	lw	s0,a1c02408 <$LCFI23+0x2e>
a1c023e4:	6205      	sw	ra,20(sp)
a1c023e6:	f0a0 c0bd 	sb	a1,189(s0)
a1c023ea:	f0c0 c0a2 	sb	a1,194(s0)
a1c023ee:	f0c0 c068 	sb	v1,200(s0)
a1c023f2:	1a03 08cd 	jal	a1c02334 <hal_UsbClrConfig>
a1c023f6:	6500      	nop
a1c023f8:	6a00      	li	v0,0
a1c023fa:	1a03 08ac 	jal	a1c022b0 <hal_UsbResetTransfert>
a1c023fe:	d841      	sw	v0,4(s0)
a1c02400:	9705      	lw	a3,20(sp)
a1c02402:	9004      	lw	s0,16(sp)
a1c02404:	ef00      	jr	a3
a1c02406:	6303      	addiu	sp,24
a1c02408:	279c      	beqz	a3,a1c02342 <$LCFI19+0xc>
a1c0240a:	a1c0      	lbu	a2,0(s1)

a1c0240c <hal_UsbConfig>:
a1c0240c:	63fb      	addiu	sp,-40

a1c0240e <$LCFI32>:
a1c0240e:	b525      	lw	a1,a1c024a0 <$LCFI32+0x92>
a1c02410:	d007      	sw	s0,28(sp)
a1c02412:	6c04      	li	a0,4
a1c02414:	b024      	lw	s0,a1c024a4 <$LCFI32+0x96>
a1c02416:	6209      	sw	ra,36(sp)
a1c02418:	1a03 0142 	jal	a1c00508 <hal_HstSendEvent>
a1c0241c:	d108      	sw	s1,32(sp)
a1c0241e:	98a0      	lw	a1,0(s0)
a1c02420:	69ff      	li	s1,255
a1c02422:	f060 a598 	lbu	a0,120(a1)
a1c02426:	6a0f      	li	v0,15
a1c02428:	ec2c      	and	a0,s1
a1c0242a:	ea8c      	and	v0,a0
a1c0242c:	3392      	srl	v1,a0,4
a1c0242e:	e279      	addu	a2,v0,v1
a1c02430:	b71e      	lw	a3,a1c024a8 <$LCFI32+0x9a>
a1c02432:	4e01      	addiu	a2,1
a1c02434:	f0c0 c7c0 	sb	a2,192(a3)
a1c02438:	4d01      	addiu	a1,1
a1c0243a:	a580      	lbu	a0,0(a1)
a1c0243c:	6b40      	li	v1,64
a1c0243e:	ec2c      	and	a0,s1
a1c02440:	ec6d      	or	a0,v1
a1c02442:	ec2c      	and	a0,s1
a1c02444:	c580      	sb	a0,0(a1)
a1c02446:	6c00      	li	a0,0
a1c02448:	d305      	sw	v1,20(sp)
a1c0244a:	1a03 093e 	jal	a1c024f8 <musb_select_ep>
a1c0244e:	d704      	sw	a3,16(sp)
a1c02450:	98a0      	lw	a1,0(s0)
a1c02452:	6c04      	li	a0,4
a1c02454:	4547      	addiu	v0,a1,7
a1c02456:	4a18      	addiu	v0,24
a1c02458:	a2c0      	lbu	a2,0(v0)
a1c0245a:	9204      	lw	v0,16(sp)
a1c0245c:	ee2c      	and	a2,s1
a1c0245e:	f0c0 c2c4 	sb	a2,196(v0)
a1c02462:	eccc      	and	a0,a2
a1c02464:	9305      	lw	v1,20(sp)
a1c02466:	2416      	beqz	a0,a1c02494 <$LCFI32+0x86>
a1c02468:	6f08      	li	a3,8
a1c0246a:	f0c0 c263 	sb	v1,195(v0)
a1c0246e:	f060 cde4 	sh	a3,100(a1)
a1c02472:	9860      	lw	v1,0(s0)
a1c02474:	6c06      	li	a0,6
a1c02476:	4b62      	addiu	v1,98
a1c02478:	c380      	sb	a0,0(v1)
a1c0247a:	f0c0 a2c3 	lbu	a2,195(v0)
a1c0247e:	98a0      	lw	a1,0(s0)
a1c02480:	37ce      	srl	a3,a2,3
a1c02482:	ef2c      	and	a3,s1
a1c02484:	4d66      	addiu	a1,102
a1c02486:	f0c0 c2c3 	sb	a2,195(v0)
a1c0248a:	cde0      	sh	a3,0(a1)
a1c0248c:	9820      	lw	s1,0(s0)
a1c0248e:	6806      	li	s0,6
a1c02490:	4963      	addiu	s1,99
a1c02492:	c900      	sh	s0,0(s1)
a1c02494:	9709      	lw	a3,36(sp)
a1c02496:	9108      	lw	s1,32(sp)
a1c02498:	9007      	lw	s0,28(sp)
a1c0249a:	ef00      	jr	a3
a1c0249c:	6305      	addiu	sp,40
a1c0249e:	6500      	nop
a1c024a0:	3003      	sra	s0,8
a1c024a2:	1406      	b	a1c01cb0 <hal_SysGetFrequencyConfiguration+0x38>
a1c024a4:	2868      	bnez	s0,a1c02576 <g_tgtSxNbCfg+0x12>
a1c024a6:	a1c0      	lbu	a2,0(s1)
a1c024a8:	279c      	beqz	a3,a1c023e2 <$LCFI23+0x8>
a1c024aa:	a1c0      	lbu	a2,0(s1)

a1c024ac <hal_UsbReset>:
a1c024ac:	6aff      	li	v0,255
a1c024ae:	ea8c      	and	v0,a0
a1c024b0:	63fd      	addiu	sp,-24

a1c024b2 <$LCFI36>:
a1c024b2:	7201      	cmpi	v0,1
a1c024b4:	6205      	sw	ra,20(sp)
a1c024b6:	6106      	btnez	a1c024c4 <$LCFI36+0x12>
a1c024b8:	1a03 08f6 	jal	a1c023d8 <hal_UsbInit>
a1c024bc:	6500      	nop
a1c024be:	1a03 0903 	jal	a1c0240c <hal_UsbConfig>
a1c024c2:	6500      	nop
a1c024c4:	9705      	lw	a3,20(sp)
a1c024c6:	ef00      	jr	a3
a1c024c8:	6303      	addiu	sp,24
a1c024ca:	6500      	nop

a1c024cc <hal_LpsClockSysChangeAllowed>:
a1c024cc:	b30a      	lw	v1,a1c024f4 <$LCFI18+0x22>
a1c024ce:	a340      	lbu	v0,0(v1)
a1c024d0:	63fc      	addiu	sp,-32

a1c024d2 <$LCFI18>:
a1c024d2:	6207      	sw	ra,28(sp)
a1c024d4:	2204      	beqz	v0,a1c024de <$LCFI18+0xc>
a1c024d6:	9707      	lw	a3,28(sp)
a1c024d8:	6a01      	li	v0,1
a1c024da:	ef00      	jr	a3
a1c024dc:	6304      	addiu	sp,32
a1c024de:	1a03 071b 	jal	a1c01c6c <hal_SysGetFreq>
a1c024e2:	d404      	sw	a0,16(sp)
a1c024e4:	9504      	lw	a1,16(sp)
a1c024e6:	9707      	lw	a3,28(sp)
a1c024e8:	ed43      	sltu	a1,v0
a1c024ea:	6758      	move	v0,t8
a1c024ec:	6c01      	li	a0,1
a1c024ee:	ea8e      	xor	v0,a0
a1c024f0:	ef00      	jr	a3
a1c024f2:	6304      	addiu	sp,32
a1c024f4:	8a68      	lh	v1,16(v0)
a1c024f6:	a1c0      	lbu	a2,0(s1)

a1c024f8 <musb_select_ep>:
a1c024f8:	6aff      	li	v0,255
a1c024fa:	ea8c      	and	v0,a0
a1c024fc:	5a0f      	sltiu	v0,15
a1c024fe:	6004      	bteqz	a1c02508 <musb_select_ep+0x10>
a1c02500:	b403      	lw	a0,a1c0250c <musb_select_ep+0x14>
a1c02502:	9c60      	lw	v1,0(a0)
a1c02504:	4b0e      	addiu	v1,14
a1c02506:	c340      	sb	v0,0(v1)
a1c02508:	e820      	jr	ra
a1c0250a:	6500      	nop
a1c0250c:	2868      	bnez	s0,a1c025de <CSWTCH.2+0x2e>
a1c0250e:	a1c0      	lbu	a2,0(s1)

Disassembly of section .sramdata:

a1c02510 <_sys_sram_data_start>:
a1c02510:	02e450a0 	0x2e450a0

a1c02512 <g_pmdMiscControlLp>:
a1c02512:	400002e4 	0x400002e4

a1c02514 <g_pmdCalibSetting2>:
a1c02514:	00244000 	0x244000

a1c02516 <g_pmdMiscControlActive>:
a1c02516:	00ff0024 	and	zero,a3,ra

a1c02518 <g_halIspiProperties>:
a1c02518:	000000ff 	0xff
a1c0251c:	00c65d40 	0xc65d40
a1c02520:	73616c46 	0x73616c46
a1c02524:	72502068 	0x72502068
a1c02528:	6172676f 	0x6172676f
a1c0252c:	72656d6d 	0x72656d6d
a1c02530:	72655620 	0x72655620
a1c02534:	6e6f6973 	0x6e6f6973
a1c02538:	00003120 	0x3120

a1c0253c <dataAry.2749>:
a1c0253c:	a1c02890 	sb	zero,10384(t6)
a1c02540:	a1c0488c 	sb	zero,18572(t6)
a1c02544:	a1c04890 	sb	zero,18576(t6)
a1c02548:	a1c0688c 	sb	zero,26764(t6)
a1c0254c:	a1c06890 	sb	zero,26768(t6)
a1c02550:	a1c0888c 	sb	zero,-30580(t6)

a1c02554 <pattern.2748>:
a1c02554:	a5a5a5a5 	sh	a1,-23131(t5)
a1c02558:	5a5a5a5a 	0x5a5a5a5a
a1c0255c:	4f35b7da 	c3	0x135b7da
a1c02560:	8e354c91 	lw	s5,19601(s1)

a1c02564 <g_tgtSxNbCfg>:
a1c02564:	00000106 	0x106
a1c02568:	00960000 	0x960000
a1c0256c:	01061402 	0x1061402
a1c02570:	001e0001 	0x1e0001
a1c02574:	01000802 	0x1000802
a1c02578:	00780507 	0x780507
a1c0257c:	012c0002 	0x12c0002
a1c02580:	00fa0064 	0xfa0064
a1c02584:	0008101e 	0x8101e

a1c02588 <g_memdFlashBankLayout>:
a1c02588:	00000000 	nop
a1c0258c:	0000003c 	0x3c
a1c02590:	00004000 	sll	t0,zero,0x0
a1c02594:	00000010 	mfhi	zero
a1c02598:	00001000 	sll	v0,zero,0x0
	...
a1c025a4:	77656956 	jalx	ad95a558 <_int_rom_end+0xbb5655c>
a1c025a8:	42444720 	c0	0x444720
a1c025ac:	00000000 	nop

a1c025b0 <CSWTCH.2>:
a1c025b0:	00000008 	jr	zero
a1c025b4:	00000001 	0x1
a1c025b8:	204b3233 	addi	t3,v0,12851
a1c025bc:	636f6c63 	0x636f6c63
a1c025c0:	7369206b 	0x7369206b
a1c025c4:	746f6e20 	jalx	a1bdb880 <_BB_DUALPORT_SRAM_end+0xdb880>
a1c025c8:	6e6f6320 	0x6e6f6320
a1c025cc:	75676966 	jalx	a59da598 <_int_rom_end+0x3bd659c>
a1c025d0:	00646572 	0x646572
a1c025d4:	72706d49 	0x72706d49
a1c025d8:	7265706f 	0x7265706f
a1c025dc:	65724620 	0x65724620
a1c025e0:	73202e71 	0x73202e71
a1c025e4:	696c6163 	0x696c6163
a1c025e8:	6620676e 	0x6620676e
a1c025ec:	75716572 	jalx	a5c595c8 <_int_rom_end+0x3e555cc>
a1c025f0:	79636e65 	0x79636e65
a1c025f4:	6425203a 	0x6425203a
a1c025f8:	00000000 	nop
a1c025fc:	72706d49 	0x72706d49
a1c02600:	7265706f 	0x7265706f
a1c02604:	73797320 	0x73797320
a1c02608:	206d6574 	addi	t5,v1,25972
a1c0260c:	71657266 	0x71657266
a1c02610:	636e6575 	0x636e6575
a1c02614:	25203a79 	addiu	zero,t1,14969
a1c02618:	00000064 	0x64
a1c0261c:	636f6c43 	0x636f6c43
a1c02620:	754f206b 	jalx	a53c81ac <_int_rom_end+0x35c41b0>
a1c02624:	64492074 	0x64492074
a1c02628:	74756f20 	jalx	a1d5bc80 <_sys_sram_non_romed_top+0x14bc84>
a1c0262c:	20666f20 	addi	a2,v1,28448
a1c02630:	676e6172 	0x676e6172
a1c02634:	25203a65 	addiu	zero,t1,14949
a1c02638:	00000064 	0x64
a1c0263c:	61766e49 	0x61766e49
a1c02640:	2064696c 	addi	a0,v1,26988
a1c02644:	636f6c63 	0x636f6c63
a1c02648:	756f206b 	jalx	a5bc81ac <_int_rom_end+0x3dc41b0>
a1c0264c:	64492074 	0x64492074
a1c02650:	6f6e2820 	0x6f6e2820
a1c02654:	706f2074 	0x706f2074
a1c02658:	64656e65 	0x64656e65
a1c0265c:	25203a29 	addiu	zero,t1,14889
a1c02660:	00000064 	0x64
a1c02664:	636f6c43 	0x636f6c43
a1c02668:	754f206b 	jalx	a53c81ac <_int_rom_end+0x35c41b0>
a1c0266c:	72462074 	0x72462074
a1c02670:	65757165 	0x65757165
a1c02674:	2079636e 	addi	t9,v1,25454
a1c02678:	2074756f 	addi	s4,v1,30063
a1c0267c:	7220666f 	0x7220666f
a1c02680:	65676e61 	0x65676e61
a1c02684:	6425203a 	0x6425203a
a1c02688:	00000000 	nop

a1c0268c <g_halSysFreqSteps>:
a1c0268c:	03197500 	0x3197500
a1c02690:	0632ea00 	0x632ea00
a1c02694:	094c5f00 	j	a5317c00 <_int_rom_end+0x3513c04>
a1c02698:	0c65d400 	jal	a1975000 <_sys_sram_heap_size+0xa196eb34>
a1c0269c:	0ee09800 	jal	ab826000 <_int_rom_end+0x9a22004>
a1c026a0:	1298be00 	beq	s4,t8,a1bf1ea4 <_BB_DUALPORT_SRAM_end+0xf1ea4>

a1c026a4 <CSWTCH.56>:
a1c026a4:	018cba80 	0x18cba80
a1c026a8:	025317c0 	0x25317c0
a1c026ac:	03197500 	0x3197500
a1c026b0:	04a62f80 	0x4a62f80
a1c026b4:	05503649 	bltzal	t2,a1c0ffdc <_sys_sram_irq_stackwatch_end+0xde0>
a1c026b8:	0632ea00 	0x632ea00
a1c026bc:	06c32dd1 	0x6c32dd1
a1c026c0:	07704c00 	bltzal	k1,a1c156c4 <_sys_sram_non_romed_top+0x56c8>
a1c026c4:	0843e2ab 	j	a10f8aac <_sys_sram_heap_size+0xa10f25e0>
a1c026c8:	094c5f00 	j	a5317c00 <_int_rom_end+0x3513c04>
a1c026cc:	0aa06c92 	j	aa81b248 <_int_rom_end+0x8a1724c>
a1c026d0:	0c65d400 	jal	a1975000 <_sys_sram_heap_size+0xa196eb34>
a1c026d4:	0ee09800 	jal	ab826000 <_int_rom_end+0x9a22004>
a1c026d8:	1298be00 	beq	s4,t8,a1bf1edc <_BB_DUALPORT_SRAM_end+0xf1edc>
a1c026dc:	5f6c6168 	0x5f6c6168
a1c026e0:	44627355 	0x44627355
a1c026e4:	62617369 	0x62617369
a1c026e8:	7045656c 	0x7045656c
a1c026ec:	7045203a 	0x7045203a
a1c026f0:	6d756e20 	0x6d756e20
a1c026f4:	20726562 	addi	s2,v1,25954
a1c026f8:	61766e69 	0x61766e69
a1c026fc:	0064696c 	0x64696c
a1c02700:	20414452 	addi	at,v0,17490
a1c02704:	7263694d 	0x7263694d
a1c02708:	656c656f 	0x656c656f
a1c0270c:	6f727463 	0x6f727463
a1c02710:	7363696e 	0x7363696e
a1c02714:	00000000 	nop
a1c02718:	666e6f43 	0x666e6f43
a1c0271c:	72756769 	0x72756769
a1c02720:	6f697461 	0x6f697461
a1c02724:	6564206e 	0x6564206e
a1c02728:	69726373 	0x69726373
a1c0272c:	726f7470 	0x726f7470
a1c02730:	6f6f7420 	0x6f6f7420
a1c02734:	67696220 	0x67696220
a1c02738:	00692520 	0x692520

a1c0273c <g_flashProgVersion>:
a1c0273c:	00000000 	nop
a1c02740:	20080121 	addi	t0,zero,289
a1c02744:	00000000 	nop
a1c02748:	a1c02520 	sb	zero,9504(t6)

a1c0274c <g_flashProgDescr>:
a1c0274c:	fb04fa01 	0xfb04fa01
	...
a1c02778:	a1c02890 	sb	zero,10384(t6)
a1c0277c:	a1c04890 	sb	zero,18576(t6)
a1c02780:	a1c06890 	sb	zero,26768(t6)
a1c02784:	00002000 	sll	a0,zero,0x0

a1c02788 <g_dif_opcode_suspend>:
a1c02788:	f53530b0 	0xf53530b0

a1c02789 <g_dif_opcode_resume>:
a1c02789:	3530      	sll	a1,s1,4

a1c0278a <g_dif_opcode_enable_QPI>:
a1c0278a:	9966f535 	lwr	a2,-2763(t3)

a1c0278b <g_dif_opcode_disable_QPI>:
a1c0278b:	66f5      	0x66f5

a1c0278c <g_dif_opcode_enable_RST>:
a1c0278c:	00009966 	0x9966

a1c0278d <g_dif_opcode_execute_RST>:
a1c0278d:	0099      	addiu	s0,sp,612
	...

a1c02790 <g_pmdLdoProfileSetting2>:
a1c02790:	87c68746 	lh	a2,-30906(s8)

a1c02794 <g_pmdLdoProfileSetting1>:
a1c02794:	f852bdfb 	0xf852bdfb

a1c02798 <g_halSysSystemFreq>:
a1c02798:	018cba80 	0x18cba80

a1c0279c <g_HalUsbVar>:
	...
a1c02858:	ff000000 	0xff000000
	...

a1c02868 <gUsbRegs>:
a1c02868:	a1a80000 	sb	t0,0(t5)
a1c0286c:	00000000 	nop

Disassembly of section .sramucdata:

a1c02870 <_sys_sram_uncached_start>:
	...

Disassembly of section .sramucbss:

a1c02890 <g_dataBufferA>:
	...

a1c04890 <g_dataBufferB>:
	...

a1c06890 <g_dataBufferC>:
	...

Disassembly of section .srambss:

a1c08890 <sxs_Boot>:
a1c08890:	00000000 	nop

a1c08894 <_ram_heap_size>:
a1c08894:	00000000 	nop

a1c08898 <g_pmdConfig>:
a1c08898:	00000000 	nop

a1c0889c <error>:
a1c0889c:	00000000 	nop

a1c088a0 <g_sys_timer_id_usb_detect>:
a1c088a0:	00000000 	nop

a1c088a4 <p_flsh_config>:
	...

a1c088b4 <g_pmdInitDone>:
a1c088b4:	00000000 	nop

a1c088b8 <g_pmdDcdcVcoreValueActiveAry>:
a1c088b8:	00000000 	nop

a1c088bc <g_pmdDcdcVcoreActRegistry>:
	...

a1c088c4 <g_pmdDcdcVcoreActCurIndex>:
a1c088c4:	00000000 	nop

a1c088c8 <g_pmdLdoUserBitmap>:
	...

a1c088fc <g_halDmaUserId>:
a1c088fc:	00000000 	nop

a1c08900 <g_halDmaMode>:
a1c08900:	00000000 	nop

a1c08904 <g_halDmaRegistry>:
a1c08904:	00000000 	nop

a1c08908 <g_halMapTable>:
	...

a1c089d8 <g_halSysFreqHandlerRegistry>:
	...

a1c08a68 <g_halLpsSleptBefore>:
a1c08a68:	00000000 	nop

a1c08a6c <g_spiflash_ID>:
	...

a1c08a78 <fp_checkflashpages>:
	...

a1c08c78 <g_halSysFreqScalRegistry>:
	...

a1c08ca0 <g_halNumberOfUserPerFreq>:
	...
