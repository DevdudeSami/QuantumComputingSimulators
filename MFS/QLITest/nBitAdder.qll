gate nBitAdd

  let N argc

  -- step 1
  loop i 3 N-2-1 2
    CNOT i+1 i
  endloop

  -- step 2
  loop i 0 N-5-1 2
    CNOT i+4 i+2
    TOFF i i+1 i+2
  endloop

  -- step 3
  CNOT N-2 N-1

  -- step 4
  loop i 3 N-4-1 2
    X i
  endloop

  -- step 5
  TOFF N-4 N-3 N-1

  -- step 6
  loop i 2 N-3-1 2
    CNOT i i+1
  endloop

  -- step 7
  TOFF N-6 N-5 N-4

  -- step 8
  loop i N-8 0 -2
    TOFF i i+1 i+2
    X i+3
    CNOT i+6 i+4
  endloop

  -- steps 9 and 10
  CNOT 4 2
  CNOT 1 0

  -- step 11
  loop i 3 N-2-1 2
    CNOT i+1 i
  endloop

endgate
