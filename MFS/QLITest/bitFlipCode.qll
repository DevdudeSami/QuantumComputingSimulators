-- Bit flip encode gate
gate bitFlipEncode 3
  CNOT 0 1
  CNOT 0 2
endgate

-- Bit flip decode gate
gate bitFlipDecode 3
  CNOT 0 1
  CNOT 0 2
  TOFF 2 1 0
endgate
