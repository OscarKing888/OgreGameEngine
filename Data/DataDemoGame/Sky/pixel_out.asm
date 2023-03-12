ps_2_0

dcl v0
dcl t0.xy
dcl_2d s0
dcl_2d s1

texld r0, t0, s0
texld r1, t0, s1

mov oC0, r0
mov oC1, r1