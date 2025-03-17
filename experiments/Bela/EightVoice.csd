<CsoundSynthesizer>
<CsOptions>
-Mhw:1,0,0 -+rtmidi=NULL --daemon -dm0
</CsOptions>
<CsInstruments>

sr = 48000
ksmps = 8
nchnls = 2
0dbfs = 1

maxalloc 1, 8 

instr 1
	iFreq cpsmidi
	iAmp ampmidi 0.3
	iFiltAmt = 0
	iCf = 20000
	iFiltAdd = (20000 - iCf) * iFiltAmt
	
	; Two detuned sawtooth oscillators
	aOut1 = vco2(iAmp, iFreq)
	aOut2 = vco2(iAmp, iFreq)

	; Envelope
	aOut = aOut1*0.5 + aOut2*0.5
	kEnv madsr 0.01, 0.1, 0.9, 0.01

	; Filter envelope
	kFiltEnv madsr 0.1, 0.3, 0.4, 0
	aFilt vclpf aOut*kEnv, iCf + (kFiltEnv * iFiltAdd), 0.7
	
	outs aFilt, aFilt
	chnmix aFilt*0.3, "revSend"
endin

instr 2
ain chnget "revSend"
al, ar reverbsc ain, ain, 0.8, 14000
outs al, ar
chnclear "revSend"
endin


</CsInstruments>
<CsScore>
i 2 0 100000000
</CsScore>
</CsoundSynthesizer>