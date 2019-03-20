def takeMeasurements(reg, n):
  statesCount = {}
  for _ in range(0,n):
    measurement = reg.measure()
    if measurement in statesCount: statesCount[measurement] += 1
    else: statesCount[measurement] = 1

  return statesCount
