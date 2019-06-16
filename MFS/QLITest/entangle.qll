-- Entangle 2 qubits
gate entangle 2
  H 0
  CNOT 0 1
endgate

-- Entangle 5 qubits
gate entangle5 5
  H 0
  loop n 1 4
    CNOT 0 n
  endloop
endgate

-- Entangle n qubits
gate entangleAll
  H 0
  loop n 1 argc-1
    CNOT 0 n
  endloop
endgate
