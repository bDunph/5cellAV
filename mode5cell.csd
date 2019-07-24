<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o moogvcf.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 48000
kr = 4800
ksmps = 10
nchnls = 2

instr 1; 5 modes excitator

idur 	init p3
iamp    init ampdb(p4)
ifreq11 init p5
ifreq12 init p6
ifreq13 init p7
ifreq14 init p8
ifreq15 init p9
iQ11    init p10
iQ12    init p11
iQ13 	init p12
iQ14	init p13
iQ15	init p14
ifreq21 init p15
ifreq22 init p16
ifreq23 init p17
ifreq24 init p18
ifreq25 init p19
iQ21    init p20
iQ22    init p21
iQ23	init p22
iQ24	init p23
iQ25	init p24

;get input vals
;kQval chnget "qValue"
;kPulseVal chnget "pulse"

; to simulate the shock between the excitator and the resonator
ashock  mpulse  1,2

;aSig	phasor	440

aexc1	mode 	ashock,	ifreq11,	iQ11
aexc1 = aexc1*iamp

aexc2  	mode 	ashock,	ifreq12,	iQ12
aexc2 = aexc2*iamp

aexc3   mode 	ashock,	ifreq13,	iQ13
aexc3 = aexc3*iamp

aexc4   mode 	ashock, ifreq14,	iQ14
aexc4 = aexc4*iamp

aexc5   mode 	ashock,	ifreq15,	iQ15
aexc5 = aexc5*iamp

aexc = (aexc1+aexc2+aexc3+aexc4+aexc5)/5

;"Contact" condition : when aexc reaches 0, the excitator looses 
;contact with the resonator, and stops "pushing it"
aexc limit aexc,0,3*iamp 

; 5modes resonator

ares1	mode	aexc,	ifreq21,	iQ21	

ares2	mode	aexc,	ifreq22,	iQ22

ares3	mode	aexc,	ifreq23,	iQ23

ares4	mode	aexc,	ifreq24,	iQ24

ares5	mode	aexc,	ifreq25,	iQ25

ares = (ares1+ares2+ares3+ares4+ares5)/5

gaOut = aexc + ares

;display aexc+ares,p3
;outs  aexc+ares,aexc+ares

;endin

;instr 2 ; spatialisation of sound source in 3D space

;aOut = aOut + 0.000001 * 0.000001

;kXpos0 chnget "xPos0"
;kYpos0 chnget "yPos0"
;kZpos0 chnget "zPos0"
;kXpos1 chnget "xPos1"
;kYpos1 chnget "yPos1"
;kZpos1 chnget "zPos1"
;kXpos2 chnget "xPos2"
;kYpos2 chnget "yPos2"
;kZpos2 chnget "zPos2"
;kXpos3 chnget "xPos3"
;kYpos3 chnget "yPos3"
;kZpos3 chnget "zPos3"
;kXpos4 chnget "xPos4"
;kYpos4 chnget "yPos4"
;kZpos4 chnget "zPos4"

;idist = sqrt(12) ; ****need to cahnge this***
;ift = 4
;imode = 1
;imdel = (4 + 1) * sqrt(12) / 340.0 ; (R + 1) * sqrt(H*H + W*W + D*D) / 340.0
;iovr = 2

;a0W, a0X, a0Y, a0Z spat3d aOut, kXpos0, kYpos0, kZpos0, idist, ift, imode, imdel, iovr
;a0W = a0W * 1.4142
;a0L = a0W + a0Y
;a0R = a0W - a0Y
;
;a1W, a1X, a1Y, a1Z spat3d aOut, kXpos1, kYpos1, kZpos1, idist, ift, imode, imdel, iovr
;a1W = a1W * 1.4142
;a1L = a1W + a1Y
;a1R = a1W - a1Y
;
;a2W, a2X, a2Y, a2Z spat3d aOut, kXpos2, kYpos2, kZpos2, idist, ift, imode, imdel, iovr
;a2W = a2W * 1.4142
;a2L = a2W + a2Y
;a2R = a2W - a2Y
;
;a3W, a3X, a3Y, a3Z spat3d aOut, kXpos3, kYpos3, kZpos3, idist, ift, imode, imdel, iovr
;a3W = a3W * 1.4142
;a3L = a3W + a3Y
;a3R = a3W - a3Y
;
;a4W, a4X, a4Y, a4Z spat3d aOut, kXpos4, kYpos4, kZpos4, idist, ift, imode, imdel, iovr
;a4W = a4W * 1.4142
;a4L = a4W + a4Y
;a4R = a4W - a4Y
;
;aOutL = (a0L + a1L + a2L + a3L + a4L)/5
;aOutR = (a0R + a1R + a2R + a3R + a4R)/5
;
;outs	aOutL, aOutR
endin

instr 20 ;test instrument for chnget

kx	chnget  "x"
ky	chnget	"y" 
kz	chnget	"z"

a1   oscil     ampdb(70), 440 

idistance = 1.0 ; ****need to cahnge this***
ift = 1
imode = 1
imdel = 2;(4 + 1) * sqrt(12) / 340.0 ; (R + 1) * sqrt(H*H + W*W + D*D) / 340.0
iovr = 2

aW, aX, aY, aZ spat3d a1, kx, ky, kz, 1.0, 1, 1, 2, 2 
aW = aW * 1.4142
aL = aW + aY
aR = aW - aY

outs       aL, aR

endin

instr 30 ; test hrtf instrument

kAzimuth	chnget	"azimuth"
kElevation	chnget	"elevation"
kSourceDist	chnget	"distance"

aSig	oscil	ampdb(80),	440

aleft,	aright	hrtfmove2	aSig,	kAzimuth,	kElevation,	"hrtf-48000-left.dat",	"hrtf-48000-right.dat"

aL = aleft * 1 / (kSourceDist + 0.1)
aR = aright * 1 / (kSourceDist + 0.1)

outs	aL,	aR
endin


</CsInstruments>
<CsScore>
;p1	p2	p3	p4	p5	p6	p7	p8	p9	p10	p11	p12	p13	p14	p15	p16	p17	p18	p19	p20	p21	p22	p23	p24
;With a metallic excitator

i1 	0 	90 	50	50	70	82	80	90	1000  	720  	850	700	820	440	882  	660	220	442	500	400	350	130	200
i1	5	90	50	1000	3000	1000	3000	1000	12	8	12	8	12	80	180	80	180	80	8	3	8	3	8

;i20	0	90

i30	0	90
</CsScore>
</CsoundSynthesizer>
