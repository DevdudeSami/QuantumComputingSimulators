-- Phase flip encode gate
gate phaseFlipEncode 3
  CNOT 0 1
  CNOT 0 2
  H 0
  H 1
  H 2
endgate

-- Phase flip decode gate
gate phaseFlipDecode 3
  H 0
  H 1
  H 2
  CNOT 0 1
  CNOT 0 2
  TOFF 2 1 0
endgate
