link bitFlipCode
link phaseFlipCode

-- Shor encode gate
gate shorEncode 9
  phaseFlipEncode 0 3 6

  loop n 0 2
    bitFlipEncode 3*n 3*n+1 3*n+2
  endloop
endgate

-- Shor decode gate
gate shorDecode 9
  loop n 0 2
    bitFlipDecode 3*n 3*n+1 3*n+2
  endloop

  phaseFlipDecode 0 3 6
endgate
