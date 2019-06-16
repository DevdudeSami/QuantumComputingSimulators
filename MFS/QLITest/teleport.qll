-- qubit 0 is the source
-- qubit 1 is the ancillary
-- qubit 2 is the target

gate teleport 3

  let source 0
  let ancillary 1
  let target 2

  H source
  H target

  CNOT target ancillary

  CNOT source ancillary

  H source
  CNOT ancillary target

  H target

  CNOT source target

  H source
  H ancillary

endgate
