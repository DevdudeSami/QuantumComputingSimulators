link bitFlipCode
link phaseFlipCode

-- Shor encode gate
gate shorEncode 9
  phaseFlipEncode 0 3 6
  bitFlipEncode 0 1 2
  bitFlipEncode 3 4 5
  bitFlipEncode 6 7 8
endgate

-- Shor decode gate
gate shorDecode 9
  bitFlipDecode 0 1 2
  bitFlipDecode 3 4 5
  bitFlipDecode 6 7 8
  phaseFlipDecode 0 3 6
endgate
