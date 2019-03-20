-- Test function for Shor code

link shorCode

gate testShorCode 9
  shorEncode all

  X 0
  Z 0
  H 0
  Y 0
  P 0

  shorDecode all
endgate
