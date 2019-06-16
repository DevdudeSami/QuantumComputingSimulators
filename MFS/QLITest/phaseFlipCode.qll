-- Phase flip encode gate
gate phaseFlipEncode 3
  CNOT 0 1
  CNOT 0 2
  loop n 0 2
    H n
  endloop
endgate

-- Phase flip decode gate
gate phaseFlipDecode 3
  loop n 0 2
    H n
  endloop
  CNOT 0 1
  CNOT 0 2
  TOFF 2 1 0
endgate
