from State import State

class Accumulator:
  
  def __init__(self, state):
    self.state = state

  def takeMeasurements(self, n):
    statesCount = {}
    for _ in range(0,n):
      measurement = self.state.measure
      if measurement in statesCount: statesCount[measurement] += 1
      else: statesCount[measurement] = 1

    return statesCount

# myAcc = Accumulator(Qubit.fromProbability(0.8))
# print(myAcc.takeMeasurements(100000))

# myAcc = Accumulator(State(2, [1,1,0,0]))
# print(myAcc.takeMeasurements(1000))